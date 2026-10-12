#include "circle.hpp"
#include <istream>
#include <stdexcept>

samarin::Circle::Circle(double radius, point_t center):
  radius_(radius),
  center_(center)
{
  if (radius <= 0.0) {
    throw std::invalid_argument("Circle radius must be positive");
  }
}

bool samarin::Circle::contains(point_t point) const noexcept
{
  const double dx = point.x - center_.x;
  const double dy = point.y - center_.y;
  return (dx * dx) + (dy * dy) <= radius_ * radius_;
}

samarin::frame_t samarin::Circle::getFrame() const noexcept
{
  return {{center_.x - radius_, center_.y - radius_}, {center_.x + radius_, center_.y + radius_}};
}

samarin::circles_t samarin::readCircles(std::istream & in)
{
  circles_t circles;
  int radius = 0;
  while (in >> radius) {
    int ignored = 0;
    int x = 0;
    int y = 0;
    if (!(in >> ignored >> x >> y)) {
      throw std::invalid_argument("Invalid circle parameters");
    }
    circles.push_back(Circle(radius, {static_cast< double >(x), static_cast< double >(y)}));
  }
  if (!in.eof()) {
    throw std::invalid_argument("Invalid circle parameters");
  }
  return circles;
}
