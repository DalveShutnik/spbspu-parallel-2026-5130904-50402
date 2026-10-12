#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <iosfwd>
#include <vector>

namespace samarin {
  struct Circle {
    int radius;
    int x;
    int y;
  };

  std::vector< Circle > readCircles(std::istream & in);
}

#endif
