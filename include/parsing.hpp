#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>

namespace lv {

struct Program_options {
  double opt_a = 0.0;
  double opt_b = 0.0;
  double opt_c = 0.0;
  double opt_d = 0.0;
  double opt_x0 = 0.0;
  double opt_y0 = 0.0;
  int opt_steps = 0;

  std::string opt_output_file = "";
  bool opt_has_output_file = false;
  int opt_precision = 2;
};

Program_options parse_arguments(int argc, char *argv[]);

} // namespace lv

#endif