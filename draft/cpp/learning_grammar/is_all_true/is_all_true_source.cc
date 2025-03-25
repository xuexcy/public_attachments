/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/25 01:23:44
# Desc   :
########################################################################
*/

#include <iostream>
#include <utility>

template <typename... Args>
bool is_all_true(Args... args) {
  return (std::forward<Args>(args) && ...);
}

template <typename T>
bool is_true(T&& value) {
  if constexpr (std::is_invocable_v<T>) {
    return std::forward<T>(value)();
  } else {
    return static_cast<bool>(std::forward<T>(value));
  }
}
template <typename... Args>
bool is_all_true_with_call_function(Args&&... args) {
  return (is_true(std::forward<Args>(args)) && ...);
}

int main() {
  int a = 1;
  bool b = true;
  auto c = []() { return true; };

  std::cout << is_all_true(a, b, c) << std::endl;
  std::cout << is_all_true_with_call_function(a, b, c) << std::endl;
}

