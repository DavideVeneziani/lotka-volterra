#include "simulation.hpp"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>

namespace lv {

Simulation::Simulation(double a, double b, double c, double d, double x0,
                       double y0)
    : a_{a}, b_{b}, c_{c}, d_{d}, x_rel_{}, y_rel_{} {
  if (a < 0.0 || b < 0.0 || c < 0.0 || d < 0.0) {
    throw std::invalid_argument{"Parameters must be non negative"};
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

const std::vector<State>& Simulation::states() const { return states_; }

void Simulation::evolve() {
  assert(x_rel_ > 0.0);
  assert(y_rel_ > 0.0);

  const double x_old = x_rel_;
  const double y_old = y_rel_;

  y_rel_ = y_old + d_ * (x_old - 1.0) * y_old * dt_;
  x_rel_ = x_old + a_ * (1.0 - y_rel_) * x_old * dt_;

  assert(x_rel_ > 0.0);
  assert(y_rel_ > 0.0);

  save_state();
}

void Simulation::run(int steps) {
  if (steps < 0) {
    throw std::invalid_argument{"steps must be non negative"};
  }

  assert(steps >= 0);

  for (int i = 0; i < steps; ++i) {
    Simulation::evolve();
  }
}

void Simulation::print(std::ostream& output, int precision) const {
  assert(precision >= 0);
  assert(!states_.empty());

  output << std::setw(8) << "step" << std::setw(18) << "x" << std::setw(18)
         << "y" << std::setw(18) << "h" << std::setw(18) << "h '%' error"
         << '\n';

  output << std::fixed << std::setprecision(precision);

  for (std::size_t i = 0; i < states_.size(); ++i) {
    output << std::setw(8) << i << std::setw(18) << states_[i].x
           << std::setw(18) << states_[i].y << std::setw(18) << states_[i].h
           << std::setw(17) << h_percentage_error(i) << '\n';
  }
}

void Simulation::print_gnuplot_script(std::ostream& os, int precision) const {
  os << "$data << EOD\n";
  print(os, precision);
  os << "EOD\n\n";

  os << "set terminal qt size 1600,800 title 'Lotka Volterra Simulation'\n";
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

  os << "set title 'First integral H and percentage error'\n";
  os << "set xlabel 'step'\n";
  os << "set ylabel 'H'\n";
  os << "set y2label 'absolute percentage error (%)'\n";
  os << "set ytics nomirror\n";
  os << "set y2tics\n";
  os << "set y2range [0:10]\n";
  os << "plot $data every ::1 using 1:4 axes x1y1 with lines title 'H', \\\n";
  os << "     $data every ::1 using 1:5 axes x1y2 with lines title "
        "'H percentage error'\n";
  os << "pause -1\n";
  os << "pause -1\n";
}

double Simulation::x_absolute() const {
  assert(c_ >= 0.0);
  assert(x_rel_ > 0.0);

  return x_rel_ * d_ / c_;
}

double Simulation::y_absolute() const {
  assert(b_ >= 0.0);
  assert(y_rel_ > 0.0);

  return y_rel_ * a_ / b_;
}

double Simulation::hamiltonian() const {
  const double x = x_absolute();
  const double y = y_absolute();

  assert(x > 0.0);
  assert(y > 0.0);

  return -d_ * std::log(x) + c_ * x + b_ * y - a_ * std::log(y);
}

double Simulation::h_percentage_error(std::size_t index) const {
  assert(!states_.empty());
  assert(index < states_.size());

  const double initial_h = states_[0].h;

  if (initial_h == 0.0) {
    if (states_[index].h - initial_h == 0) {
      return 0;
    } else {
      return std::numeric_limits<double>::quiet_NaN();
    }
  } else {
    return 100.0 * std::abs(states_[index].h - initial_h) / std::abs(initial_h);
  }
}

void Simulation::save_state() {
  const double x = x_absolute();
  const double y = y_absolute();
  const double h = hamiltonian();

  assert(x > 0.0);
  assert(y > 0.0);

  states_.push_back(State{x, y, h});
}

} // namespace lv