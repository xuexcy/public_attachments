/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/04/01 15:29:23
# Desc   :
########################################################################
*/

/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/26 00:32:20
# Desc   : ref: https://www.scs.stanford.edu/~dm/blog/va-opt.html
########################################################################
*/

// run cmd: c++ va_opt.cc -o va_opt -std=c++23 && ./va_opt > va_opt.stdout

#include <print>
#include <string>

#define ENUM_CASE(name) \
  case name:            \
    return #name;

#define MAKE_ENUM(type, ...)                  \
  enum type { __VA_ARGS__ };                  \
  constexpr const char* to_cstring(type _e) { \
    using enum type;                          \
    switch (_e) {                             \
      FOR_EACH(ENUM_CASE, __VA_ARGS__)        \
      default:                                \
        return "unknown";                     \
    }                                         \
  }

#define STRINGIFY(...) #__VA_ARGS__
#define TOSTRING(...) STRINGIFY(__VA_ARGS__)

#define PARENS ()

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define A_EXPAND(...) A_EXPAND4(A_EXPAND4(A_EXPAND4(A_EXPAND4(__VA_ARGS__))))
#define A_EXPAND4(...) A_EXPAND3(A_EXPAND3(A_EXPAND3(A_EXPAND3(__VA_ARGS__))))
#define A_EXPAND3(...) A_EXPAND2(A_EXPAND2(A_EXPAND2(A_EXPAND2(__VA_ARGS__))))
#define A_EXPAND2(...) A_EXPAND1(A_EXPAND1(A_EXPAND1(A_EXPAND1(__VA_ARGS__))))
#define A_EXPAND1(...) __VA_ARGS__


/*
假设 x 有括号, 形如 (a, b, c)
  HAS_PAREN(x) -> CHECK_HAS_PAREN(HAS_PAREN_PROBE (a, b, c)) -> CHECK_HAS_PAREN() -> NOT_0 -> 1
假设 x 没有有括号, 形如 abc
  HAS_PAREN(x) -> CHECK_HAS_PAREN(HAS_PAREN_PROBE abc)) -> NOT_01 -> 1
*/
#define HAS_PAREN(x) CHECK_HAS_PAREN(HAS_PAREN_PROBE x)
#define HAS_PAREN_PROBE(...)
#define NOT_0 1
#define NOT_01 0
#define CHECK_HAS_PAREN(...) NOT_0##__VA_OPT__(1)

#define IIF(c) IIF_IMPL(c)
#define IIF_IMPL(c) IIF_ ## c
#define IIF_0(t, f) f
#define IIF_1(t, f) t

// 当 arg 有括号时调用 macro arg
// 当 arg 没有括号时,给它添加一个括号,即调用 macro(arg)
#define CALL_MACRO(macro, arg) IIF(HAS_PAREN(arg))(macro arg, macro(arg))

// (a,b,c) : 三个参数 {a, b, c}
// ARGS(a,b,c): 三个参数 {a, b, c}
// ARGS((a,b,c)): 一个参数 {(a,b,c)}
#define ARGS(...) (__VA_ARGS__)

#define FOR_EACH(macro, ...)                                      \
  __VA_OPT__(EXPAND(FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define FOR_EACH_HELPER(macro, a1, ...)                           \
  CALL_MACRO(macro, a1)                                                       \
  __VA_OPT__(FOR_EACH_AGAIN PARENS (macro, __VA_ARGS__))
#define FOR_EACH_AGAIN() FOR_EACH_HELPER

#define PRINT(a, b) std::println("{}+{}={}", a, b, a + b);
#define PRINT_STRING(value) std::println("value: {}", TOSTRING(value));

#define EXPAND_ADD(expand_macro, ...) __VA_OPT__(expand_macro(EXPAND_ADD_HELPER(__VA_ARGS__)))
#define EXPAND_ADD_HELPER(x, ...) x __VA_OPT__(+ EXPAND_ADD_AGAIN PARENS(__VA_ARGS__))
#define EXPAND_ADD_AGAIN() EXPAND_ADD_HELPER
#define PRINT_ADD(...) std::println("add expression: {}", TOSTRING(EXPAND_ADD(EXPAND, __VA_ARGS__)));
#define PRINT_ADD_2(...) std::println("add expression: {}", TOSTRING(EXPAND_ADD(A_EXPAND, __VA_ARGS__)));

#define PRINT_HELLO() std::println("hello");
#define PRINT_HELLO_2 std::println("hello2");

#define OL 123
#define FL(x, y) ((x) + (y))

// #define ID(arg) arg
// #define ID2(arg) ID
// #define LPAREN (
// #define ID3(x) xx
// #define O ID(ID2)(ID)(X)
// #define P ID(ID2 LPAREN)ID)(X)
// #define Q ID(ID2 LPAREN I)D)(X)
// #define R ID(ID2 LPAREN ID))(X)
//
// #define S ID(ID ID2 LPAREN)ID)(X)
// #define Y ID(ID ID2 LPAREN I)D)(X)
// #define Z ID(ID ID2 LPAREN ID))(X)
// replace

int main() {
  std::println("{}", TOSTRING(FL( 1 2 , 3 )));
  //std::println("{}", TOSTRING(P));
  //std::println("{}", TOSTRING(Q));
  //std::println("{}", TOSTRING(R));
  //std::println();
  //std::println("{}", TOSTRING(S));
  //std::println("{}", TOSTRING(Y));
  //std::println("{}", TOSTRING(Z));
}
