#include <fstream>
#include <cassert>
#include <iostream>

int main() {
  std::ifstream input_file(CURRENT_DIR "/input1.txt");
  std::string line;
  size_t total = 0;
  while (std::getline(input_file, line)){
    size_t current = std::stoi(line);
    total += static_cast<int>((current / 3) - 2);
  }
  std::cout << "total = " << total << std::endl;
  assert(total == 3320226);
  return 0;
}
