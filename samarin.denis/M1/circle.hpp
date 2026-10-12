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

  using circles_t = std::vector< Circle >;

  circles_t readCircles(std::istream & in);
}

#endif
