#include <emulator/intcode.hpp>
#include <io/io.hpp>

#include <catch/catch.hpp>

#include <string>
#include <set>
#include <cassert>
#include <vector>

std::vector<std::string> GetCombinations(const std::vector<std::string> values) {
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

TEST_CASE( "Day 7 solution part 1", "[day7]" ) {
  auto combinations = GetCombinations({"0", "1", "2", "3", "4"});
  REQUIRE(combinations.size() == 120);

  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input7.txt");
  std::set<int> outputs;
  for (const auto& combination : combinations) {
    int current_input = 0;
    for (auto c : combination) {
      auto instructions = original_instructions;
      int phase_setting = c - '0';
      emulator::Intcode processor;
      processor.SetInputs({phase_setting, current_input});
      processor.ProcessInstructions(instructions);
      current_input = processor.GetOutputs()[0];
    }
    outputs.insert(current_input);
  }

  REQUIRE(*outputs.rbegin() == 21000);

}

TEST_CASE( "Day 7 solution, part 2", "[day7]" ) {
  auto combinations = GetCombinations({"5", "6", "7", "8", "9"});
  REQUIRE(combinations.size() == 120);

  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input7.txt");
  auto instructions = original_instructions;
  std::set<int> outputs;
  for (const auto& combination : combinations) {
    int current_input = 0;
    for (auto c : combination) {
      int phase_setting = c - '0';
      emulator::Intcode processor;
      processor.SetInputs({phase_setting, current_input});
      processor.ProcessInstructions(instructions);
      current_input = processor.GetOutputs()[0];
    }
    outputs.insert(current_input);
  }

}
