#include "wallpaper_manager.hxx"
#include <iostream>
#include <regex>
#include <spdlog/spdlog.h>

WallpaperManager::WallpaperManager(json* config)
{
  if (!config->contains("wallpaper"))
  {
    spdlog::error("wallpaper: wallpaper cfg not exists");
    throw std::runtime_error("wallpaper: wallpaper cfg not exists");
  }

  if (!(*config)["wallpaper"].contains("path_dir"))
  {
    spdlog::error("wallpaper: path_dir not exists");
    throw std::runtime_error("wallpaper: path_dir not exists");
  }

  std::map<fs::path, int> wallpaperImages(getImagesMap((*config)["wallpaper"]["path_dir"]));
  std::vector<std::pair<fs::path, int>> images(wallpaperImages.begin(), wallpaperImages.end());
  std::sort(images.begin(), images.end(), [](const std::pair<std::filesystem::path, int>& a, const std::pair<std::filesystem::path, int>& b) {
    return a.second < b.second;
  });

  sortedImages = images;
}

fs::path WallpaperManager::chooseWallpaper()
{
  for (const auto& image : sortedImages)
    std::cout << "Image: " << image.first.string() << std::endl;

  fs::path imgPath;
  int index;

  std::cout << "Enter index: ";
  std::cin >> index;

  for (const auto& pair : sortedImages)
  {
    if (pair.second == index)
      imgPath = pair.first;
  }

  return imgPath;
}


std::map<fs::path, int> WallpaperManager::getImagesMap(const fs::path& path)
{
  std::map<fs::path, int> wallpaperImages;
  for (const auto& entry : fs::directory_iterator(path))
  {
    if (fs::is_regular_file(entry))
      wallpaperImages[entry.path()] = extractNumber(entry.path());
  }

  return wallpaperImages;
}

int WallpaperManager::extractNumber(const fs::path& path)
{
  std::string filename = path.filename().string();
  std::regex re(R"(walp(\d+)\.\w+)");

  std::smatch match;
  if (std::regex_search(filename, match, re))
    return std::stoi(match[1].str());

  return -1;
}

