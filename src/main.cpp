#include "parsing.hpp"
#include "simulation.hpp"

#include <exception>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

  try {
    lv::Program_options options = lv::parse_arguments(argc, argv);

    lv::Simulation simulation{options.opt_a, options.opt_b,  options.opt_c,
                              options.opt_d, options.opt_x0, options.opt_y0};

    simulation.run(options.opt_steps);

    if (options.opt_has_output_file) {
      std::ofstream file(options.opt_output_file);
      if (!file.is_open()) {
        std::cerr << "Error: cannot open output file '"
                  << options.opt_output_file << "'\n";
        return 1;
      }
      simulation.print(file, options.opt_precision);
    } else {
      simulation.print(std::cout, options.opt_precision);
    }

  } catch (const std::exception &error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  }
}