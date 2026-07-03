#include "simulation.hpp"

#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <stdexcept>

namespace lv {

Simulation::Simulation(double a, double b, double c, double d, double x0,
                       double y0)
    : a_{a}, b_{b}, c_{c}, d_{d}, x_rel_{}, y_rel_{} {
  if (a <= 0.0 || b <= 0.0 || c <= 0.0 || d <= 0.0) {
    throw std::invalid_argument{"Parameters must be positive"};
  }

  if (x0 <= 0.0 || y0 <= 0.0) {
    throw std::invalid_argument{"Initial populations must be positive"};
  }

  x_rel_ = x0 * c_ / d_;
  y_rel_ = y0 * b_ / a_;

  assert(x_rel_ > 0.0);
  assert(y_rel_ > 0.0);

  save_state();
}

const std::vector<State> &Simulation::states() const { return states_; };

void Simulation::evolve() {
  assert(x_rel_ > 0.);
  assert(y_rel_ > 0.);

  const double x_old = x_rel_;
  const double y_old = y_rel_;

  y_rel_ = y_old + d_ * (x_old - 1.0) * y_old * dt_;
  x_rel_ = x_old + a_ * (1.0 - y_rel_) * x_old * dt_;

  save_state();
}

void Simulation::run(int steps) {
  if (steps < 0) {
    throw std::invalid_argument{"Il numero di passi deve essere positivo"};
  }

  for (int i = 0; i < steps; ++i) {
    Simulation::evolve();
  }
};

void Simulation::print(std::ostream &output, int precision) const {
  output << std::setw(8) << "step" << std::setw(18) << "x" << std::setw(18)
         << "y" << std::setw(18) << "h" << '\n';

  output << std::fixed << std::setprecision(precision);

  for (std::size_t i = 0; i < states_.size(); ++i) {
    output << std::setw(8) << i << std::setw(18) << states_[i].x
           << std::setw(18) << states_[i].y << std::setw(18) << states_[i].h
           << '\n';
  }
}

void Simulation::print_gnuplot_script(std::ostream& os, int precision) const
{
  os << "$data << EOD\n";
  print(os, precision);
  os << "EOD\n\n";

  os << "set grid\n";
  os << "set key outside\n\n";

  os << "set title 'Preys and predators'\n";
  os << "set xlabel 'step'\n";
  os << "set ylabel 'population'\n";
  os << "plot $data every ::1 using 1:2 with lines title 'preys', \\\n";
  os << "     $data every ::1 using 1:3 with lines title 'predators'\n";
  os << "pause -1\n\n";

  os << "set title 'Phase portrait'\n";
  os << "set xlabel 'preys x'\n";
  os << "set ylabel 'predators y'\n";
  os << "plot $data every ::1 using 2:3 with lines title 'orbit'\n";
  os << "pause -1\n\n";

  os << "set title 'First integral H'\n";
  os << "set xlabel 'step'\n";
  os << "set ylabel 'H'\n";
  os << "plot $data every ::1 using 1:4 with lines title 'H'\n";
  os << "pause -1\n";
}

double Simulation::x_absolute() const {
  assert(c_ > 0.0);

  return x_rel_ * d_ / c_;
}

double Simulation::y_absolute() const {
  assert(b_ > 0.0);

  return y_rel_ * a_ / b_;
}

double Simulation::hamiltonian() const {
  const double x = x_absolute();
  const double y = y_absolute();

  assert(x > 0.0);
  assert(y > 0.0);

  return -d_ * std::log(x) + c_ * x + b_ * y - a_ * std::log(y);
}

void Simulation::save_state() {
  states_.push_back(State{x_absolute(), y_absolute(), hamiltonian()});
}

} // namespace lv