#ifndef WALLPAPER_MANAGER_HXX
#define WALLPAPER_MANAGER_HXX

#include "json.hpp"
#include <filesystem>

using json = nlohmann::json;
namespace fs = std::filesystem;

class WallpaperManager {
  public:
    WallpaperManager(json* config);
    fs::path chooseWallpaper();
  private:
    std::vector<std::pair<std::filesystem::path, int>> sortedImages;
    int extractNumber(const fs::path& path);
    std::map<fs::path, int> getImagesMap(const fs::path& path);
};

#endif
