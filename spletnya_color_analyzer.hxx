#ifndef SPLETNYA_PALITRA
#define SPLETNYA_PALITRA

#include <array>
#include "color.hxx"
#include <filesystem>
#include <opencv2/core/mat.hpp>

class ImageColorAnalyzer {
  public:
    ImageColorAnalyzer(std::filesystem::path& pathImg);
    std::array<Color, 5> analyze();
  private:
    std::filesystem::path pathImg;
};

#endif
