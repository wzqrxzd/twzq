#include "color.hxx"

Color::Color() : color_value{0} {}

Color::Color(int r, int g, int b, int a)
{
  this->r = r;
  this->g = g;
  this->b = b;
  this->a = a;
}

Color& Color::operator=(const Color& other)
{
  if (this != &other)
  {
    color_value = other.color_value;
  }
  return *this;
}

Color& Color::operator=(Color&& other) noexcept
{
  if (this != &other)
  {
    color_value = other.color_value;
    other.color_value = 0;
  }
  return *this;
}

Color::Color(const Color& other) : color_value{other.color_value} {}
Color::Color(Color && other) noexcept : color_value{other.color_value} 
{
  other.color_value = 0;
}


bool Color::operator==(const Color& other) const
{
  return color_value == other.color_value;
}
