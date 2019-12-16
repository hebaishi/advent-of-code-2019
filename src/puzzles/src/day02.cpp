#include <io/io.hpp>
#include <emulator/intcode.hpp>

#include <catch/catch.hpp>

#include <fstream>
#include <cassert>
#include <iostream>

#include <boost/algorithm/string.hpp>

void ReplaceInstructions(std::vector<int>& instructions, int a, int b) {
  instructions[1] = a;
  instructions[2] = b;
}

decltype(auto) GetInstructions() {
  return io::ReadInstructions(CURRENT_DIR "/input2.txt");;
}

TEST_CASE( "Day 2 solution, part 1", "[day2]" ) {
  auto instructions = GetInstructions();
  ReplaceInstructions(instructions, 12, 2);
  emulator::Intcode processor;
  processor.ProcessInstructions(instructions);
  REQUIRE(instructions[0] == 3931283);
}

TEST_CASE( "Day 2 solution, part 2", "[day2]" ) {
  auto original_instructions = GetInstructions();
  bool found = false;
  for (size_t a = 0 ; a <= 99 ; a++) {
    for (size_t b = 0 ; b <= 99 ; b++) {
      auto current_instructions = original_instructions;
      emulator::Intcode processor;
      ReplaceInstructions(current_instructions, a, b);
      processor.ProcessInstructions(current_instructions);
      if (current_instructions[0] == 19690720)
        found = true;
    }
  }
  REQUIRE(found);
}
