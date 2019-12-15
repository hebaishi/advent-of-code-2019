#include <fstream>
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
  return instructions;
}

void ReplaceInstructions(std::vector<int>& instructions, int a, int b) {
  instructions[1] = a;
  instructions[2] = b;
}

int ProcessInstructions(std::vector<int>& instructions) {
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

int main() {
  auto original_instructions = GetInstructions();
  for (size_t a = 0 ; a <= 99 ; a++) {
    for (size_t b = 0 ; b <= 99 ; b++) {
      auto new_instructions = original_instructions;
      ReplaceInstructions(new_instructions, a, b);
      if (ProcessInstructions(new_instructions) == 19690720) {
        std::cout << ((a * 100) + b) << std::endl;
        assert(((a * 100) + b) == 6979);
      }
    }
  }
  return 0;
}
