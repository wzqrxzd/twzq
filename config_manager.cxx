#include "config_manager.hxx"
#include <stdexcept>
#include <spdlog/spdlog.h>
#include <fstream>

ConfigManager::ConfigManager(const fs::path& configPath) : filePath{configPath}
{
  if (!fs::exists(configPath))
  {
    spdlog::info("Configuration not found. Creating default config");
    createDefaultConfig();
  }
}

json ConfigManager::loadConfig()
{
  std::ifstream configFile(filePath);
  if (!configFile.is_open())
  {
    spdlog::error("Failed to open {} file", filePath.string());
    throw std::runtime_error("Failed to open config file");
  }

  json config;
  try {
    configFile >> config;
  } catch (const json::exception& e)
  {
    spdlog::error("Error reading JSON: {}", e.what());
    throw std::runtime_error("Error reading JSON");
  }

  return config;
}

void ConfigManager::saveConfig(const json& config)
{
  std::ofstream ofile(filePath);
  if (!ofile.is_open())
  {
    spdlog::error("Failed to open {} file", filePath.string());
    throw std::runtime_error("Failed to open config file");
  }

  ofile << config.dump(4);
  ofile.close();
}

void ConfigManager::createDefaultConfig()
{
  json defaultConfig = {
    {"hyprland", {
      {"active_color", ""},
      {"inactive_color", ""},
      {"path", "/home/wzqrxzd/.config/hypr/hyprland.conf"}
    }},
    {"wallpaper", {
      {"current", ""},
      {"hyprpaper_path", "/home/wzqrxzd/.config/hypr/hyprpaper.conf"},
      {"path_dir", "/home/wzqrxzd/Wallpapers/"}
    }},
    {"waybar", {
      {"color", ""},
      {"path", "/home/wzqrxzd/.config/waybar/style.css"}
    }},
    {"wofi", {
      {"color", ""},
      {"path", "/home/wzqrxzd/.config/wofi/style.css"}
    }}
  };

  saveConfig(defaultConfig);
}
