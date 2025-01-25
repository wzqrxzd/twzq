#include "spletnya_color_analyzer.hxx"
#include <stdexcept>
#include "color.hxx"
#include <opencv2/opencv.hpp>

ImageColorAnalyzer::ImageColorAnalyzer(const std::filesystem::path& pathImg) : pathImg{pathImg}
{
  cv::Mat image = cv::imread(pathImg.string());
  
  if (image.empty())
    throw std::invalid_argument("image: image not exists");
}

/*std::array<Color, 5> ImageColorAnalyzer::analyze()*/
/*{*/
/*  cv::Mat image = cv::imread(pathImg.string());*/
/*  cv::Mat reshapedImage = image.reshape(1, image.rows * image.cols);*/
/*  reshapedImage.convertTo(reshapedImage, CV_32F);  // Convert to floating-point for k-means*/
/**/
/*  // Number of clusters (most frequent colors)*/
/*  int K = 5;*/
/**/
/*  // Perform k-means clustering*/
/*  cv::Mat labels, centers;*/
/*  cv::kmeans(reshapedImage, K, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.2), 3, cv::KMEANS_PP_CENTERS, centers);*/
/**/
/*  // Convert cluster centers (centroids) back to integers and store in Color objects*/
/*  std::vector<Color> colors;*/
/*  for (int i = 0; i < K; ++i) {*/
/*    cv::Vec3f colorVec = centers.at<cv::Vec3f>(i);*/
/*    colors.push_back(Color(static_cast<int>(colorVec[2]), static_cast<int>(colorVec[1]), static_cast<int>(colorVec[0])));*/
/*  }*/
/**/
/*  // Return the top K colors (centroids)*/
/*  std::array<Color, 5> resultColors;*/
/*  for (int i = 0; i < K; ++i) {*/
/*    resultColors[i] = colors[i];*/
/*  }*/
/**/
/*  return resultColors;*/
/*}*/

std::array<Color, 5> ImageColorAnalyzer::analyze()
{
  cv::Mat image = cv::imread(pathImg.string());
  cv::Mat reshapedImage = image.reshape(1, image.rows * image.cols);
  reshapedImage.convertTo(reshapedImage, CV_32F);  // Convert to floating-point for k-means

  // Number of clusters (most frequent colors)
  int K = 5;

  // Perform k-means clustering
  cv::Mat labels, centers;
  cv::kmeans(reshapedImage, K, labels, cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::MAX_ITER, 100, 0.2), 3, cv::KMEANS_PP_CENTERS, centers);

  // Convert cluster centers (centroids) back to integers and store in Color objects
  std::vector<Color> colors(K);
  
  // Use parallel_for_ for parallelizing this loop
  cv::parallel_for_(cv::Range(0, K), [&](const cv::Range& range) {
    for (int i = range.start; i < range.end; ++i) {
      cv::Vec3f colorVec = centers.at<cv::Vec3f>(i);
      uint8_t r = static_cast<uint8_t>(std::clamp(colorVec[2], 0.0f, 255.0f));  // red
      uint8_t g = static_cast<uint8_t>(std::clamp(colorVec[1], 0.0f, 255.0f));  // green
      uint8_t b = static_cast<uint8_t>(std::clamp(colorVec[0], 0.0f, 255.0f));  // blue
      colors[i] = Color(r,g,b);
    }
  });

  // Return the top K colors (centroids)
  std::array<Color, 5> resultColors;
  for (int i = 0; i < K; ++i) {
    resultColors[i] = colors[i];
  }

  return resultColors;
}
