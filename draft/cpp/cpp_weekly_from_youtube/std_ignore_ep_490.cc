/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/07/26 13:55:07
# Desc   : https://www.youtube.com/watch?v=iUcS0LCj2Ko
########################################################################
*/

#include <tuple>

// _ VS std::ignore VS [[maybe_unused]]

/* nodiscard: 提醒开发者关注表达式返回值，如果未获取其返回值，则编译器告警
[[nodiscard]] 或则 [[nodiscard("Message")]]
*/
[[nodiscard("check value")]] int get_value() {
  return 42;
}

/*
std::ignore: c++11, 返回值会立刻销毁(析构)
[[maybe_unused]]: c++17, 返回值到作用域结束销毁
_ : c++26, 返回值到作用域结束后析构
*/
int main() {
  {
    get_value();  // get a warning from compiler
    auto x = get_value();  // unused value
  }

  {
    std::ignore = get_value();
    [[maybe_unused]] auto y = get_value();  // no warning of unused
    auto _ = get_value();
  }

  return 0;
}

