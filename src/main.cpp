#include "simulation.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc != 8) {
    std::cerr << argv[0]
              << " needs seven entry arguments: a b c d x0 y0 steps\n";
  }

  try {
    const double a = std::stod(argv[1]);
    const double b = std::stod(argv[2]);
    const double c = std::stod(argv[3]);
    const double d = std::stod(argv[4]);
    const double x0 = std::stod(argv[5]);
    const double y0 = std::stod(argv[6]);
    const int steps = std::stoi(argv[7]);

    if (a <= 0.0 || b <= 0.0 || c <= 0.0 || d <= 0.0 || x0 <= 0.0 ||
        y0 <= 0.0 || steps < 0) {
      std::cerr << "Error: all parameters must be positive, except steps which "
                   "must be non-negative\n";
      return 1;
    }

    lv::Simulation simulation{a, b, c, d, x0, y0};

    simulation.run(steps);
    simulation.print(std::cout, 2);

  } catch (const std::exception& error) {
    std::cerr << "Error: invalid input\n";
    std::cerr << argv[0] << " needs eight entry arguments written in figures: a b c d x0 y0 steps\n";
    return 1;
  }
}