/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/22 16:19:59
# Desc   : 实现一个 std::array
########################################################################
*/
/*
1. 实现 operator[]
2. 实现 begin/end 用于 for loop
3. 实现 偏特化 std::tuple_size std::tuple_element 和 get 函数，用于结构化绑定
*/

#include <cstddef>
#include <print>
#include <tuple>
#include <type_traits>

template <typename T, std::size_t N>
struct array {
  T values[N];

  // operator []
  constexpr T& operator[](std::size_t idx) { return values[idx]; }
  constexpr const T& operator[](std::size_t idx) const { return values[idx]; }

  // begin()
  constexpr T* begin() { return values; }
  constexpr const T* begin() const { return values; }
  constexpr  const T* cbegin() const { return values; }

  // end()
  constexpr T* end() { return values + N; }
  constexpr const T* end() const { return values + N; }
  constexpr const T* cend() const { return values + N; }

  // front()
  constexpr T& front() { return *values; }
  constexpr const T& front() const { return *values; }

  // back()
  constexpr T& back() { return values[N - 1]; }
  constexpr const T& back() const { return values[N-1]; }
};  // struct array

/* 结构化绑定需要知道 大小/元素类型/根据idx取值
1. 实现 std::tuple_size<array<T, N>> 获取数组大小
2. 实现 std::tuple_element<Idx, array<T, N>>::type 获取数组元素类型, 由于 array 中的元素类型都是 T, 所以 type 跟 Idx 没什么关系
3. 实现 const T& get<Idx>(const array<T, N>&) 取值
*/
template <typename T, std::size_t N>
struct std::tuple_size<array<T, N>> : integral_constant<std::size_t, N> {
};  // struct tuple_size<array<T, N>>
template <std::size_t Idx, typename T, std::size_t N>
struct std::tuple_element<Idx, array<T, N>> {
  using type = T;
};  // struct std::tuple_element<Idx, array<T, N>>
template <std::size_t Idx, typename T, std::size_t N>
const T& get(const array<T, N>& arr) {
  return arr[Idx];
}

void run_array() {
  array<int, 5> arr{1, 2, 3, 4, 5};
  for (auto&& elem : arr) {
    std::print("{} ", elem);
  }
  std::println();

  const auto [a, b, c, d, e] = arr;
  std::println("sum(arr) = ", a + b + c + d + e);
}

int main() {
  run_array();
  return 0;
}
