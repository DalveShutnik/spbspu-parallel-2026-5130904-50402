#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
  long long parseInteger(const char * str)
  {
    const std::string text(str);
    std::size_t end = 0;
    const long long value = std::stoll(text, &end);
    if (end != text.size()) {
      throw std::invalid_argument("Invalid number: " + text);
    }
    return value;
  }
}

int main(int argc, char ** argv)
{
  if (argc != 3 && argc != 4) {
    std::cerr << "Usage: lab threads tries [seed]\n";
    return 1;
  }
  try {
    const long long threads = parseInteger(argv[1]);
    const long long tries = parseInteger(argv[2]);
    const long long seed = (argc == 4) ? parseInteger(argv[3]) : 0;
    if (tries <= 0) {
      throw std::invalid_argument("Tries must be positive");
    }
    if (threads < 0 || seed < 0) {
      throw std::invalid_argument("Threads and seed must not be negative");
    }
  } catch (const std::logic_error & e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
