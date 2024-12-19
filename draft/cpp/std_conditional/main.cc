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

template <bool UseInt>
struct IntegerType {
  using type = conditional_t<UseInt, int, long>;
};  // struct IntegerType

template <class T1, class T2>
auto max(T1 t1, T2 t2) -> conditional_t<(sizeof(t1) > sizeof(t2)), T1, T2> {
  return t1 > t2 ? t1 : t2;
}

void run() {
  constexpr bool b{false};
  conditional_t<b, A, B> f;
  f.print();

  IntegerType<true>::type i = 1;
  std::println("i: {}", i);
  IntegerType<false>::type j = 1;
  std::println("j: {}", j);

  {
    int i = 1;
    short j = 2;
    double k = 3.0;
    auto m = max(i, j);
    static_assert(std::is_same_v<decltype(m), int>);
    auto m2 = max(i, k);
    static_assert(std::is_same_v<decltype(m2), double>);
  }
}
int main() {
  run();
  return 0;
}
