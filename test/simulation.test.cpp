#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "simulation.hpp"

#include <stdexcept>

TEST_CASE("Simulation saves the initial state") {
  lv::Simulation simulation(1.0, 2.0, 3.0, 4.0, 1.0, 1.0);

  REQUIRE(simulation.states().size() == 1);

  const lv::State state = simulation.states().front();

  CHECK(state.x == doctest::Approx(1.0));
  CHECK(state.y == doctest::Approx(1.0));
}

TEST_CASE("Simulation rejects non-positive parameters") {
  CHECK_THROWS_AS(lv::Simulation(0.0, 2.0, 3.0, 4.0, 1.0, 1.0),
                  std::invalid_argument);

  CHECK_THROWS_AS(lv::Simulation(1.0, 0.0, 3.0, 4.0, 1.0, 1.0),
                  std::invalid_argument);

  CHECK_THROWS_AS(lv::Simulation(1.0, 2.0, 0.0, 4.0, 1.0, 1.0),
                  std::invalid_argument);

  CHECK_THROWS_AS(lv::Simulation(1.0, 2.0, 3.0, 0.0, 1.0, 1.0),
                  std::invalid_argument);
}

TEST_CASE("Simulation prints results") {
  lv::Simulation simulation{1.0, 0.1, 1.5, 0.075, 10.0, 5.0};

  simulation.run(3);

  std::ostringstream os;
  simulation.print(os, 2);

  CHECK(!os.str().empty());
}

TEST_CASE("Simulation print uses selected precision") {
  lv::Simulation simulation{1.0, 0.1, 1.5, 0.075, 10.0, 5.0};

  simulation.run(1);

  std::stringstream output1;
  std::stringstream output2;
  simulation.print(output1, 2);
  simulation.print(output2, 6);
  std::string printed_text1 = output1.str();
  std::string printed_text2 = output2.str();

  CHECK(printed_text1.find("10.00") != std::string::npos);
  CHECK(printed_text2.find("10.000000") != std::string::npos);
}

TEST_CASE("Simulation run stores the correct number of states")
{
  lv::Simulation simulation{1.0, 0.1, 1.5, 0.075, 10.0, 5.0};

  simulation.run(10);

  CHECK(simulation.states().size() == 11);
}

TEST_CASE("Simulation run with zero steps keeps only initial state")
{
  lv::Simulation simulation{1.0, 0.1, 1.5, 0.075, 10.0, 5.0};

  simulation.run(0);

  CHECK(simulation.states().size() == 1);
  CHECK(simulation.states()[0].x == doctest::Approx(10.0));
  CHECK(simulation.states()[0].y == doctest::Approx(5.0));
}

TEST_CASE("Initial H percentage error is zero")
{
  lv::Simulation simulation{1.0, 0.1, 0.075, 1.5, 18.0, 8.0};

  std::stringstream stream;
  simulation.print(stream, 8);

  std::string header;
  std::getline(stream, header);

  int step = 0;
  double x = 0.0;
  double y = 0.0;
  double h = 0.0;
  double h_percentage_error = 0.0;

  stream >> step >> x >> y >> h >> h_percentage_error;

  CHECK(step == 0);
  CHECK(h_percentage_error == doctest::Approx(0.0));
}

TEST_CASE("Print writes H before H percentage error")
{
  lv::Simulation simulation{1.0, 0.1, 0.075, 1.5, 18.0, 8.0};

  const auto states = simulation.states();
  const double expected_h = states[0].h;

  std::stringstream stream;
  simulation.print(stream, 8);

  std::string header;
  std::getline(stream, header);
  int step = 0;
  double x = 0.0;
  double y = 0.0;
  double printed_h = 0.0;
  double h_percentage_error = 0.0;

  stream >> step >> x >> y >> printed_h >> h_percentage_error;

  CHECK(printed_h == doctest::Approx(expected_h));
  CHECK(h_percentage_error == doctest::Approx(0.0));
}

TEST_CASE("H percentage error is computed and printed correctly after one step")
{
  lv::Simulation simulation{1.0, 0.1, 0.075, 1.5, 18.0, 8.0};

  simulation.run(1);

  const auto& states = simulation.states();

  const double h0 = states[0].h;
  const double h1 = states[1].h;

  const double expected_error = 100. * std::abs(h1 - h0) / std::abs(h0);

  std::stringstream stream;
  simulation.print(stream, 8);

  std::string header;
  std::getline(stream, header);

  int step = 0;
  double x = 0.;
  double y = 0.;
  double h = 0.;
  double h_percentage_error = 0.;


  stream >> step >> x >> y >> h >> h_percentage_error;

  stream >> step >> x >> y >> h >> h_percentage_error;

  CHECK(step == 1);
  CHECK(h == doctest::Approx(h1));
  CHECK(h_percentage_error == doctest::Approx(expected_error));
}