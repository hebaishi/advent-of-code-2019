#include <fstream>
#include <vector>
#include <cassert>
#include <iostream>

std::vector<int> GetValues() {
  std::vector<int> output;
  std::ifstream input_file(CURRENT_DIR "/input1.txt");
  std::string line;
  size_t total = 0;
  while (std::getline(input_file, line))
    output.push_back(std::stoi(line));
  return output;
}

size_t CalculateTotal(const std::vector<int>& values, bool increase_accuracy = false) {
  size_t total = 0;
  int current_total = 0;
  for (const auto& current : values) {
    current_total = static_cast<int>((current / 3) - 2);
    total += current_total;
    if (increase_accuracy) {
      while (current_total > 0) {
        current_total = static_cast<int>((current_total / 3) - 2);
        if (current_total > 0)
          total += current_total;
      }
    }
  }
  return total;
}

int main() {
  auto total = CalculateTotal(GetValues());
  std::cout << "total = " << total << std::endl;
  assert(total == 3320226);

  total = CalculateTotal(GetValues(), true);
  std::cout << "total = " << total << std::endl;
  assert(total == 4977473);

  return 0;
}
