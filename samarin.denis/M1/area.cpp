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
  using hits_t = std::pair< std::size_t, std::size_t >;

  samarin::frame_t getFrame(const samarin::circles_t & circles)
  {
    samarin::frame_t frame = circles.front().getFrame();
    for (const samarin::Circle & circle : circles) {
      const samarin::frame_t circleFrame = circle.getFrame();
      frame.min.x = std::min(frame.min.x, circleFrame.min.x);
      frame.min.y = std::min(frame.min.y, circleFrame.min.y);
      frame.max.x = std::max(frame.max.x, circleFrame.max.x);
      frame.max.y = std::max(frame.max.y, circleFrame.max.y);
    }
    return frame;
  }

  hits_t countHits(const samarin::circles_t & circles, samarin::frame_t frame, std::size_t tries, std::size_t seed)
  {
    std::minstd_rand0 generator(seed);
    std::uniform_real_distribution< double > xDistribution(frame.min.x, frame.max.x);
    std::uniform_real_distribution< double > yDistribution(frame.min.y, frame.max.y);
    hits_t hits{0, 0};
    for (std::size_t i = 0; i < tries; ++i) {
      const samarin::point_t point{xDistribution(generator), yDistribution(generator)};
      const auto isInside = [point](const samarin::Circle & circle)
      {
        return circle.contains(point);
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
  const frame_t frame = getFrame(circles);
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
  const double width = frame.max.x - frame.min.x;
  const double height = frame.max.y - frame.min.y;
  const double frameArea = width * height;
  const double count = tries;
  return {frameArea * total.first / count, frameArea * total.second / count};
}
