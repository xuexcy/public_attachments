/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/23 13:42:00
# Desc   : 模板类在.h中定义，在.cpp中实现(https://zhuanlan.zhihu.com/p/147623943)
########################################################################
*/

#include <print>

#include "test_temp_1.h"
#include "test_temp_2.h"
#include "test_temp_3.cpp"
#include "test_temp_4.cpp"

#include "cpp_utils/util.h"

template <template <typename> class T, typename... Arg>
void run_test_temp() {
  PRINT_CURRENT_FUNCTION_NAME;
  ([] {
    T<Arg> t;
    t.set_value(Arg());
    std::println("value: {}", t.get_value());
    std::println();
  }(), ...);
  std::println();
}
void run_test_temp() {
  space1::TestTemp<int> t;  // 这个构造函数已经实例化了
  std::println();

  // 通过定义 void temporary_function() 方式好像会导致链接失败, 找不到 get_value 和 set_value 函数
  // 应该是函数内并没有调用这两个函数，所以没有实例化这两个函数，导致了链接失败
  // run_test_temp<space1::TestTemp, int, short>();

  run_test_temp<space2::TestTemp, float, double>();

  run_test_temp<space3::TestTemp, char, std::string>();

  run_test_temp<space4::TestTemp, unsigned int, long>();
}
int main() {
  run_test_temp();
  return 0;
}
