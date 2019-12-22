#include <io/io.hpp>

#include <emulator/intcode.hpp>

#include <catch/catch.hpp>

#include <iostream>

TEST_CASE( "Day 9 solution part 1", "[day9]" ) {
  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input9.txt");
  emulator::Intcode processor;
  processor.SetInstructions(original_instructions);
  auto state = processor.Run();
  if (state == emulator::State::AwaitingInput) {
    processor.ProvideInput(1);
  }
  state = processor.Run();
  REQUIRE(processor.GetOutput() == 2682107844);
}

TEST_CASE( "Day 9 solution part 2", "[day9]" ) {
  auto original_instructions = io::ReadInstructions(CURRENT_DIR "/input9.txt");
  emulator::Intcode processor;
  processor.SetInstructions(original_instructions);
  auto state = processor.Run();
  if (state == emulator::State::AwaitingInput) {
    processor.ProvideInput(2);
  }
  state = processor.Run();
  REQUIRE(processor.GetOutput() == 34738);
}

