#ifndef COLOR_HXX
#define COLOR_HXX

#include <unordered_map>
#include <cstdint>

class Color {
  public:
    Color(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a = 255);
    Color();
    Color(const Color& other);
    Color(Color&& other) noexcept;

    union {
      struct // DO NOT SWAP ORDER (BIT ORDER PROBLEM)
      {
        uint8_t   a;
        uint8_t   b;
        uint8_t   g;
        uint8_t   r;
      };
      uint32_t color_value;
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
