#include <io/io.hpp>

#include <emulator/intcode.hpp>

#include <catch/catch.hpp>

#include <iostream>
#include <vector>

struct Iteration {
  int64_t input, output;
};

TEST_CASE( "Day 9 solution parts 1 + 2", "[day9]" ) {
  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input9.txt");
  std::vector<Iteration> iterations = {
    {1, 2682107844},
    {2, 34738}
  };

  for (const auto& iteration : iterations) {
    emulator::Intcode processor;
    processor.SetInstructions(original_instructions);
    auto state = processor.Run();
    if (state == emulator::State::AwaitingInput) {
      processor.ProvideInput(iteration.input);
    }
    state = processor.Run();
    REQUIRE(processor.GetOutput() == iteration.output);
  }
}
