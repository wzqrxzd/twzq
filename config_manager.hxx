#ifndef CONFIG_MANAGER_HXX
#define CONFIG_MANAGER_HXX

#include "json.hpp"
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

class ConfigManager {
  public:
    ConfigManager(const fs::path& themePath);
    json getConfig();
    void saveTheme(const json& theme);
  private:
    void createDefaultConfig();
    const fs::path configPath = std::getenv("HOME") + std::string("/.config/twzq/config.json");
    const fs::path themePath;
    const json defaultConfig{
      {"wallpaper_dir", std::format("{}/Wallpapers/", std::getenv("HOME"))}
    };
};

#endif
