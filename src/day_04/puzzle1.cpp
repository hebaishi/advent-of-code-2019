#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>

bool IsPasswordValid(size_t input) {
  assert(input > 0);
  auto password = std::to_string(input);
  for (size_t idx = 1 ; idx < password.length() ; idx++) {
    if (password[idx] < password[idx-1])
      return false;
  }
  return std::adjacent_find(password.begin(), password.end()) != password.end();
}

bool ContainsDoubleRepeat(size_t input) {
  auto password = std::to_string(input);
  int current_run_length = 1;
  auto last_char = password[0];

  bool double_found = false;

  for (auto it = password.begin() + 1 ; it != password.end() ; it++) {
    if (*it == last_char) {
      current_run_length++;
    } else {
      if (current_run_length == 2)
        double_found = true;
      current_run_length = 1;
    }
    last_char = *it;
  }

  return double_found || (current_run_length == 2);
}


int main() {
  size_t count = 0;
  for (size_t value = 359282 ; value <= 820401 ; value++)
    if (IsPasswordValid(value))
      count++;

  std::cout << count << std::endl;

  count = 0;

  for (size_t value = 359282 ; value <= 820401 ; value++)
    if (IsPasswordValid(value) && ContainsDoubleRepeat(value))
      count++;

  std::cout << count << std::endl;

  return 0;
}
