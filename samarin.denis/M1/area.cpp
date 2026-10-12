#include "area.hpp"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <future>
#include <random>
#include <utility>
#include <vector>
#include "circle.hpp"

namespace {
  struct Frame {
    int left;
    int bottom;
    int right;
    int top;
  };

  using hits_t = std::pair< std::size_t, std::size_t >;

  Frame getFrame(const samarin::circles_t & circles)
  {
    const samarin::Circle & first = circles.front();
    Frame frame{first.x - first.radius, first.y - first.radius, first.x + first.radius, first.y + first.radius};
    for (const samarin::Circle & circle : circles) {
      frame.left = std::min(frame.left, circle.x - circle.radius);
      frame.bottom = std::min(frame.bottom, circle.y - circle.radius);
      frame.right = std::max(frame.right, circle.x + circle.radius);
      frame.top = std::max(frame.top, circle.y + circle.radius);
    }
    return frame;
  }

  bool contains(const samarin::Circle & circle, double x, double y)
  {
    const double dx = x - circle.x;
    const double dy = y - circle.y;
    const double radius = circle.radius;
    return (dx * dx) + (dy * dy) <= radius * radius;
  }

  hits_t countHits(const samarin::circles_t & circles, const Frame & frame, std::size_t tries, std::size_t seed)
  {
    std::default_random_engine generator(seed);
    std::uniform_real_distribution< double > xDistribution(frame.left, frame.right);
    std::uniform_real_distribution< double > yDistribution(frame.bottom, frame.top);
    hits_t hits{0, 0};
    for (std::size_t i = 0; i < tries; ++i) {
      const double x = xDistribution(generator);
      const double y = yDistribution(generator);
      const auto isInside = [x, y](const samarin::Circle & circle)
      {
        return contains(circle, x, y);
      };
      const std::size_t inside = std::count_if(circles.cbegin(), circles.cend(), isInside);
      if (inside > 0) {
        ++hits.first;
      }
      if (inside == circles.size()) {
        ++hits.second;
      }
    }
    return hits;
  }
}

samarin::areas_t samarin::getAreas(const circles_t & circles, std::size_t threads, std::size_t tries, std::size_t seed)
{
  if (circles.empty()) {
    return {0.0, 0.0};
  }
  const Frame frame = getFrame(circles);
  const std::size_t workers = std::max< std::size_t >(threads, 1);
  std::vector< std::future< hits_t > > futures;
  futures.reserve(workers);
  for (std::size_t i = 0; i < workers; ++i) {
    const std::size_t part = (tries / workers) + (i < tries % workers ? 1 : 0);
    futures.push_back(std::async(std::launch::async, countHits, std::cref(circles), frame, part, seed + i));
  }
  hits_t total{0, 0};
  for (std::future< hits_t > & future : futures) {
    const hits_t hits = future.get();
    total.first += hits.first;
    total.second += hits.second;
  }
  const double width = frame.right - frame.left;
  const double height = frame.top - frame.bottom;
  const double frameArea = width * height;
  const double count = tries;
  return {frameArea * total.first / count, frameArea * total.second / count};
}
