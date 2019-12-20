#include <array>
#include <iostream>
#include <map>
#include <functional>

#include <emulator/intcode.hpp>

namespace emulator {

constexpr const int64_t HaltOpcode = 99;
constexpr const int64_t InputOpcode = 3;
constexpr const int64_t OutputOpcode = 4;

enum class Mode {
  Position = 0,
  Immediate = 1
};

struct Instruction {
  int64_t opcode;
  std::array<Mode, 3> modes;
};

static std::pair<int64_t, int64_t> GetOperands(const std::vector<int64_t> instructions, std::array<Mode, 3> modes, int64_t ip) {
  return {
    modes[0] == Mode::Immediate ? instructions[ip+1] : instructions[instructions[ip+1]],
    modes[1] == Mode::Immediate ? instructions[ip+2] : instructions[instructions[ip+2]]
  };
}

static Instruction ProcessOpcode(int code) {
  auto str = std::to_string(code);
  std::string leading_zeroes(5 - str.length(), '0');
  str = leading_zeroes + str;
  Instruction instruction;
  instruction.opcode = std::stoi(str.substr(3));
  instruction.modes = {Mode::Position, Mode::Position, Mode::Position};
  for (int idx = 2 ; idx >= 0 ; idx--) {
    if (str[idx] == '1') {
      instruction.modes[2-idx] = Mode::Immediate;
    }
  }
  return instruction;
}

class Intcode::Impl {
public:
  Impl();

  void SetInstructions(const std::vector<int64_t>& instructions);

  void RegisterInstructions();

  void ResetCounter();

  State Run();

  void ProvideInput(int64_t value);

  int64_t GetOutput();

  std::vector<int64_t> GetInstructions();

private:

  std::map<int64_t, std::function<int(std::vector<int64_t>&, std::array<Mode, 3>, int64_t)>> operations_;
  std::vector<int64_t> instructions_;
  State current_state_;
  int ip_;
  int64_t current_input_, current_output_;
  bool received_input_;
};

Intcode::Impl::Impl() : ip_(0), received_input_(false) {}

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
  instructions_ = instructions;
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
      ip_ = operations_[instruction.opcode](instructions_, instruction.modes, ip_);
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
  operations_[99] = [](std::vector<int64_t>&, std::array<Mode, 3>, int64_t ip) -> int64_t {
    return ip + 1;
  };

  operations_[1] = [](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(instructions, modes, ip);
    instructions[ instructions[ip+3] ] = operands.first + operands.second;
    return ip + 4;
  };

  operations_[2] = [](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(instructions, modes, ip);
    instructions[ instructions[ip+3] ] = operands.first * operands.second;
    return ip + 4;
  };

  operations_[3] = [this](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    instructions[ instructions[ip+1] ] = current_input_;
    return ip + 2;
  };

  operations_[4] = [this](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    int64_t value = modes[0] == Mode::Immediate ? instructions[ip+1] : instructions[instructions[ip+1]];
    current_output_ = value;
    return ip + 2;
  };

  operations_[5] = [](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first != 0) {
      return operands.second;
    } else {
      return ip + 3;
    }
  };


  operations_[6] = [](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first == 0) {
      return operands.second;
    } else {
      return ip + 3;
    }
  };

  operations_[7] = [](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first < operands.second) {
      instructions[instructions[ip+3]] = 1;
    } else {
      instructions[instructions[ip+3]] = 0;
    }
    return ip + 4;
  };

  operations_[8] = [](std::vector<int64_t>& instructions, std::array<Mode, 3> modes, int64_t ip) -> int64_t {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first == operands.second) {
      instructions[instructions[ip+3]] = 1;
    } else {
      instructions[instructions[ip+3]] = 0;
    }
    return ip + 4;
  };
}

}
