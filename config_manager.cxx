#include "config_manager.hxx"
#include <spdlog/spdlog.h>
#include <fstream>

ConfigManager::ConfigManager(const fs::path& themePath) : themePath{themePath}
{
  if (fs::exists(themePath))
  {
    spdlog::warn("Rewrited theme: {}", themePath.string());
    fs::remove(themePath);
  }
  
  if (!fs::exists(configPath))
  {
    spdlog::warn("Default config been created.");
    createDefaultConfig();
  }
}

json ConfigManager::getConfig()
{
  std::ifstream configFile(configPath);
  if (!configFile.is_open())
  {
    spdlog::error("Config file {}, failed to load", configPath.string());
    throw std::runtime_error(std::format("Config file {}, failed to load", configPath.string()));
  }

  json config;
  try {
    configFile >> config;
  } catch (const json::exception& e)
  {
    spdlog::error("Error reading JSON: {}", e.what());
    throw std::runtime_error(std::format("Error reading JSON: {}", e.what()));
  }

  return config;
}

void ConfigManager::saveTheme(const json& theme)
{
  if (!fs::exists(themePath))
    fs::remove(themePath);

  if (!fs::exists(themePath.parent_path()))
   fs::create_directory(themePath.parent_path()); 
  
  std::ofstream themeFile(themePath);
  if (!themeFile.is_open())
  {
    spdlog::error("Theme file {}, failed to load", themePath.string());
    throw std::runtime_error(std::format("Theme file {}, failed to load", themePath.string()));
  }

  themeFile << theme.dump(4);
  themeFile.close();
}

void ConfigManager::createDefaultConfig()
{ 
  if (!fs::exists(configPath.parent_path()))
   fs::create_directory(configPath.parent_path()); 

  std::ofstream configFile(configPath);
  if (!configFile.is_open())
  {
    spdlog::error("Config file {}, failed to load", configPath.string());
    throw std::runtime_error(std::format("Config file {}, failed to load", configPath.string()));
  }

  configFile << defaultConfig.dump(4);
  configFile.close();
}
