#ifndef COLOR_HXX
#define COLOR_HXX

#include <unordered_map>
#include <array>

class Color {
  public:
    Color(int r, int g, int b);
    Color(std::array<int, 3> rgb);
    Color();
    Color(const Color& other);
    Color(Color&& other) noexcept;

    const int& r = r_;
    const int& g = g_;
    const int& b = b_;

    Color& operator=(const Color& other);
    Color& operator=(Color&& other) noexcept;
    bool operator==(const Color& other) const;

    void set_color(int r, int g, int b);
  private:
    int r_;
    int g_;
    int b_;
};

namespace std {
  template<>
  struct hash<Color> {
    size_t operator()(const Color& color) const {
      size_t h1 = std::hash<int>{}(color.r);
      size_t h2 = std::hash<int>{}(color.g);
      size_t h3 = std::hash<int>{}(color.b);
      return h1 ^ (h2 << 1) ^ (h3 << 2); 
    }
  };
}

#endif
