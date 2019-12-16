#pragma once

#include <array>
#include <map>
#include <functional>

#include <memory>

namespace emulator {

class Intcode {
public:

Intcode();

void SetInputs(const std::vector<int>& inputs);

const std::vector<int>& GetOutputs();

void ProcessInstructions(std::vector<int>& instructions);

~Intcode();

private:
class Impl;
std::unique_ptr<Impl> pimpl_;

};

}
