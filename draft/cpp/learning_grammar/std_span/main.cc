/**
########################################################################
#
# Copyright (c) 2026 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2026/05/13 14:30:08
# Desc   : std::span: C++20 引入的一个视图类型，它提供对一个连续内存序列（如数组、std::vector、std::array）的非拥有观察
########################################################################
*/
#include <algorithm>
#include <cassert>
#include <print>
#include <span>
#include <vector>

void f(const std::span<int>& s) {
  for (auto i : s) {
    std::print("{} ", i);
  }
  std::println();
}

int main() {
  std::vector<int> v{1, 2, 3, 4};
  int arr[] = {5, 6, 7, 8};
  f(v);
  f(arr);
  std::span<int> s(v);
  auto sub = s.subspan(1, 2);  // {2, 3}
  f(sub);
  assert(std::ranges::equal(s.subspan(0, 2), std::span<int>(s.first<2>())));
  assert(std::ranges::equal(s.subspan(2, 2), std::span<int>(s.last<2>())));
  assert(4 == s.size());
  assert(!s.empty());
}
