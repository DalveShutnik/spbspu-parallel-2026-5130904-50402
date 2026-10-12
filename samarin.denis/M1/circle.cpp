#include "circle.hpp"
#include <istream>
#include <stdexcept>
#include <vector>

std::vector< samarin::Circle > samarin::readCircles(std::istream & in)
{
  std::vector< Circle > circles;
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
