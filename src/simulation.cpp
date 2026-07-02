#include "simulation.hpp"

#include <cassert>
#include <cmath>
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

const std::vector<State>& Simulation::states() const{
    return states_;
};

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

void Simulation::save_state()  {
  states_.push_back(State{x_absolute(), y_absolute(), hamiltonian()});
}

} // namespace lv