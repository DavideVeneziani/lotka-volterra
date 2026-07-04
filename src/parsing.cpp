#include "parsing.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace lv {

namespace {
bool is_integer(const std::string& text) {
  try {
    std::size_t index = 0;
    std::stoi(text, &index);
    return index == text.size();
  } catch (const std::exception &) {
    return false;
  }
}

bool is_double(const std::string& text) {
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
  Program_options options;

  if (argc < 8) {
    throw std::invalid_argument{std::string{argv[0]} +
                                " expects 7 arguments: a b c d x0 y0 steps\n"};
  }

  if (!is_double(argv[1]) || !is_double(argv[2]) || !is_double(argv[3]) ||
      !is_double(argv[4]) || !is_double(argv[5]) || !is_double(argv[6]) ||
      !is_integer(argv[7])) {
    throw std::invalid_argument{
        "arguments a, b, c, d, x0, y0 must be double; steps must be integer"};
  }

  options.a = std::stod(argv[1]);
  options.b = std::stod(argv[2]);
  options.c = std::stod(argv[3]);
  options.d = std::stod(argv[4]);
  options.x0 = std::stod(argv[5]);
  options.y0 = std::stod(argv[6]);
  options.steps = std::stoi(argv[7]);

  if (options.steps < 0) {
    throw std::invalid_argument{"Steps must be non-negative"};
  }

  for (int i = 8; i < argc;) {
    const std::string option{argv[i]};

    if (option == "--print_file") {
      throw std::invalid_argument{"Did you mean --file_print?"};
    }

    if (option == "--file_print") {

      if (i + 1 >= argc) {
        throw std::invalid_argument{
            "--file_print: expecting  destination file"};
      }
      std::string file_name{argv[i + 1]};

      if (file_name == "--gnuplot" || file_name == "--file_print") {
        throw std::invalid_argument{
            "--file_print: expecting  destination file"};
      }

      if (is_double(file_name)) {
        throw std::invalid_argument{"file name cannot be a number"};
      }

      options.has_output_file = true;
      options.output_file = file_name;
      options.precision = 2;

      i += 2;

      if (i < argc && is_integer(argv[i])) {
        options.precision = std::stoi(argv[i]);

        if (options.precision < 0) {
          throw std::invalid_argument{"Precision must be non-negative"};
        }

        ++i;
      }

    } else if (option == "--gnuplot") {
      if (i + 1 >= argc) {
        throw std::invalid_argument{"'--gnuplot': expecting  destination file"};
      }

      std::string script_file{argv[i + 1]};

      if (script_file == "--gnuplot" || script_file == "--file_print") {
        throw std::invalid_argument{
            "'--gnuplot' must be followed by a destination file"};
      }

      if (is_double(script_file)) {
        throw std::invalid_argument{
            "gnuplot script file name cannot be a number"};
      }

      options.generate_gnuplot_script = true;
      options.gnuplot_script_file = script_file;
      options.precision_gnu = 2;

      i += 2;

      if (i < argc && is_integer(argv[i])) {
        options.precision_gnu = std::stoi(argv[i]);

        if (options.precision_gnu < 0) {
          throw std::invalid_argument{"Precision must be non-negative"};
        }

        ++i;
      }

    } else {
      throw std::invalid_argument{"unknown option: " + option};
    }
  }

  return options;
}

} // namespace lv