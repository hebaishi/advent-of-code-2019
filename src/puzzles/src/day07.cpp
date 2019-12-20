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
    for (auto _ : combination) {
      amplifiers.emplace_back();
      amplifiers.back().SetInstructions(original_instructions);
    }

    std::list<int64_t> inputs;
    int64_t current_input = 0;
    for (size_t index = 0 ; index < combination.length() ; index++) {
      auto& current_amplifier = amplifiers[index];
      int phase_setting = combination[index] - '0';
      auto state = current_amplifier.Run();
      assert(state == emulator::State::AwaitingInput);
      current_amplifier.ProvideInput(phase_setting);
      state = current_amplifier.Run();
      assert(state == emulator::State::AwaitingInput);
      current_amplifier.ProvideInput(current_input);
      state = current_amplifier.Run();
      assert(state == emulator::State::ProvidedOutput);
      current_input = current_amplifier.GetOutput();
    }
    outputs.insert(current_input);
  }

  REQUIRE(*outputs.rbegin() == 21000);

}

TEST_CASE( "Day 7 solution, part 2", "[day7]" ) {
//  auto combinations = GetCombinations({"5", "6", "7", "8", "9"});
//  REQUIRE(combinations.size() == 120);

//  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input7.txt");
//  std::set<int> outputs;
//  for (const auto& combination : combinations) {
//    std::list<int64_t> inputs;
//    int64_t current_input = 0;
//    for (int64_t rep = 0 ; rep < 20 ; rep++) {
//      for (auto c : combination) {
//        emulator::Intcode processor;
//        auto instructions = original_instructions;
//        int phase_setting = c - '0';
//        inputs = {phase_setting, current_input};
//        auto input_it = inputs.begin();
//        processor.SetInputCallback([&input_it]() -> int64_t {
//          auto value = *input_it++;
//          return value;
//        });
//        processor.SetOutputCallback([&current_input, &inputs](int64_t value) {
//          if (inputs.size() == 2)
//            current_input = value;
//          inputs.push_back(value);
//        });
//        processor.ProcessInstructions(instructions);
//      }
//      outputs.insert(current_input);
//    }
//  }

//  REQUIRE(*outputs.rbegin() == 21000);
}
