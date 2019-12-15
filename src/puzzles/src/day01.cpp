#include <catch/catch.hpp>
#include <io/io.hpp>

#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>

size_t CalculateTotal(const std::vector<int>& values, bool increase_accuracy = false) {
  size_t total = 0;
  int current_total = 0;
  for (const auto& current : values) {
    current_total = static_cast<int>((current / 3) - 2);
    total += current_total;
    if (increase_accuracy) {
      while (current_total > 0) {
        current_total = static_cast<int>((current_total / 3) - 2);
        if (current_total > 0)
          total += current_total;
      }
    }
  }
  return total;
}

TEST_CASE( "Day 1 solution", "[day1]" ) {
  auto input_data = io::ReadIntegers(CURRENT_DIR "/input1.txt");

  auto total = CalculateTotal(input_data);
  REQUIRE(total == 3320226);

  total = CalculateTotal(input_data, true);
  REQUIRE(total == 4977473);
}
