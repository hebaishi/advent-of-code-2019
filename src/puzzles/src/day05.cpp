#include <io/io.hpp>

#include <catch/catch.hpp>

#include <fstream>
#include <cassert>
#include <iostream>
#include <array>

#include <boost/algorithm/string.hpp>

enum class Mode {
  Position = 0,
  Immediate = 1
};

struct Instruction {
  int code;
  std::array<Mode, 3> modes;
};

Instruction ProcessOpcode(int code) {
  auto str = std::to_string(code);
  std::string leading_zeroes(5 - str.length(), '0');
  str = leading_zeroes + str;
  Instruction instruction;
  instruction.code = std::stoi(str.substr(3));
  instruction.modes = {Mode::Position, Mode::Position, Mode::Position};
  for (int idx = 2 ; idx >= 0 ; idx--) {
    if (str[idx] == '1') {
      instruction.modes[2-idx] = Mode::Immediate;
    }
  }
  return instruction;

}

std::pair<int, int> GetOperands(const std::vector<int> instructions, std::array<Mode, 3> modes, std::pair<int, int> values) {
  return {
    modes[0] == Mode::Immediate ? values.first : instructions[values.first],
    modes[1] == Mode::Immediate ? values.second : instructions[values.second]
  };
}

void ProcessInstructions5(std::vector<int>& instructions) {
  size_t ip = 0;
  while (ip < instructions.size()) {
    auto instruction = ProcessOpcode(instructions[ip]);
    if (instruction.code == 99)
      break;

    std::pair<int, int> operands;
    operands = GetOperands(instructions, instruction.modes, { instructions[ip+1], instructions[ip+2] });

    switch (instruction.code) {
    case 1: {
      instructions[ instructions[ip+3] ] = operands.first + operands.second;
      ip += 4;
      break;
    }
    case 2: {
      instructions[ instructions[ip+3] ] = operands.first * operands.second;
      ip += 4;
      break;
    }
    case 3: {
      int value = 5;
      std::cout << "Enter value:" << std::endl;
      instructions[ instructions[ip+1] ] = value;
      ip += 2;
      break;
    }
    case 4:
      std::cout << "Value = " << (instruction.modes[0] == Mode::Immediate ? instructions[ip+1] : instructions[instructions[ip+1]]) << std::endl;
      ip += 2;
      break;
    case 5:
      if (operands.first != 0) {
        ip = operands.second;
      } else {
        ip += 3;
      }
      break;
    case 6:
      if (operands.first == 0) {
        ip = operands.second;
      } else {
        ip += 3;
      }
      break;
    case 7:
      if (operands.first < operands.second) {
        instructions[instructions[ip+3]] = 1;
      } else {
        instructions[instructions[ip+3]] = 0;
      }
      ip += 4;
      break;
    case 8:
      if (operands.first == operands.second) {
        instructions[instructions[ip+3]] = 1;
      } else {
        instructions[instructions[ip+3]] = 0;
      }
      ip += 4;
      break;
    default:
      throw std::runtime_error("Unknown opcode encountered");
      break;
    }
  }
}

TEST_CASE( "Day 5 solution", "[day5]" ) {
  auto instructions = io::ReadInstructions(CURRENT_DIR "/input5.txt");
  ProcessInstructions5(instructions);
  REQUIRE(instructions[0] == 314);
}
