#include <fstream>
#include "json.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <regex>

#include "spletnya_color_analyzer.hxx"
#include "hyprland_controller.hxx"
#include "waybar_controller.hxx"
#include "wallpaper_controller.hxx"
#include "wofi_controller.hxx"

using json = nlohmann::json;

int extractNumber(const std::filesystem::path& path);
std::map<std::filesystem::path, int> getImagesMap(const std::filesystem::path& path);
std::filesystem::path chooseWallpaper(const std::vector<std::pair<std::filesystem::path, int>>& imagesPaths);
int chooseColor(const std::array<Color, 5>& colors);
std::string rgbToHex(const Color& col);
std::string rgbaToHex(const Color& col);

int main()
{
  std::ifstream configFile("config.json");
  if (!configFile.is_open())
  {
    spdlog::error("Failed to open config.json file");
    return -1;
  }

  json config;
  try {
    configFile >> config;
  } catch (const json::exception& e) {
    spdlog::error("Error reading JSON: {}", e.what());
    return -1;
  }

  if (!config.contains("wallpaper") && config["wallpaper"].contains("path_dir"))
  {
    spdlog::error("wallpaper: path_dir not exists");
    return -1;
  }

  std::map<std::filesystem::path, int> wallpaperImages(getImagesMap(config["wallpaper"]["path_dir"].get<std::string>()));
  std::vector<std::pair<std::filesystem::path, int>> sortedImages(wallpaperImages.begin(), wallpaperImages.end());

  std::sort(sortedImages.begin(), sortedImages.end(), [](const std::pair<std::filesystem::path, int>& a, const std::pair<std::filesystem::path, int>& b) {
    return a.second < b.second;
  });

  std::filesystem::path choosenImagePath = chooseWallpaper(sortedImages);
  config["wallpaper"]["current"] = choosenImagePath.string();

  ImageColorAnalyzer img(choosenImagePath);
  std::array<Color, 5> colors = img.analyze();

  int colorIndex = chooseColor(colors);
  Color activeColor(colors[colorIndex].r*0.8, colors[colorIndex].g*0.8, colors[colorIndex].b*0.8);
  Color inactiveColor(colors[colorIndex].r*0.6, colors[colorIndex].g*0.6, colors[colorIndex].b*0.6);

  config["hyprland"]["active_color"] = rgbaToHex(activeColor);
  config["hyprland"]["inactive_color"] = rgbaToHex(inactiveColor);
  HyprlandController hyprland(&config);
  hyprland.apply();

  config["waybar"]["color"] = rgbToHex(activeColor);
  WaybarController waybar(&config);
  waybar.apply();

  config["wofi"]["color"] = rgbToHex(activeColor);
  WofiController wofi(&config);
  wofi.apply();

  WallpaperController wallpaper(&config);
  wallpaper.apply();

  std::ofstream ofile("config.json");
  if (!ofile.is_open())
  {
    spdlog::error("Failed to open config.json file");
    return 1;
  }

  ofile << config.dump(4);
  ofile.close();
}

int extractNumber(const std::filesystem::path& path) {
    std::string filename = path.filename().string();
    std::regex re(R"(walp(\d+)\.\w+)");

    std::smatch match;
    if (std::regex_search(filename, match, re)) {
        return std::stoi(match[1].str());
    }
    return -1;
}

std::map<std::filesystem::path, int> getImagesMap(const std::filesystem::path& path)
{
  std::map<std::filesystem::path, int> wallpaperImages;
  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    if (std::filesystem::is_regular_file(entry))
      wallpaperImages[entry.path()] = extractNumber(entry.path());
  }

  return wallpaperImages;
}

std::filesystem::path chooseWallpaper(const std::vector<std::pair<std::filesystem::path, int>>& imagesPaths)
{
  for (const auto& val : imagesPaths)
    std::cout << "Image: " << val.first.string() << std::endl;

  std::filesystem::path imgPath;
  int index;

  std::cout << "enter index: ";
  std::cin >> index;

  for (const auto& pair : imagesPaths)
  {
    if (pair.second == index)
      imgPath = pair.first;
  }

  return imgPath;
}

int chooseColor(const std::array<Color, 5>& colors)
{
  for (int i{0}; i < colors.size(); i++)
  {
    std::cout << "\x1b[48;2;" << colors[i].r << ";" 
              << colors[i].g << ";" << colors[i].b 
              << "m" << i << ": " << colors[i].r 
              << ", " << colors[i].g << ", " 
              << colors[i].b << "\x1b[0m" << std::endl;
  }

  int index;
  std::cout << "Choose color: ";
  std::cin >> index;
  if (index > colors.size() || index < 0)
    throw std::invalid_argument("invalid color index");
  return index;
}

std::string rgbToHex(const Color& col)
{
  std::stringstream ss;
  ss << '#'
     << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << col.r
     << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << col.g
     << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << col.b;
  return ss.str();
}

std::string rgbaToHex(const Color& col)
{
  std::stringstream ss;
  std::stringstream rgba;
  ss << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << col.r
     << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << col.g
     << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << col.b
     << std::setw(2) << std::setfill('0') << std::hex << std::uppercase << 255;

  rgba << "rgba(" << ss.str() << ")";

  return rgba.str();
}
