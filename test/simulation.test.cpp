#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "simulation.hpp"

#include <stdexcept>

TEST_CASE("Simulation saves the initial state")
{
  lv::Simulation simulation(1.0, 2.0, 3.0, 4.0, 1.0, 1.0);

  REQUIRE(simulation.states().size() == 1);

  const lv::State state = simulation.states().front();

  CHECK(state.x == doctest::Approx(1.0));
  CHECK(state.y == doctest::Approx(1.0));
}

TEST_CASE("Simulation rejects non-positive parameters")
{
  CHECK_THROWS_AS(lv::Simulation(0.0, 2.0, 3.0, 4.0, 1.0, 1.0),
                  std::invalid_argument);

  CHECK_THROWS_AS(lv::Simulation(1.0, 0.0, 3.0, 4.0, 1.0, 1.0),
                  std::invalid_argument);

  CHECK_THROWS_AS(lv::Simulation(1.0, 2.0, 0.0, 4.0, 1.0, 1.0),
                  std::invalid_argument);

  CHECK_THROWS_AS(lv::Simulation(1.0, 2.0, 3.0, 0.0, 1.0, 1.0),
                  std::invalid_argument);
}