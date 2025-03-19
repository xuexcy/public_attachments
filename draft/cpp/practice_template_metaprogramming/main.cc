/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/19 19:13:08
# Desc   : refs: https://zhuanlan.zhihu.com/p/354927495
########################################################################
*/

#include <cstddef>
#include <iostream>
#include <ostream>
#include <print>

template <size_t N>
struct SumUp {
  enum { value = SumUp<N-1>::value + N };
};  // struct SumUp

template <>
struct SumUp<0> {
  enum { value = 0 };
};  // struct SumUp<0>

template <typename T, T v>
struct integral_constant {
  static constexpr T value = v;
  using value_type = T;
  using type = integral_constant<T, v>;
  constexpr value_type operator()() const noexcept {
    return value;
  }
  // 隐式转换
  constexpr operator value_type() const noexcept {
    std::println(
      "integral_constant<{0}, {1}>::operator {0}()",
      typeid(value_type).name(), value);
    return value;
  }
};  // struct integral_constant

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

void run_integral_constant() {
  using a_float = integral_constant<float, 2.33f>;
  true_type t;
  std::cout << t << std::endl;
  std::println("true_type::value: {}", true_type::value);
  std::println("true_type::value: {}", true_type::value);
  std::println("false_type::value: {}", false_type::value);
  std::println("a_float::value: {}", a_float::value);
}

template <typename T>
struct identity {
  using type = T;
};
template <typename T>
using identity_t = typename identity<T>::type;

template <bool B, typename T = void>
struct enable_if_ {};
template <typename T>
struct enable_if_ <true, T> { using type = T; };
template <bool B, typename T = void>
using enable_if_t_ = enable_if_<B, T>::type;

template <typename T>
struct is_const_value : false_type {};
template <typename T>
struct is_const_value<const T*> : true_type {};
template <typename T>
struct is_const_value<const volatile T*> : true_type {};
template <typename T>
struct is_const : is_const_value<T*> {};
template <typename T>
struct is_const<T&> : false_type {};

template <typename>
struct is_function : false_type {};
// Args... 表示可变参数
// ... 表示不定参数
template <typename ReturnType, typename... Args>
struct is_function<ReturnType(Args...)> : true_type {};
template <typename ReturnType, typename... Args>
struct is_function<ReturnType(Args..., ...)> : true_type {};
template <typename... Ts>
constexpr bool is_function_v = is_function<Ts...>::value;

void f1() {}
void f2(int, int) {}
void f3(int, ...) {}
void run_is_function() {
  f1();
  f2(1, 2);
  f3(1, 2.0, 3.0);
  std::println("is_function<f1>: {}", is_function<decltype(f1)>::value);
  std::println("is_function<f2>: {}", is_function<decltype(f2)>::value);
  std::println("is_function<f3>: {}", is_function<decltype(f3)>::value);
  std::println("is_function<int>: {}", is_function<int>::value);
}


template <typename T>
struct remove_const { using type = T; };
template <typename T>
struct remove_const<const T> { using type = T; };
template <typename T>
struct remove_const<const T[]> { using type = T[]; };
template <typename T, size_t N>
struct remove_const<const T[N]> { using type = T[N]; };

template <typename T>
struct rank : public integral_constant<size_t, 0> {};
template <typename T>
struct rank<T[]> : public integral_constant<size_t, rank<T>::value + 1> {};
template <typename T, size_t N>
struct rank<T[N]> : public integral_constant<size_t, rank<T>::value + 1> {};
template <typename T>
constexpr auto rank_v = rank<T>::value;

void run_rank() {
  std::println("rank<int>: {}", rank_v<int>);
  std::println("rank<int[]>: {}", rank_v<int[]>);
  std::println("rank<int[10]>: {}", rank_v<int[10]>);
  std::println("rank<int[10][20]>: {}", rank_v<int[10][20]>);
}

template <size_t... Integrals>
struct static_max {};
template <size_t I0>
struct static_max<I0> {
  static const constexpr size_t value = I0;
};
template <size_t I0, size_t I1, size_t... I>
struct static_max<I0, I1, I...> {
  static const constexpr size_t value = (I0 > I1) ?
    static_max<I0, I...>::value :
    static_max<I1, I...>::value;
};

void run_static_max() {
  std::println("static_max<1>::value: {}", static_max<1>::value);
  std::println("static_max<1, 2, 3, 4, 5>::value: {}", static_max<1, 2, 3, 4, 5>::value);
}

// 编译期求 and
template <typename ...>
struct conjunction : true_type {};
template <typename T>
struct conjunction<T> : T {};
template <typename T, typename... Rest>
struct conjunction<T, Rest...> : std::conditional<bool(T::value), conjunction<Rest...>, T>::type {};
template <typename... Ts>
constexpr bool conjunction_v = conjunction<Ts...>::value;

void run_conjunction() {
  std::println("conjunction_<>>::value: {}", conjunction_v<>);
  int a{0};
  std::println(
    "conjunction_v<is_const<a>, is_function<f1>>: {}",
    conjunction_v<is_const<decltype(a)>, is_function<decltype(f1)>>);
  const int b{0};
  std::println(
    "conjunction_v<is_const<b>, is_function<f1>>: {}",
    conjunction_v<is_const<decltype(b)>, is_function<decltype(f1)>>);
}

using yes_type = char;
struct no_type {
  char pad[8];
};  // struct no_type

// void -> void : yes
// void -> not void: no
// From -> 数组: no
// From -> 函数: no
template <typename From, typename To, bool = (std::is_void_v<From> || std::is_function_v<To> ||
  std::is_array_v<To>)>
struct is_convertible_impl {
  static constexpr bool value = std::is_void_v<To>;
};
template <typename From, typename To>
struct is_convertible_impl<From, To, false> {
private:
  template <typename T>
  static void ToFunction(T arg);
  // 这里一定要有 int 作为参数
  template <typename F, typename T>
  static decltype(ToFunction<T>(std::declval<F>()), yes_type()) is(int);
  template <typename, typename>
  static no_type is(...);
public:
  static const bool value = sizeof(is<From, To>(0)) == sizeof(yes_type);
};  // struct is_convertible_impl<From, To, false>

template <typename From, typename To>
struct is_convertible : integral_constant<bool, is_convertible_impl<From, To>::value> {};
template <typename From, typename To>
constexpr bool is_convertible_v = is_convertible<From, To>::value;

void run_is_convertible() {
  int a{0};
  double b{0};
  char c{0};
  std::println("is_convertible<int, double>::value: {}", is_convertible_v<int, double>);
  std::println("is_convertible<int, char>::value: {}", is_convertible_v<int, char>);
  std::println("is_convertible<double, char>::value: {}", is_convertible_v<double, char>);
}

int main() {
  run_integral_constant();
  run_is_function();
  run_rank();
  run_static_max();
  run_conjunction();
  run_is_convertible();
  return 0;
}
