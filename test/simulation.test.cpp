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

TEST_CASE("Simulation prints results")
{
  lv::Simulation simulation{1.0, 0.1, 1.5, 0.075, 10.0, 5.0};

  simulation.run(3);

  std::ostringstream os;
  simulation.print(os, 2);

  CHECK(!os.str().empty());
}

TEST_CASE("Simulation print uses selected precision")
{
  lv::Simulation simulation{1.0, 0.1, 1.5, 0.075, 10.0, 5.0};

  simulation.run(1);

  std::ostringstream output1;
  std::ostringstream output2;
  simulation.print(output1, 2);
  simulation.print(output2, 6);
  std::string printed_text1 = output1.str();
  std::string printed_text2 = output2.str();

  CHECK(printed_text1.find("10.00") != std::string::npos);
  CHECK(printed_text2.find("10.000000") != std::string::npos);
}