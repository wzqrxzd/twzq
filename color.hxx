#ifndef COLOR_HXX
#define COLOR_HXX

#include <unordered_map>
#include <array>
#include <cstdint>

class Color {
  public:
    Color(int r, int g, int b);
    Color(std::array<int, 3> rgb);
    Color();
    Color(const Color& other);
    Color(Color&& other) noexcept;

    union {
      struct {
        uint8_t r,g,b,a; // r, g, b
      };
      uint32_t color_value;    // colorvalue
    };

    Color& operator=(const Color& other);
    Color& operator=(Color&& other) noexcept;
    bool operator==(const Color& other) const;
};

namespace std {
  template<>
  struct hash<Color> {
    size_t operator()(const Color& color) const {
      return color.color_value; 
    }
  };
}

#endif
