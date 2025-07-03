/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/11 15:49:20
# Desc   : C++20: Concept详解以及个人理解: https://zhuanlan.zhihu.com/p/266086040
########################################################################
*/

#include <concepts>
#include <print>
#include <type_traits>
#include "cpp_utils/util.h"

template <typename T>
concept always_satisfied = true;

template <typename T>
concept integral = std::is_integral_v<T>;

template <typename T>
concept signed_integral = integral<T> && std::is_signed_v<T>;

template <always_satisfied T>
T mul(T a, T b) {
  return a * b;
}
template <integral T>
T add(T a, T b) {
  return a + b;
}

template <signed_integral T>
T subtract(T a, T b) {
  return a - b;
}

void run_1() {
  PRINT_CURRENT_FUNCTION_NAME;
  mul(1, 2);  // T => int
  mul(1.0f, 2.0f);  // T => float

  add(1, -2);  // T => int
  //  add(1.0f, 2.0f);
  // subtract(1U, 2U);
  subtract(1, 2);
  std::println();
}

// concept 的本质: 编译期的 bool 变量，相当于 constexpr bool

template <typename T, std::enable_if_t<std::is_integral_v<T>, T> = 0>
T add_original(T a, T b) {
  return a + b;
}

// 使用方法
// 1.
template <always_satisfied T>
void f_1_1(T v);
auto f_1_2 = []<always_satisfied T>(T v) {};
// 2.
template <typename T>
requires always_satisfied<T>
void f2_1(T v);
auto f2_2 = []<typename T> requires always_satisfied<T>(T v) {};
// 3.
template <typename T>
void f3_1(T v) requires always_satisfied<T>;
auto f3_2 = []<typename T>(T v) requires always_satisfied<T> {};
// 4. cpp14 auto 函数参数
void f4_1(always_satisfied auto v);
auto f4_2 = [](always_satisfied auto v) {};
// 5. auto 模板参数
template <always_satisfied auto v>
void g1_1();
auto g1_2 = []<always_satisfied auto v>() {};
// 6. auto 变量
always_satisfied auto foo = 1;

template <typename T>
concept Integral = std::is_integral_v<T>;
template <typename T>
concept SignedIntegral = Integral<T> && std::is_signed_v<T>;
template <typename T>
concept UnsignedIntegral = Integral<T> && !SignedIntegral<T>;

// concept 用于定义
// requires 用于使用
template <typename T>
concept callable = requires (T f) { f(); };  // 表明类型 T 的实例必须可以调用

template <typename T>
requires requires (T x) { x + x; }  // 两个 requires 表明: 要求(动词)满足某个要求(名词)
T add_2(T a, T b) {
  return a + b;
}

// 简单约束
template <typename T>
concept Addable = requires (T a, T b) {
  a + b;  // 需要 a + b 在编译时是 valid
};
template <typename T, typename U = T>
concept Swappable = requires (T&& t, U&& u) {
  swap(std::forward<T>(t), std::forward<U>(u));
  swap(std::forward<U>(u), std::forward<T>(t));
};

// 类型约束
struct Foo {
};  // struct Foo
struct Bar {
  using value = int;
  value data;
};  // struct Bar
struct Baz {
  using value = int;
  value data;
};  // struct Baz
template <typename T, typename = std::enable_if_t<std::is_same_v<T, Baz>>>
struct S {
};  // struct S
template <typename T>
using Ref = T&;  // T 的引用
template <typename T>
concept C = requires {
  typename T::value;  // T 有名为 value 的成员
  typename S<T>;  // T 和 Baz 同类型
  typename Ref<T>;  // T 可以被引用
};
template <C T>
void g(T a) {}


void run_2() {
  PRINT_CURRENT_FUNCTION_NAME;
  // g(Foo{});  // Foo 不符合 T::value
  // g(Bar{});  // Bar 不符合 S<T>
  g(Baz{});
  std::println();
}

