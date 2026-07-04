#include "parsing.hpp"

#include "doctest.h"

#include <stdexcept>
#include <string>
#include <vector>

namespace {

lv::Program_options parse(std::vector<std::string> &arguments) {
  std::vector<char *> argv;

  for (std::string &argument : arguments) {
    argv.push_back(argument.data());
  }

  return lv::parse_arguments(static_cast<int>(argv.size()), argv.data());
}

} // namespace

TEST_CASE("Parse required command line arguments") {
  std::vector<std::string> arguments = {"lotka_volterra", "1.0",  "0.1", "1.5",
                                        "0.075",          "10.0", "5.0", "100"};

  lv::Program_options options = parse(arguments);

  CHECK(options.a == doctest::Approx(1.0));
  CHECK(options.b == doctest::Approx(0.1));
  CHECK(options.c == doctest::Approx(1.5));
  CHECK(options.d == doctest::Approx(0.075));
  CHECK(options.x0 == doctest::Approx(10.0));
  CHECK(options.y0 == doctest::Approx(5.0));
  CHECK(options.steps == 100);
  CHECK(options.has_output_file == false);
  CHECK(options.precision == 2);
}

TEST_CASE("Parsing fails if one required argument is not numeric") {
  std::vector<std::string> valid_arguments = {
      "lotka_volterra", "1.0", "0.1", "1.5", "0.075", "10.0", "5.0", "100"};

  for (std::size_t i = 1; i <= 7; ++i) {
    std::vector<std::string> arguments = valid_arguments;
    arguments[i] = "abc";

    CAPTURE(i);

    CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
  }
}

TEST_CASE("Parsing fails if file name is a number") {
  std::vector<std::string> arguments = {"lotka_volterra",
                                        "1.0",
                                        "0.1",
                                        "1.5",
                                        "0.075",
                                        "10.0",
                                        "5.0",
                                        "100",
                                        "1.1",
                                        "--file_print",
                                        "1.5"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}

TEST_CASE("Parsing reads file name and precision after file name with "
          "--file_print option") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0",       "0.1", "1.5",
      "0.075",          "10.0",      "5.0", "100",
      "--file_print",   "file_name", "4"};

  lv::Program_options options = parse(arguments);

  CHECK(options.has_output_file == true);
  CHECK(options.generate_gnuplot_script == false);
  CHECK(options.output_file == "file_name");
  CHECK(options.gnuplot_script_file == "");
  CHECK(options.precision == 4);
  CHECK(options.precision_gnu == 2);
}

TEST_CASE("Parsing reads file name and precision after file name with "
          "--gnuplot option") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0",      "0.1", "1.5", "0.075", "10.0", "5.0", "100",
      "--gnuplot",      "gnu_file", "4"};

  lv::Program_options options = parse(arguments);

  CHECK(options.has_output_file == false);
  CHECK(options.generate_gnuplot_script == true);
  CHECK(options.output_file == "");
  CHECK(options.gnuplot_script_file == "gnu_file");
  CHECK(options.precision == 2);
  CHECK(options.precision_gnu == 4);
}

TEST_CASE("Parsing reads file name and precision after file name with "
          "--file_print and --gnuplot option") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0",       "0.1", "1.5",
      "0.075",          "10.0",      "5.0", "100",
      "--file_print",   "file_name", "6",   "--gnuplot",
      "gnu_file",       "4"};

  lv::Program_options options = parse(arguments);

  CHECK(options.has_output_file == true);
  CHECK(options.generate_gnuplot_script == true);
  CHECK(options.output_file == "file_name");
  CHECK(options.gnuplot_script_file == "gnu_file");
  CHECK(options.precision == 6);
  CHECK(options.precision_gnu == 4);
}

TEST_CASE("Parsing reads file name and precision after file name with "
          "--gnuplot and --file_print option") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0",      "0.1", "1.5",
      "0.075",          "10.0",     "5.0", "100",
      "--gnuplot",      "gnu_file", "4",   "--file_print",
      "file_name",      "6",
  };

  lv::Program_options options = parse(arguments);

  CHECK(options.has_output_file == true);
  CHECK(options.generate_gnuplot_script == true);
  CHECK(options.output_file == "file_name");
  CHECK(options.gnuplot_script_file == "gnu_file");
  CHECK(options.precision == 6);
  CHECK(options.precision_gnu == 4);
}

TEST_CASE("Parsing fails if precision is not an integer") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0", "0.1", "1.5", "0.075",
      "10.0",           "5.0", "100", "6.5"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}

TEST_CASE("Parsing fails if precision is negative") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0", "0.1", "1.5", "0.075",
      "10.0",           "5.0", "100", "-3"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}

TEST_CASE("Parsing fails if required arguments are missing") {
  std::vector<std::string> arguments = {"lotka_volterra", "1.0", "0.1", "1.5"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}

TEST_CASE("Parsing fails if there are too many arguments") {
  std::vector<std::string> arguments = {
      "lotka_volterra", "1.0",       "0.1",  "1.5",
      "0.075",          "10.0",      "5.0",  "100",
      "--file_print",   "file_name", "6",    "--gnuplot",
      "gnu_file",       "4",         "extra"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}

TEST_CASE("Parsing fails if gnuplot script file is missing") {
  std::vector<std::string> arguments{"lotka_volterra", "1",  "0.1", "1.5",
                                     "0.075",          "10", "5",   "100",
                                     "--gnuplot"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}

TEST_CASE("Parsing fails if gnuplot script file name is a number") {
  std::vector<std::string> arguments{"lotka_volterra", "1",  "0.1", "1.5",
                                     "0.075",          "10", "5",   "100",
                                     "--gnuplot",      "6"};

  CHECK_THROWS_AS(parse(arguments), std::invalid_argument);
}