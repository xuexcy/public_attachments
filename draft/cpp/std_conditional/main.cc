/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyunxue@gmail.com
# Date    :   2024/12/19 15:08:44
# Desc    :
########################################################################
*/

#include <print>

template <bool B, class T, class F>
struct conditional {
  using type = T;
};  // struct conditional

template <class T, class F>
struct conditional<false, T, F> {
  using type = F;
};  // struct conditional<false, T, F>

template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;

class A {
public:
  void print() { std::println("A"); }
};  // class A

class B {
public:
  void print() { std::println("B"); }
};  // class B

void run() {
  constexpr bool b{false};
  conditional_t<b, A, B> f;
  f.print();
}
int main() {
  run();
  return 0;
}
