/**
########################################################################
#
# Copyright (c) 2024 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2024/12/26 16:04:43
# Desc   : 通过 Inner 的实例地址，获取 Outer 的实例地址
########################################################################
*/

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <print>

class Outer;
class Inner {
public:
  int x;
  double y;
  static Outer* get_outer(const Inner& inner);
};  // class Inner

// 1. 所有非静态数据成员都具有相同的访问控制（public、protected、private）
// 2. 所有基类（如果有的话）也是标准布局类型
// 3. 没有虚函数或虚基类
// 4. 没有类型别名作为数据成员
// 否则，会出现 warning：‘offsetof’ applied to non-standard-layout type ‘Outer’
class Outer {
public:
  int a;
  double b;
  Inner inner;
};  // class Outer

Outer* Inner::get_outer(const Inner& inner) {
  // inner 必须是 Outer 的 public 成员
  return const_cast<Outer*>(reinterpret_cast<const Outer*>(
      reinterpret_cast<const char*>(&inner) - offsetof(Outer, inner)));
}

void run(const Inner& inner) {
  auto inner_addr = static_cast<const void*>(&inner);
  auto outer_addr = static_cast<const void*>(Inner::get_outer(inner));
  std::println("inner address: {}", inner_addr);
  std::println("outer address: {}", outer_addr);
  assert(sizeof(double) * 2 ==
      (reinterpret_cast<uintptr_t>(inner_addr) - reinterpret_cast<uintptr_t>(outer_addr)));
}
int main() {
  Outer outer;
  run(outer.inner);
  return 0;
}
