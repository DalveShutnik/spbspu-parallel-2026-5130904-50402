#include "circle.hpp"
#include <istream>
#include <stdexcept>

samarin::circles_t samarin::readCircles(std::istream & in)
{
  circles_t circles;
  Circle circle{0, 0, 0};
  int ignored = 0;
  while (in >> circle.radius >> ignored >> circle.x >> circle.y) {
    circles.push_back(circle);
  }
  if (!in.eof()) {
    throw std::invalid_argument("Invalid circle parameters");
  }
  return circles;
}
