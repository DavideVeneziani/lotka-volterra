#ifndef LV_SIMULATION_HPP
#define LV_SIMULATION_HPP

#include <ostream>
#include <vector>

namespace lv {

struct State {
  double x;
  double y;
  double h;
};

class Simulation {
public:
  Simulation(double a, double b, double c, double d, double x0, double y0);

  const std::vector<State> &states() const;
  void evolve();
  void run(int steps);
  void print(std::ostream &output, int precision) const;
  void print_gnuplot_script(std::ostream& os, int precision) const;

private:
  double x_absolute() const;
  double y_absolute() const;
  double hamiltonian() const;
  void save_state();

  static constexpr double dt_{0.001};

  double a_;
  double b_;
  double c_;
  double d_;
  double x_rel_ = 0.0;
  double y_rel_ = 0.0;
  std::vector<State> states_;
};

} // namespace lv

#endif