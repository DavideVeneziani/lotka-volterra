#include "simulation.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>

bool is_integer(const std::string& text)
{
  try {
    std::size_t index = 0;
    std::stoi(text, &index);
    return index == text.size();
  } catch (const std::exception&) {
    return false;
  }
}

bool is_double(const std::string& text)
{
  try {
    std::size_t index = 0;
    std::stod(text, &index);
    return index == text.size();
  } catch (const std::exception&) {
    return false;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 8) {
    std::cerr << argv[0]
              << " needs at least seven entry arguments: a b c d x0 y0 steps\n";
  }
  if (argc > 10) {
    std::cerr << argv[0]
              << " handles nine arguments max: a b c d x0 y0 steps file_name data_precision\n";
  }

  try {
    const double a = std::stod(argv[1]);
    const double b = std::stod(argv[2]);
    const double c = std::stod(argv[3]);
    const double d = std::stod(argv[4]);
    const double x0 = std::stod(argv[5]);
    const double y0 = std::stod(argv[6]);
    const int steps = std::stoi(argv[7]);

    std::string output_file = "";
    bool has_output_file = false;
    int precision = 2;

    if (argc == 9) {
    std::string arg8{argv[8]}

    if (is_integer(arg8)) {
     precision = std::stoi(arg8);
    } else {
     output_file = arg8;
      has_output_file = true;
    }
  }
    
    if (argc == 10) {
      output_file = argv[8];
      has_output_file = true;

      std::string arg9{argv[9]};

      if (is_double(arg8)) {
      std::cerr << "Error: output file name can't be a number\n";
      return 1;
      }

      if (!is_integer(arg9)) {
      std::cerr << "Error: precision must be an integer\n";
      return 1;
      }

      precision = std::stoi(arg9);
    } 
    
    if (a <= 0.0 || b <= 0.0 || c <= 0.0 || d <= 0.0 || x0 <= 0.0 ||
        y0 <= 0.0 || steps < 0) {
      std::cerr << "Error: all parameters must be positive, except steps which "
                   "must be non-negative\n";
      return 1;
    }

    lv::Simulation simulation{a, b, c, d, x0, y0};

    simulation.run(steps);
    simulation.print(std::cout, precision);

  } catch (const std::exception& error) {
    std::cerr << "Error: invalid input\n";
    std::cerr << argv[0] << " needs at least seven entry arguments written in figures: a b c d x0 y0 steps\n";
    return 1;
  }
}