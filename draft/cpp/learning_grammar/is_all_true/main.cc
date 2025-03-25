/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/24 23:19:41
# Desc   : C++17： 用折叠表达式实现一个IsAllTrue函数: https://zhuanlan.zhihu.com/p/718967999
########################################################################
*/

#include <algorithm>
#include <initializer_list>
#include <print>

#include "cpp_utils/util.h"

struct Data {
  int value{0};
};  // struct Data

// 缺点：需要将参数全部先转换成 bool，但实际上只有有一个为 false，其他的就不用转换了
bool is_all_true_v1(const std::initializer_list<bool>& conditions) {
  PRINT_CURRENT_FUNCTION_NAME;
  return std::all_of(conditions.begin(), conditions.end(), [](bool a) { return a; });
}

// 基于一元右折叠的 is_all_true()
// 缺点：在 && 时，仍然需要先将所有 args 转换成 bool
template <typename... Args>
bool is_all_true_v2(Args&&... args) {
  PRINT_CURRENT_FUNCTION_NAME;
  // 我们只是调用了 std::forward<Args>(args)，于是编译器帮我们在编译时自动添加了 static_cast<bool>
  // 将 is_all_true_source.h 复制到 https://cppinsights.io/ 中 即可得到 is_all_true_insight.cc
  // 在 is_all_true_insight.h 可以看到编译器替我们添加了 static_cast<bool>
  return (std::forward<Args>(args) && ...);
}

// 使用 type traits 进一步优化
// note: 这里的实现逻辑和前两种不同，这里会对可调用对象进行调用，前两种不会
template <typename T>
bool is_true(T&& value) {
  if constexpr (std::is_invocable_v<T>) {  // 可调用对象，对其进行调用
    return std::forward<T>(value)();
  } else {  // 直接转换为 bool
    return static_cast<bool>(std::forward<T>(value));
  }
}

template <typename... Args>
bool is_all_true_v3(Args&&... args) {
  PRINT_CURRENT_FUNCTION_NAME;
  return (is_true(std::forward<Args>(args)) && ...);
}

void run_is_all_true() {
  int a = 1;
  bool b = true;
  auto c = []() {
    std::println("this is c: true");
    return true;
  };
  int* d{nullptr};
  auto e = []() {
    std::println("this is e: false");
    return false;
  };

  // note: 在不使用 static_cast<bool> 的情况下，clang++ 19.1.7 出现编译错误
  // 缺点：提前将 a, c 转换成了 bool, 但实际上 all_true 只要遇到一个 false 即可返回,
  // 可能存在多余的转换
  std::println(
      "is_all_true_v1(a, b, c): {}", is_all_true_v1({static_cast<bool>(a), b, static_cast<bool>(c)}));
  // 缺点：虽然在模板实例化时没有将 a、c 主动转换为 bool，但在内部实现 && 时，也需要将形参转换为 bool
  std::println("is_all_true_v2(a, b, c): {}", is_all_true_v2(a, b, c));
  std::println("is_all_true_v3(a, b, c): {}", is_all_true_v3(a, b, c));

  Data* d1 = new Data{1};

  std::println("is_all_true_v1({{d1, d1->value}}): {}",
      is_all_true_v1({static_cast<bool>(d1), static_cast<bool>(d1->value)}));
  std::println("is_all_true_v2(d1, d1->value): {}", is_all_true_v2(d1, d1->value));
  std::println("is_all_true_v3(d1, d1->value): {}",
      is_all_true_v3(d1, d1->value));

  Data* d2{nullptr};
  // 这里会出现运行时错误
  // is_all_true_v1 的参数是 std::initializer_list<bool>，d2 和 d2->value 会先转换成 bool 来构造
  // std::initializer_list<bool> 参数，但是 d2 是空指针，这就导致将 d2->value 转换成 bool 时出错
  // std::println("is_all_true_v1(d2, d2->value): {}",
  //     is_all_true_v1({static_cast<bool>(d2), static_cast<bool>(d2->value)}));

  // 这里会出现运行时错误
  // is_all_true_v2 是一个变参模板函数，在函数实例化后，多个 std::forward<Args>(args) 之间 && 时，编译器会主动
  // 通过 static_cast<bool> 将 forward 的结果转换成 bool，也就是先转换再执行 &&
  // 由于 d2 是 nullptr, 所以在 d2->value 转换成 bool 就会出错
  // std::println("is_all_true_v2(d2, d2->value): {}", is_all_true_v2(d2, d2->value));

  // 这个可以成功
  // is_all_true_v3 是一个变参模板函数，函数的实现是多个 is_true(std::forward<Args>(args))
  // &&，这样，编译器就不会提前将所有 args 转换成 bool，而是由 is_true 的函数内部来实现，同时 is_true 从左到右
  // 执行，当 d2 是空指针时，is_true(std::forwards<Data*>(d2)) 为 false，就不会继续执行后面的
  // is_true(std::forwards<int>(d2->value))
  std::println("is_all_true_v3(d2, d2->value): {}",
      is_all_true_v3(d2, d2->value));


  std::println("is_all_true_v2(b, c): {}", is_all_true_v2(b, c));  // true
  std::println("is_all_true_v2(d, c): {}", is_all_true_v2(d, c));  // false
  std::println("is_all_true_v3(b, c): {}", is_all_true_v3(b, c));  // true
  std::println("is_all_true_v3(d, c): {}", is_all_true_v3(d, c));  // false
  // is_all_true_v2 不会对可调用对象进行调用
  // is_all_true_v3 会对可调用对象进行调用
  std::println("is_all_true_v2(c, e): {}", is_all_true_v2(c, e));  // true
  std::println("is_all_true_v3(c, e): {}", is_all_true_v3(c, e));  // false
  // 执行 e 后就不会执行 c 了
  std::println("is_all_true_v3(e, c): {}", is_all_true_v3(e, c));  // false

}

int main() {
  run_is_all_true();
  return 0;
}
