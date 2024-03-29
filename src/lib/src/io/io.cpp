#include <io/io.hpp>

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <iostream>

namespace io {

std::vector<int> ReadIntegers(const std::string& path) {
  std::vector<int> output;
  std::ifstream input_file(path);
  std::string line;
  while (std::getline(input_file, line))
    output.push_back(std::stoi(line));
  return output;
}

std::vector<int64_t> ReadInstructions(const std::string& path) {
  std::ifstream input_file(path);
  std::string input;
  std::getline(input_file, input);
  std::vector<std::string> tokens;
  boost::algorithm::split(tokens, input, boost::is_any_of(","));
  std::vector<int64_t> instructions;
  for (const auto& token : tokens)
    instructions.push_back(std::stoll(token));
  return instructions;
}

std::string ReadLine(const std::string& path) {
  std::ifstream input_file(path);
  std::string input;
  std::getline(input_file, input);
  return input;
}
}

