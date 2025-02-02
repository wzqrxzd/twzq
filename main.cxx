#include "config_manager.hxx"
#include "json.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <memory>

#include "spletnya_color_analyzer.hxx"

using json = nlohmann::json;

int chooseColor(const std::array<Color, 5>& colors);
std::string rgbToHex(const Color& col);
std::string rgbaToHex(const Color& col);

int main(int argc, char* argv[])
{
  constexpr double activeFactor = 0.8;
  constexpr double inactiveFactor = 0.6;

  if (argc < 3)
  {
    spdlog::error("Usage: twzq \"walpaper\" \"name_theme\" [-icon path_to_icon]");
    return 0;
  }

  const fs::path themePath = std::getenv("HOME") + std::string("/.config/tpzq/themes/") + argv[2];

  ConfigManager configMgr(themePath);
  json config = configMgr.getConfig();

  const fs::path wallpaperPath = config["wallpaper_dir"].get<std::string>() + argv[1];

  ImageColorAnalyzer imageAnalyzer(wallpaperPath);
  std::array<Color, 5> colors = imageAnalyzer.analyze();

  int colorIndex = chooseColor(colors);

  Color activeColor(colors[colorIndex].r*activeFactor,
                    colors[colorIndex].g*activeFactor,
                    colors[colorIndex].b*activeFactor);

  Color inactiveColor(colors[colorIndex].r*inactiveFactor,
                      colors[colorIndex].g*inactiveFactor,
                      colors[colorIndex].b*inactiveFactor);

  json theme;

  theme["hyprland"]["active_color"] = rgbaToHex(activeColor);
  theme["hyprland"]["inactive_color"] = rgbaToHex(inactiveColor);
  theme["waybar"]["color"] = rgbToHex(activeColor);
  theme["wofi"]["color"] = rgbToHex(activeColor);
  theme["tpzq"]["accent_color"] = rgbToHex(activeColor);
  theme["tpzq"]["hovered_color"] = rgbToHex(inactiveColor);
  theme["cava"]["color"] = rgbToHex(activeColor);
  theme["dunst"]["color"] = rgbToHex(activeColor);
  theme["wallpaper"]["current"] = wallpaperPath.string();

  configMgr.saveTheme(theme);
}

int chooseColor(const std::array<Color, 5>& colors)
{
  for (int i{0}; i < colors.size(); i++)
  {
    std::cout << "\x1b[48;2;" 
              << static_cast<int>(colors[i].r) << ";" 
              << static_cast<int>(colors[i].g) << ";"
              << static_cast<int>(colors[i].b) << "m"
              << i << ": " << static_cast<int>(colors[i].r) 
              << ", " << static_cast<int>(colors[i].g) << ", " 
              << static_cast<int>(colors[i].b) << "\x1b[0m" << std::endl;
  }

  int index;
  std::cout << "Choose color: ";
  std::cin >> index;
  if (index > colors.size() || index < 0)
    throw std::invalid_argument("Invalid color index.");
  return index;
}

std::string rgbToHex(const Color& col)
{
  std::stringstream ss;
  ss << '#'
     << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << col.color_value;
  std::string hexColor = ss.str();
  return hexColor.substr(0, hexColor.size() - 2);
}

std::string rgbaToHex(const Color& col)
{
  std::stringstream ss;
  std::stringstream rgba;
  ss << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << col.color_value;

  rgba << "rgba(" << ss.str() << ")";

  return rgba.str();
}
