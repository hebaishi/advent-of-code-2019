#include <emulator/intcode.hpp>
#include <io/io.hpp>

#include <catch/catch.hpp>

#include <string>
#include <list>
#include <set>
#include <cassert>
#include <vector>
#include <iostream>

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
    std::vector<emulator::Intcode> amplifiers;
    for (size_t idx = 0 ; idx < combination.length() ; idx++) {
      amplifiers.emplace_back();
      amplifiers.back().SetInstructions(original_instructions);
    }

    std::list<int64_t> inputs;
    int64_t current_input = 0;
    for (size_t index = 0 ; index < combination.length() ; index++) {
      auto& current_amplifier = amplifiers[index];
      int phase_setting = combination[index] - '0';
      auto state = current_amplifier.Run();
      REQUIRE(state == emulator::State::AwaitingInput);
      current_amplifier.ProvideInput(phase_setting);
      state = current_amplifier.Run();
      REQUIRE(state == emulator::State::AwaitingInput);
      current_amplifier.ProvideInput(current_input);
      state = current_amplifier.Run();
      REQUIRE(state == emulator::State::ProvidedOutput);
      current_input = current_amplifier.GetOutput();
    }
    outputs.insert(current_input);
  }

  REQUIRE(*outputs.rbegin() == 21000);

}

TEST_CASE( "Day 7 solution, part 2", "[day7]" ) {
  auto combinations = GetCombinations({"5", "6", "7", "8", "9"});
  REQUIRE(combinations.size() == 120);
  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input7.txt");
  std::set<int> outputs;

  for (const auto& combination : combinations) {
    std::vector<emulator::Intcode> amplifiers;
    for (size_t idx = 0 ; idx < combination.length() ; idx++) {
      amplifiers.emplace_back();
      amplifiers.back().SetInstructions(original_instructions);
    }

    std::list<int64_t> inputs;
    int64_t current_input = 0;
    int num_iterations = 0;
    while(num_iterations >= 0) {
      for (size_t index = 0 ; index < combination.length() ; index++) {
        auto& current_amplifier = amplifiers[index];
        int phase_setting = combination[index] - '0';
        auto state = current_amplifier.Run();
        if (num_iterations == 0) {
          assert(state == emulator::State::AwaitingInput);
          current_amplifier.ProvideInput(phase_setting);
          state = current_amplifier.Run();
        }
        if (state == emulator::State::AwaitingInput) {
          current_amplifier.ProvideInput(current_input);
          state = current_amplifier.Run();
        }
        if (state == emulator::State::ProvidedOutput) {
          current_input = current_amplifier.GetOutput();
        } else if (state == emulator::State::Halted && index == 4) {
          num_iterations = -6;
          break;
        }

      }

      num_iterations++;
    }
    outputs.insert(current_input);
  }

  REQUIRE(*outputs.rbegin() == 61379886);
}
