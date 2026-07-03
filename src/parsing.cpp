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

  options.a = std::stod(argv[1]);
  options.b = std::stod(argv[2]);
  options.c = std::stod(argv[3]);
  options.d = std::stod(argv[4]);
  options.x0 = std::stod(argv[5]);
  options.y0 = std::stod(argv[6]);
  options.steps = std::stoi(argv[7]);

  if (argc == 9) {
    std::string arg8{argv[8]};

    if (is_integer(arg8)) {
      options.precision = std::stoi(arg8);
    } else if (is_double(arg8)) {
      throw std::invalid_argument{"Precision must be an integer"};
    } else {
      options.output_file = arg8;
      options.has_output_file = true;
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

    options.output_file = arg8;
    options.has_output_file = true;
    options.precision = std::stoi(arg9);
  }

  if (options.precision < 0) {
    throw std::invalid_argument{"Precision must be non-negative"};
  }

  if (options.steps < 0) {
    throw std::invalid_argument{"Steps must be non-negative"};
  }

  return options;
}

} // namespace lv