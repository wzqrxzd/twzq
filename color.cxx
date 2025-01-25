#include "color.hxx"
#include <stdexcept>

Color::Color() : r_{0}, g_{0}, b_{0} {}

Color::Color(int r, int g, int b)
{
  if (r > 255 || r < 0)
    throw std::out_of_range("out of range red value");
  if (g > 255 || g < 0)
    throw std::out_of_range("out of range green value");
  if (b > 255 || b < 0)
    throw std::out_of_range("out of range blue value");

  this->r_ = r;
  this->g_ = g;
  this->b_ = b;
}

Color& Color::operator=(const Color& other)
{
  if (this != &other)
  {
    r_ = other.r_;
    g_ = other.g_;
    b_ = other.b_;
  }
  return *this;
}

Color& Color::operator=(Color&& other) noexcept
{
  if (this != &other)
  {
    r_ = other.r_;
    g_ = other.g_;
    b_ = other.b_;

    other.r_ = 0;
    other.g_ = 0;
    other.b_ = 0;
  }
  return *this;
}

Color::Color(const Color& other) : r_{other.r_}, g_{other.g_}, b_{other.b_} {}
Color::Color(Color && other) noexcept : r_{other.r_}, g_{other.g_}, b_{other.b_}
{
  other.r_ = 0;
  other.g_ = 0;
  other.b_ = 0;
}

void Color::set_color(int r, int g, int b)
{
  if (r > 255 || r < 0)
    throw std::out_of_range("out of range red value");
  if (g > 255 || g < 0)
    throw std::out_of_range("out of range green value");
  if (b > 255 || b < 0)
    throw std::out_of_range("out of range blue value");

  this->r_ = r;
  this->b_ = b;
  this->g_ = g;
}

bool Color::operator==(const Color& other) const
{
  return r == other.r && g == other.g && b == other.b;
}
