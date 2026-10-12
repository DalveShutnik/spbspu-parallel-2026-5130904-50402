#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <iosfwd>
#include <vector>

namespace samarin {
  struct point_t {
    double x;
    double y;
  };

  struct frame_t {
    point_t min;
    point_t max;
  };

  class Circle {
  public:
    Circle(double radius, point_t center);
    bool contains(point_t point) const noexcept;
    frame_t getFrame() const noexcept;

  private:
    double radius_;
    point_t center_;
  };

  using circles_t = std::vector< Circle >;

  circles_t readCircles(std::istream & in);
}

#endif
