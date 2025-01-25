#ifndef CONFIG_MANAGER
#define CONFIG_MANAGER

#include "json.hpp"
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

class ConfigManager {
  public:
    ConfigManager(const std::filesystem::path& configPath);
    json loadConfig();
    void saveConfig(const json& config);
  private:
    fs::path filePath;
    void createDefaultConfig();
};

#endif
