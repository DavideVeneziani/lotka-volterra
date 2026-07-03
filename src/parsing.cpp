#include "parsing.hpp"

#include <stdexcept>
#include <string>

namespace lv {

namespace {

bool is_integer(const std::string &text) {
  try {
    std::size_t index = 0;
    std::stoi(text, &index);
    return index == text.size();
  } catch (const std::exception &) {
    return false;
  }
}

bool is_double(const std::string &text) {
  try {
    std::size_t index = 0;
    std::stod(text, &index);
    return index == text.size();
  } catch (const std::exception &) {
    return false;
  }
}

} // namespace

Program_options parse_arguments(int argc, char *argv[]) {
  if (argc < 8) {
    throw std::invalid_argument{
        std::string{argv[0]} +
        " needs at least seven entry arguments: a b c d x0 y0 steps\n"};
  }
  if (argc > 10) {
    throw std::invalid_argument{std::string{argv[0]} +
                                " handles nine arguments max: a b c d x0 y0 "
                                "steps file_name data_precision\n"};
  }

  if (!is_double(argv[1]) || !is_double(argv[2]) || !is_double(argv[3]) ||
      !is_double(argv[4]) || !is_double(argv[5]) || !is_double(argv[6]) ||
      !is_integer(argv[7])) {
    throw std::invalid_argument{
        "arguments a, b, c, d, x0, y0 must be double; steps must be integer"};
  }

  Program_options options;

  options.opt_a = std::stod(argv[1]);
  options.opt_b = std::stod(argv[2]);
  options.opt_c = std::stod(argv[3]);
  options.opt_d = std::stod(argv[4]);
  options.opt_x0 = std::stod(argv[5]);
  options.opt_y0 = std::stod(argv[6]);
  options.opt_steps = std::stoi(argv[7]);

  if (argc == 9) {
    std::string arg8{argv[8]};

    if (is_integer(arg8)) {
      options.opt_precision = std::stoi(arg8);
    } else if (is_double(arg8)) {
      throw std::invalid_argument{"Precision must be an integer"};
    } else {
      options.opt_output_file = arg8;
      options.opt_has_output_file = true;
    }
  }

  if (argc == 10) {
    std::string arg8{argv[8]};
    std::string arg9{argv[9]};

    
    if (is_double(arg8)) {
      throw std::invalid_argument{"Output file name cannot be a number"};
    }

    if (!is_integer(arg9)) {
      throw std::invalid_argument{"Precision must be an integer"};
    }

    options.opt_output_file = arg8;
    options.opt_has_output_file = true;
    options.opt_precision = std::stoi(arg9);
  }

  if (options.opt_precision < 0) {
    throw std::invalid_argument{"Precision must be non-negative"};
  }

  if (options.opt_steps < 0) {
  throw std::invalid_argument{"Steps must be non-negative"};
  }

  return options;
}

} // namespace lv