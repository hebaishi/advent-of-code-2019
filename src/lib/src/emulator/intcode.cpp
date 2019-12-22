#include <array>
#include <iostream>
#include <algorithm>
#include <map>
#include <functional>

#include <emulator/intcode.hpp>

namespace emulator {

constexpr const int64_t HaltOpcode = 99;
constexpr const int64_t InputOpcode = 3;
constexpr const int64_t OutputOpcode = 4;

enum class Mode {
  Position = 0,
  Immediate = 1,
  Relative = 2
};

struct Instruction {
  int64_t opcode;
  std::array<Mode, 3> modes;
};


class Intcode::Impl {
public:
  Impl();

  void SetInstructions(const std::vector<int64_t>& instructions);

  void RegisterInstructions();

  State Run();

  void ProvideInput(int64_t value);

  int64_t GetOutput();

  std::vector<int64_t> GetInstructions();

  std::pair<int64_t, int64_t> GetOperands(std::array<Mode, 3> modes, int64_t ip);

  int64_t GetOperand(Mode mode, int64_t ip);

  int64_t GetOffset(Mode mode);

private:

  std::map<int64_t, std::function<int(std::array<Mode, 3>, int64_t)>> operations_;
  std::vector<int64_t> instructions_;
  State current_state_;
  size_t ip_, base_offset_;
  int64_t current_input_, current_output_;
  bool received_input_;
};

int64_t Intcode::Impl::GetOperand(Mode mode, int64_t ip) {
  if (mode == Mode::Immediate) {
    return instructions_[ip];
  } else {
    return instructions_[instructions_[ip] + GetOffset(mode)];
  }
}

int64_t Intcode::Impl::GetOffset(Mode mode) {
  return (mode == Mode::Relative) ? base_offset_ : 0;
}

std::pair<int64_t, int64_t> Intcode::Impl::GetOperands(std::array<Mode, 3> modes, int64_t ip) {
  return {
    GetOperand(modes[0], ip+1),
    GetOperand(modes[1], ip+2),
  };
}

static Instruction ProcessOpcode(int code) {
  auto str = std::to_string(code);
  std::string leading_zeroes(5 - str.length(), '0');
  str = leading_zeroes + str;
  Instruction instruction;
  instruction.opcode = std::stoi(str.substr(3));
  instruction.modes = {Mode::Position, Mode::Position, Mode::Position};
  for (int idx = 2 ; idx >= 0 ; idx--)
    instruction.modes[2-idx] = static_cast<Mode>(str[idx] - '0');
  return instruction;
}

Intcode::Impl::Impl() : instructions_(50000, 0), ip_(0), base_offset_(0), received_input_(false) {}

Intcode::Intcode() : pimpl_(new Impl()) {
  pimpl_->RegisterInstructions();
}

State Intcode::Run() {
  return pimpl_->Run();
}

void Intcode::SetInstructions(const std::vector<int64_t> &instructions) {
  return pimpl_->SetInstructions(instructions);
}

void Intcode::Impl::SetInstructions(const std::vector<int64_t> &instructions) {
  std::copy(instructions.begin(), instructions.end(), instructions_.begin());
}


std::vector<int64_t> Intcode::GetInstructions() {
  return pimpl_->GetInstructions();
}

std::vector<int64_t> Intcode::Impl::GetInstructions() {
  return instructions_;
}

Intcode::~Intcode() {}

State Intcode::Impl::Run() {
  while (ip_ < instructions_.size()) {
    auto instruction = ProcessOpcode(instructions_[ip_]);
    if (operations_.find(instruction.opcode) == operations_.end()) {
      throw std::runtime_error("Unsupported code: " + std::to_string(instruction.opcode));
    } else {
      if (instruction.opcode == InputOpcode && !received_input_) {
        current_state_ = State::AwaitingInput;
        break;
      }
      ip_ = operations_[instruction.opcode](instruction.modes, ip_);
      received_input_ = false;
      if (instruction.opcode == HaltOpcode) {
        current_state_ = State::Halted;
        break;
      } else if (instruction.opcode == OutputOpcode) {
        current_state_ = State::ProvidedOutput;
        break;
      }
    }
  }
  return current_state_;
}

void Intcode::ProvideInput(int64_t value) {
  pimpl_->ProvideInput(value);
}

int64_t Intcode::GetOutput() {
  return pimpl_->GetOutput();
}

void Intcode::Impl::ProvideInput(int64_t value) {
  current_input_ = value;
  received_input_ = true;
}

int64_t Intcode::Impl::GetOutput() {
  return current_output_;
}


void Intcode::Impl::RegisterInstructions() {
  operations_[99] = [](std::array<Mode, 3>, int64_t ip) -> int64_t {
    return ip + 1;
  };

  operations_[1] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(modes, ip);
    instructions_[ instructions_[ip+3] + GetOffset(modes[2]) ] = operands.first + operands.second;
    return ip + 4;
  };

  operations_[2] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(modes, ip);
    instructions_[ instructions_[ip+3] + GetOffset(modes[2]) ] = operands.first * operands.second;
    return ip + 4;
  };

  operations_[3] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    instructions_[ instructions_[ip + 1] + GetOffset(modes[0])] = current_input_;
    return ip + 2;
  };

  operations_[4] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    int64_t value = GetOperand(modes[0], ip+1);
    current_output_ = value;
    return ip + 2;
  };

  operations_[5] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(modes, ip);
    if (operands.first != 0) {
      return operands.second;
    } else {
      return ip + 3;
    }
  };

  operations_[6] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(modes, ip);
    if (operands.first == 0) {
      return operands.second;
    } else {
      return ip + 3;
    }
  };

  operations_[7] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(modes, ip);
    if (operands.first < operands.second) {
      instructions_[instructions_[ip+3] + GetOffset(modes[2])] = 1;
    } else {
      instructions_[instructions_[ip+3] + GetOffset(modes[2])] = 0;
    }
    return ip + 4;
  };

  operations_[8] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(modes, ip);
    if (operands.first == operands.second) {
      instructions_[instructions_[ip+3] + GetOffset(modes[2])] = 1;
    } else {
      instructions_[instructions_[ip+3] + GetOffset(modes[2])] = 0;
    }
    return ip + 4;
  };

  operations_[9] = [this](std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    int64_t operand = GetOperand(modes[0], ip+1);
    base_offset_ += operand;
    return ip + 2;
  };


}

}
