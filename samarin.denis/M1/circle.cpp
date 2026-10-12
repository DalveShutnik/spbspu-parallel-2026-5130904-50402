#include "circle.hpp"
#include <istream>
#include <stdexcept>

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
    circles.push_back(Circle{radius, x, y});
  }
  if (!in.eof()) {
    throw std::invalid_argument("Invalid circle parameters");
  }
  return circles;
}
