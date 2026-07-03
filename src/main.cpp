#include "parsing.hpp"
#include "simulation.hpp"

#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

  try {
    lv::Program_options options = lv::parse_arguments(argc, argv);

    lv::Simulation simulation{options.a, options.b,  options.c,
                              options.d, options.x0, options.y0};

    simulation.run(options.steps);

    if (options.has_output_file) {
      std::ofstream file(options.output_file);
      if (!file.is_open()) {
        std::cerr << "Error: cannot open output file '" << options.output_file
                  << "'\n";
        return 1;
      }
      simulation.print(file, options.precision);
    }
    if (options.generate_gnuplot_script) {
      std::ofstream gnuplot_file(options.gnuplot_script_file);

      if (!gnuplot_file.is_open()) {
        std::cerr << "Error: cannot open gnuplot script file '"
                  << options.gnuplot_script_file << "'\n";
        return 1;
      }

      simulation.print_gnuplot_script(gnuplot_file, options.precision);
    }
    if (!options.has_output_file && !options.generate_gnuplot_script) {
      simulation.print(std::cout, options.precision);
    }

  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}