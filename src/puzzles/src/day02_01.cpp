#include <catch/catch.hpp>

#include <fstream>
#include <cassert>
#include <iostream>

#include <boost/algorithm/string.hpp>

std::vector<int> GetInstructions() {
  std::ifstream input_file(CURRENT_DIR "/input2.txt");
  std::string input;
  std::getline(input_file, input);
  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, input, boost::is_any_of(","));
  std::vector<int> instructions;
  for (const auto& token : tokens)
    instructions.push_back(std::stoi(token));
  instructions[1] = 12;
  instructions[2] = 2;
  return instructions;
}

void ProcessInstructions(std::vector<int>& instructions) {
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
}

TEST_CASE( "Day 2 solution part 1", "[day2]" ) {
  auto instructions = GetInstructions();
  ProcessInstructions(instructions);
  REQUIRE(instructions[0] == 3931283);
}