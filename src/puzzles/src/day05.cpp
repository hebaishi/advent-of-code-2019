#include <io/io.hpp>
#include <emulator/intcode.hpp>

#include <catch/catch.hpp>

#include <fstream>
#include <cassert>
#include <iostream>
#include <array>

TEST_CASE( "Day 5 solution", "[day5]" ) {
  auto instructions = io::ReadInstructions(CURRENT_DIR "/input5.txt");
  emulator::Intcode processor;
  processor.SetInstructions(instructions);
  auto state = processor.Run();
  REQUIRE(state == emulator::State::AwaitingInput);
  processor.ProvideInput(5);
  processor.Run();
  REQUIRE(processor.GetInstructions()[0] == 314);
}
