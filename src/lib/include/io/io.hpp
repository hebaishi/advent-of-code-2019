#pragma once

#include <vector>
#include <string>

namespace io {
std::vector<int> ReadIntegers(const std::string& path);
std::vector<int64_t> ReadInstructions(const std::string& path);
}

