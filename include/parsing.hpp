#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>

namespace lv {

struct Program_options {
  double a = 0.0;
  double b = 0.0;
  double c = 0.0;
  double d = 0.0;
  double x0 = 0.0;
  double y0 = 0.0;
  int steps = 0;

  std::string output_file = "";
  bool has_output_file = false;

  std::string gnuplot_script_file = "";
  bool generate_gnuplot_script = false;

  int precision = 2;
};

Program_options parse_arguments(int argc, char *argv[]);

} // namespace lv

#endif