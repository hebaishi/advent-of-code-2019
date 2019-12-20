#pragma once

#include <array>
#include <map>
#include <functional>

#include <memory>

using InputCallback = std::function<int64_t()>;

using OutputCallback = std::function<void(int64_t)>;

namespace emulator {

enum class State {
  AwaitingInput,
  ProvidedOutput,
  Halted
};

class Intcode {
public:

Intcode();

void SetInstructions(const std::vector<int64_t>& instructions);

std::vector<int64_t> GetInstructions();

State Run();

void ProvideInput(int64_t value);

int64_t GetOutput();

~Intcode();

private:
class Impl;
std::shared_ptr<Impl> pimpl_;

};

}
