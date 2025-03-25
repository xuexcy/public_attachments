/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/20 14:17:26
# Desc   :
########################################################################
*/

#include <print>

namespace anyone_with_lambda {
bool anyone(char x) {
  if (x == 'x' || x == 'X' || x == 'y' || x == 'Y' ) {
    return true;
  }
  return false;
}
// 折叠表达式 + 泛型 lambda
static auto lambda_anyone = [](auto&& k, auto&&... args) -> bool {
  return ((k == args) || ...);
};

void run_anyone() {
  char x = 'a';
  std::println("anyone(x) = {}", anyone(x));
  std::println("lambda_anyone(x, 'x', 'X', 'y', 'Y') = {}", lambda_anyone(x, 'x', 'X', 'y', 'Y'));
}
}  // namespace anyone_with_lambda

int main() {
  anyone_with_lambda::run_anyone();
  return 0;
}
