#include <catch/catch.hpp>

#include <string>
#include <cassert>
#include <vector>

std::vector<std::string> GetCombinations() {
  std::vector<std::string> values = {"0", "1", "2", "3", "4"};
  std::vector<std::string> output;

  output = values;

  for (size_t idx = 0 ; idx < 4 ; idx++) {
    auto current_output = output;
    output.clear();
    for (const auto& value : current_output) {
      for (const auto& v : values) {
        if (value.find(v) == std::string::npos)
          output.push_back(v + value);
      }
    }
  }

  return output;
}

TEST_CASE( "Day 7 solution", "[day7]" ) {
  auto combinations = GetCombinations();
  REQUIRE(combinations.size() == 120);
}
