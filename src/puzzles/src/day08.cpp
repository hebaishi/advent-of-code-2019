#include <catch/catch.hpp>

#include <space/image.hpp>

#include <io/io.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

TEST_CASE( "Day 8 solution part 1", "[day8]" ) {
  auto input_data = io::ReadLine(CURRENT_DIR "/input8.txt");
  auto image = space::Image(input_data, 25, 6);
  int min_count = -1;
  size_t min_count_layer = -1;

  for (size_t index = 0 ; index < image.GetNumLayers() ; index++) {
    auto current_layer = image.GetLayer(index);
    int current_zero_count = std::count(current_layer.begin(), current_layer.end(), '0');
    if (min_count == -1 || current_zero_count < min_count) {
      std::swap(min_count, current_zero_count);
      std::swap(min_count_layer, index);
    }
  }

  auto layer = image.GetLayer(min_count_layer);
  size_t num_ones = std::count(layer.begin(), layer.end(), '1');
  size_t num_twos = std::count(layer.begin(), layer.end(), '2');

  REQUIRE((num_ones * num_twos) == 2210);
}


TEST_CASE( "Day 8 solution part 2", "[day8]" ) {
  auto input_data = io::ReadLine(CURRENT_DIR "/input8.txt");
  auto image = space::Image(input_data, 25, 6);


  auto render = image.Render();
  REQUIRE(render == "011000110011110011001111010010100101000010010100001000010000111001000011100100001011010000101101000010010100101000010010100000110001110111100111011110");
}
