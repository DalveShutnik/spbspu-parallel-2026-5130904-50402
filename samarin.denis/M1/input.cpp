#include "input.hpp"
#include <istream>
#include <memory>
#include <stdexcept>
#include "circle.hpp"

samarin::shapes_t samarin::readShapes(std::istream & in)
{
  shapes_t shapes;
  int radius = 0;
  while (in >> radius) {
    int ignored = 0;
    int x = 0;
    int y = 0;
    if (!(in >> ignored >> x >> y)) {
      throw std::invalid_argument("Invalid shape parameters");
    }
    const point_t center{static_cast< double >(x), static_cast< double >(y)};
    shapes.push_back(std::make_unique< Circle >(radius, center));
  }
  if (!in.eof()) {
    throw std::invalid_argument("Invalid shape parameters");
  }
  return shapes;
}
