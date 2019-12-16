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
  processor.SetInputs({5});
  processor.ProcessInstructions(instructions);
  REQUIRE(instructions[0] == 314);
}
