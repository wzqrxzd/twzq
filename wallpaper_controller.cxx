#include "wallpaper_controller.hxx"
#include <fstream>
#include <regex>
#include <cstdlib>

WallpaperController::WallpaperController(nlohmann::json* json)
{
  if (json == nullptr)
    throw std::invalid_argument("json: pointer == nullptr");

  if (!json->contains("wallpaper"))
    throw std::invalid_argument("json: wallpaper cfg not exists");

  if (!(*json)["wallpaper"].contains("current"))
    throw std::invalid_argument("json: wallpaper curent not exists");

  if (!(*json)["wallpaper"].contains("hyprpaper_path"))
    throw std::invalid_argument("json: wallpaper hyprpaper not exists");

  config = json;
}

void WallpaperController::apply()
{
  std::ifstream ifile((*config)["wallpaper"]["hyprpaper_path"].get<std::string>());
  if (!ifile.is_open())
    throw std::invalid_argument("wallpaper: hyprpaper_path not vald");

  std::string configData((std::istreambuf_iterator<char>(ifile)), std::istreambuf_iterator<char>());
  ifile.close();

  std::regex pattern_preload(R"(preload = .+)");
  std::regex pattern_wallpaper(R"(wallpaper = .+)");

  configData = std::regex_replace(configData, pattern_preload, "preload = " + (*config)["wallpaper"]["current"].get<std::string>());
  configData = std::regex_replace(configData, pattern_wallpaper, "wallpaper = eDP-1," + (*config)["wallpaper"]["current"].get<std::string>());

  std::ofstream ofile((*config)["wallpaper"]["hyprpaper_path"].get<std::string>());
  if (!ofile.is_open())
    throw std::invalid_argument("wallpaper: hyprpaper_path not valid");
  ofile << configData;
  ofile.close();

  std::system("pkill hyprpaper >/dev/null 2>&1");
  std::system("hyprpaper &>/dev/null &");
}
