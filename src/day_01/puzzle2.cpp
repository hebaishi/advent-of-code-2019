#include <fstream>
#include <cassert>
#include <iostream>

int main() {
  std::ifstream input_file(CURRENT_DIR "/input1.txt");
  std::string line;
  size_t total = 0;
  while (std::getline(input_file, line)){
    size_t current = std::stoi(line);
    int current_total = static_cast<int>((current / 3) - 2);
    total += current_total;
    while (current_total > 0) {
      current_total = static_cast<int>((current_total / 3) - 2);
      if (current_total > 0)
        total += current_total;
    }
  }
  std::cout << "total = " << total << std::endl;
  assert(total == 4977473);
  return 0;
}
