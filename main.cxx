#include "json.hpp"
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <memory>

#include "spletnya_color_analyzer.hxx"
#include "hyprland_controller.hxx"
#include "waybar_controller.hxx"
#include "wallpaper_controller.hxx"
#include "wofi_controller.hxx"
#include "config_manager.hxx"
#include "wallpaper_manager.hxx"

using json = nlohmann::json;

int chooseColor(const std::array<Color, 5>& colors);
std::string rgbToHex(const Color& col);
std::string rgbaToHex(const Color& col);

int main()
{
  ConfigManager cfgManager("config.json");
  json config(cfgManager.loadConfig());

  WallpaperManager wallpaperManager(&config);
  fs::path choosenImagePath(wallpaperManager.chooseWallpaper());

  config["wallpaper"]["current"] = choosenImagePath.string();

  ImageColorAnalyzer imageAnalyzer(choosenImagePath);
  std::array<Color, 5> colors = imageAnalyzer.analyze();

  int colorIndex = chooseColor(colors);

  Color activeColor(colors[colorIndex].r*0.8, colors[colorIndex].g*0.8, colors[colorIndex].b*0.8);
  Color inactiveColor(colors[colorIndex].r*0.6, colors[colorIndex].g*0.6, colors[colorIndex].b*0.6);

  config["hyprland"]["active_color"] = rgbaToHex(activeColor);
  config["hyprland"]["inactive_color"] = rgbaToHex(inactiveColor);
  config["waybar"]["color"] = rgbToHex(activeColor);
  config["wofi"]["color"] = rgbToHex(activeColor);
  config["tpzq"]["accent_color"] = rgbToHex(activeColor);
  config["tpzq"]["hovered_color"] = rgbToHex(inactiveColor);

  std::vector<std::unique_ptr<Controller>> controllers;
  controllers.push_back(std::make_unique<WaybarController>(&config));
  controllers.push_back(std::make_unique<HyprlandController>(&config));
  controllers.push_back(std::make_unique<WofiController>(&config));
  controllers.push_back(std::make_unique<WallpaperController>(&config));

  for (const auto& controller : controllers)
    controller->apply();

  cfgManager.saveConfig(config);
}

int chooseColor(const std::array<Color, 5>& colors)
{
  for (int i{0}; i < colors.size(); i++)
  {
    std::cout << "\x1b[48;2;" << static_cast<int>(colors[i].r) << ";" 
              << static_cast<int>(colors[i].g) << ";" << static_cast<int>(colors[i].b) 
              << "m" << i << ": " << static_cast<int>(colors[i].r) 
              << ", " << static_cast<int>(colors[i].g) << ", " 
              << static_cast<int>(colors[i].b) << "\x1b[0m" << std::endl;
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