// 复合约束
// {expression} noexcept(optional) -> type-constraint
/*
1. expression: 表达式合法
2. noexcept: 如果使用，表达式不能抛出异常
3. type-constraint:
  a. 模板参数类型符合约束
  b. 表达式类型(即delctype(expression)) 符合约束
*/
template <typename T>
concept D = requires(T x) {
  {*x} -> std::same_as<typename T::inner>;  // *x 可以转换为 T::inner 类型
  {x + 1} -> std::same_as<int>;
  {x * 1} -> std::same_as<T>;
};

// 嵌套约束
template <typename T>
concept Semiregular = std::is_default_constructible_v<T> &&
    std::copy_constructible<T> &&
    std::destructible<T> &&
    std::is_copy_assignable_v<T> &&
    requires (T a, size_t n) {
      requires std::is_same_v<T*, decltype(&a)>;
      // 表达式中后面没有分号 ';'
      { a.~T() } noexcept;
      requires std::is_same_v<T*, decltype(new T)>;
      requires std::is_same_v<T*, decltype(new T[n])>;
      { delete new T };
      { delete new T[n] };
};

// 约束类型: 与(conjunction)、或(disjunction)、原子(atomic constraints, 不包含任何AND/OR)
template <typename T> constexpr bool is_a = true;
template <typename T> constexpr bool is_b = true;
// 这是一个或(disjunction)
template <typename T> concept a_or_b = is_a<T> || is_b<T>;

// constraints normalization
template <typename T> concept A = T::value || true;  // A 永远为 true
template <typename T> concept B = A<T*>;
template <typename T> concept E = B<T&>;  // => A<T&*> , T&* 是一个无效类型，所以这个 concept 永远无法满足

template <A T>
void h1() {}
template <B T>
void h2() {}
template <E T>
void h3() {}
void run_constraint_normalization() {
  h1<Foo>();
  h1<Bar>();
  h2<Foo>();
  h2<Bar>();
  // Because substituted constraint expression is ill-formed: 'type name' declared as a pointer to a reference of type 'Foo &'
  // 一个引用的指针，这是一个不合法的类型
  // h3<Foo>();
}

template <typename T> constexpr bool is_mm = true;
template <typename T> constexpr bool is_cc = true;
template <typename T> concept MM = is_mm<T>;
template <typename T> concept BB = is_mm<T> && is_cc<T>;
template <typename T> concept GG = MM<T> && is_cc<T>;

template <MM T> void f1(T) {}
template <BB T> void f1(T) {}
template <MM T> void f2(T) {
  PRINT_CURRENT_FUNCTION_NAME;
}
template <GG T> void f2(T) {
  PRINT_CURRENT_FUNCTION_NAME;
}
// 原子约束相等: 在源代码层面相同
void run_equal() {
  PRINT_CURRENT_FUNCTION_NAME;
  // Call to 'f1' is ambiguous
  // 第一个 f1 的约束 normalization 后，是 is_mm<T>, 其来自 concept MM 的定义
  // 第二个 f1 的约束 normalization 后, 是 is_mm<T> && is_cc<T>, 其中 is_mm<T> 来自 concept BB 的定义
  // f1(0);

  // 第一个 f2 的约束 normalization 后，是 is_mm<T>, 其来自 concept MM 的定义
  // 第二个 f2 的约束 normalization 后，是 is_mm<T> && is_cc<T>, 其中 is_mm<T> 来自 concept MM 的定义
  // 也就是两个 is_mm 都源于 concept MM
  f2(0);  // 调用第 2 个 f2, 因为它的限制更多
  std::println();
}

// normalization 的返回结果是 bool，而不能是转换来的 bool
template <typename T>
struct G {
  constexpr operator bool() const { return true; }
};  // struct G
template <typename T>
//requires (S<T>{})  // clang 19.1.7 好像可以编译
requires (bool(S<T>{}))
// requires (S<T>{}())
void p(T) {
  PRINT_CURRENT_FUNCTION_NAME;
}

void p(int) {
  PRINT_CURRENT_FUNCTION_NAME;
}

void run_bool() {
  PRINT_CURRENT_FUNCTION_NAME;
  p(1);
  std::println();
}

int main() {
  run_1();
  run_2();
  run_constraint_normalization();
  run_equal();
  run_bool();
  return 0;
}
