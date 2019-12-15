#include <io/io.hpp>

#include <catch/catch.hpp>

#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>

void ReplaceInstructions(std::vector<int>& instructions, int a, int b) {
  instructions[1] = a;
  instructions[2] = b;
}

int ProcessInstructions2(std::vector<int>& instructions) {
  for (size_t idx = 0 ; idx < instructions.size() ; idx += 4) {
    if (instructions[idx] == 99)
      break;
    switch (instructions[idx]) {
    case 1:
      instructions[ instructions[idx+3] ] = instructions[ instructions[idx+1] ] + instructions[ instructions[idx+2] ];
      break;
    case 2:
      instructions[ instructions[idx+3] ] = instructions[ instructions[idx+1] ] * instructions[ instructions[idx+2] ];
      break;
    default:
      throw std::runtime_error("Unknown opcode encountered");
      break;
    }
  }
  return instructions[0];
}

TEST_CASE( "Day 2 part 2 solution", "[day2]" ) {
  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input2.txt");
  for (size_t a = 0 ; a <= 99 ; a++) {
    for (size_t b = 0 ; b <= 99 ; b++) {
      auto new_instructions = original_instructions;
      ReplaceInstructions(new_instructions, a, b);
      if (ProcessInstructions2(new_instructions) == 19690720) {
        REQUIRE(((a * 100) + b) == 6979);
      }
    }
  }
}
