#include <array>
#include <map>
#include <functional>

#include <emulator/intcode.hpp>

namespace emulator {

enum class Mode {
  Position = 0,
  Immediate = 1
};

struct Instruction {
  int opcode;
  std::array<Mode, 3> modes;
};

static std::pair<int, int> GetOperands(const std::vector<int> instructions, std::array<Mode, 3> modes, int ip) {
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

  void SetInputs(const std::vector<int>& inputs);

  const std::vector<int>& GetOutputs();

  void ProcessInstructions(std::vector<int>& instructions);

  void RegisterInstructions();

private:

  std::map<int, std::function<int(std::vector<int>&, std::array<Mode, 3>, int)>> operations_;
  std::vector<int> instructions_, inputs_, outputs_;
  std::vector<int>::iterator current_input_;
};



Intcode::Intcode() : pimpl_(new Impl()) {
  pimpl_->RegisterInstructions();
}

void Intcode::SetInputs(const std::vector<int> &inputs) {
  pimpl_->SetInputs(inputs);
}

const std::vector<int>& Intcode::GetOutputs() {
  return pimpl_->GetOutputs();
}

void Intcode::ProcessInstructions(std::vector<int>& instructions) {
  pimpl_->ProcessInstructions(instructions);
}

Intcode::~Intcode() {}

void Intcode::Impl::SetInputs(const std::vector<int>& inputs) {
  inputs_ = inputs;
  current_input_ = inputs_.begin();
}

const std::vector<int>& Intcode::Impl::GetOutputs() {
  return outputs_;
}

void Intcode::Impl::ProcessInstructions(std::vector<int>& instructions) {
  int ip = 0;
  while (ip < instructions.size()) {
    auto instruction = ProcessOpcode(instructions[ip]);
    if (operations_.find(instruction.opcode) == operations_.end()) {
      throw std::runtime_error("Unsupported code: " + std::to_string(instruction.opcode));
    } else {
      auto new_ip = operations_[instruction.opcode](instructions, instruction.modes, ip);
      if (new_ip >= 0)
        ip = new_ip;
      else
        break;
    }
  }
}

void Intcode::Impl::RegisterInstructions() {
  operations_[99] = [](std::vector<int>&, std::array<Mode, 3>, int) -> int {
    return -1;
  };

  operations_[1] = [](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    auto operands = GetOperands(instructions, modes, ip);
    instructions[ instructions[ip+3] ] = operands.first + operands.second;
    return ip + 4;
  };

  operations_[2] = [](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    auto operands = GetOperands(instructions, modes, ip);
    instructions[ instructions[ip+3] ] = operands.first * operands.second;
    return ip + 4;
  };

  operations_[3] = [this](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    instructions[ instructions[ip+1] ] = *current_input_;
    current_input_++;
    return ip + 2;
  };

  operations_[4] = [this](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    outputs_.push_back(modes[0] == Mode::Immediate ? instructions[ip+1] : instructions[instructions[ip+1]]);
    return ip + 2;
  };

  operations_[5] = [](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first != 0) {
      return operands.second;
    } else {
      return ip + 3;
    }
  };


  operations_[6] = [](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first == 0) {
      return operands.second;
    } else {
      return ip + 3;
    }
  };

  operations_[7] = [](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first < operands.second) {
      instructions[instructions[ip+3]] = 1;
    } else {
      instructions[instructions[ip+3]] = 0;
    }
    return ip + 4;
  };

  operations_[8] = [](std::vector<int>& instructions, std::array<Mode, 3> modes, int ip) -> int {
    auto operands = GetOperands(instructions, modes, ip);
    if (operands.first == operands.second) {
      instructions[instructions[ip+3]] = 1;
    } else {
      instructions[instructions[ip+3]] = 0;
    }
    ip += 4;
  };
}

}
