/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/06 19:33:09
# Desc   : stmp: https://zhuanlan.zhihu.com/p/646752343
                 https://zhuanlan.zhihu.com/p/646812253
########################################################################
状态元编程 Stateful Template Meta Programming
*/

#include <algorithm>
#include <cassert>
#include <print>


// __COUNTER__ 宏一个宏
constexpr auto a  = __COUNTER__;
constexpr auto b  = __COUNTER__;
constexpr auto c  = __COUNTER__;
/*
预处理: gcc -E main.cc
得到:
constexpr auto a  = 0;
constexpr auto b  = 1;
constexpr auto c  = 2;
*/
static_assert(a == 0 && b == 1 && c == 2);

// 是否是一个完整类
// C++ 标准规定 sizdof 只能对完整类型使用
template <typename T>
constexpr inline bool is_complete_v = requires { sizeof(T); };

struct X;
static_assert(!is_complete_v<X>);
struct X {};
// 这个断言会失败，因为编译器缓存了第一次实例化的结果
// static_assert(is_complete_v<X>);

struct Y;
static_assert(!is_complete_v<Y>);
struct Y {};
static_assert(!is_complete_v<Y>);

struct Z;
struct Z {};
static_assert(is_complete_v<Z>);

// 用 lambda 表达式作为非类型模板参数(NTTP, Non Type Template Parameter)
template <auto seed = [] {}>
void test() {
  std::println("{}", typeid(seed).name());
}
void run_test() {
  // 可以看到，seed 的 id 每次都在递增
  test();  // UlvE_
  test();  // UlvE0_
  test();  // UlvE1_
  test();  // UlvE2_
}

template <typename T, auto seed = [] {}>
constexpr inline bool is_complete_v_2 = requires { sizeof(T); };
struct A;
static_assert(!is_complete_v_2<A>);
struct A {};
static_assert(is_complete_v_2<A>);

// 检测 foo 是否有定义
template <auto seed = [] {}>
constexpr inline bool is_foo_complete_v = requires { foo(seed); };
auto foo(auto);
static_assert(!is_foo_complete_v<>);
auto foo(auto value) { return sizeof(value); }
static_assert(is_foo_complete_v<>);

// 友元注入 friend injection: 实例化类模板的同时实例化友元函数，给其他位置声明的函数添加定义
template <auto seed = [] {}>
constexpr inline bool is_bar_complete_v = requires { bar(seed); };
auto bar(auto);
template <typename T>
struct B {
  friend auto bar(auto value) { return sizeof(value); }
};  // struct B
static_assert(!is_bar_complete_v<>);
B<void> bb;  // 实例化 B<void> 后就有了 bar 的定义
static_assert(is_bar_complete_v<>);


// 实例化一个编译时的开关
auto flag(auto);
template <auto value>
struct Setter {
  friend auto flag(auto) {}
};  // struct Setter

// 有点像运行时使用 static 变量来的 init_once
template <auto N = 0, auto seed = [] {}>
consteval auto value() {
  constexpr bool exist = requires { flag(N); };
  if constexpr (!exist) {
    Setter<exist> setter;
  }
  return exist;
}
bool init_once() {
  static bool initted{false};
  struct cleanup {
    ~cleanup() {
      if (!initted) {
        initted = true;
      }
    }
  } dummy ;
  return initted;
  /*
  auto res = initted;
  if (!initted) {
    initted = true;
  }
    return res;
  */
}

void run_value() {
  constexpr auto a = value();
  constexpr auto b = value();
  static_assert(a != b);
  auto c = init_once();
  auto d = init_once();
  assert(c != d);
}

auto flag2(auto);
template <int N>
struct Reader {
  friend auto flag2(Reader);
};  // struct Reader
template <int N>
struct Setter2 {
  friend auto flag2(Reader<N>) {}
};  // struct Setter2
template <int N = 0, auto seed = [] {}>
consteval auto next() {
  constexpr bool exist = requires { flag2(Reader<N>{}); };
  if constexpr (!exist) {
    Setter2<N> setter;
    return N;
  } else {
    return next<N + 1>();
  }
}

void run_next() {
  // 1. 实例化 Reader<0>
  // 2. 没有 flag2(Reader<0>) 的定义, exist = false
  // 3. 实例化 Setter2<0>, 定义 flag2(Reader<0>)
  // 4. return 0
  constexpr auto a = next();
  // 1. 有 flag2(Reader<0>) 的定义, exist = true
  // 2. return next<N+1>()
  //      a. 实例化 Reader<1>
  //      b. 没有 flag2(Reader<1>) 的定义, exist = false
  //      c. 实例化 Setter2<1>, 定义 flag2(Reader<1>)
  //      d. return 1
  constexpr auto b = next();
  constexpr auto c = next();
  static_assert(a == 0 && b == 1 && c == 2);
}

class Bank {
private:
  double money = 999;
public:
  void check() const {
    std::println("money: {}", money);
  }
};  // class Bank

template <auto mp>
struct Thief {
  friend double& steal(Bank& bank) { return bank.*mp; }
};  // struct Thief
double& steal(Bank& bank);

// 显示实例化
// 类权限访问说明符 private/public/protected 只作用域编译期检查
// 在编译期检查后，我们可以模板实例化来获取 private 成员
template struct Thief<&Bank::money>;
void run_steal() {
  Bank bank;
  bank.check();
  steal(bank) = 100;
  bank.check();
}

struct identity {
  int size;
};  // struct identity
using meta_value = const identity*;
template <typename T>
struct storage {
  constexpr inline static identity value = { sizeof(T) };
};  // struct storage
// 通过 T 得到一个实例化的 storage 的成员 value 的地址
template <typename T>
consteval meta_value value_of() {
  return &storage<T>::value;
}

template <meta_value value>
struct type_of;
// 将 storage<int> 的成员 value 的地址与 type = int 绑定
// 这要求我们提前特化好所有要使用的类型
template <>
struct type_of<value_of<int>()> {
  using type = int;
};  // struct type_of<value_of<int>()>

template <typename T>
struct Self {
  using type = T;
};  // struct Self
template <meta_value value>
struct MyReader {
  friend consteval auto to_type(MyReader);
};  // struct MyReader
template <meta_value value, typename T>
struct MySetter {
  friend consteval auto to_type(MyReader<value>) {
    return Self<T>{};
  }
};  // struct MySetter
template <typename T>
consteval meta_value value_of_2() {
  constexpr auto value = &storage<T>::value;
  MySetter<value, T> setter;
  return value;
}

// 原理: 用户使用 value_of_2 时会注册 T，也就是实例化 MySetter
// 读取时调用 to_type 函数, 只要被注册过就可以得到 Self<T> (因为注册时在 MySetter 中定义了 to_type 函数)
template <meta_value value>
using type_of_2 = typename decltype(to_type(MyReader<value>{}))::type;

template <typename... Ts>
struct type_list {};
template <std::array types, typename = std::make_index_sequence<types.size()>>
struct array_to_list;
template <std::array types, std::size_t... Is>
struct array_to_list<types, std::index_sequence<Is...>> {
  using result = type_list<type_of_2<types[Is]>...>;
};

template <typename List>
struct sort_list;
template <typename... Ts>
struct sort_list<type_list<Ts...>> {
  constexpr inline static std::array sorted_types = [] {
    std::array types{ value_of_2<Ts>()... };
    std::ranges::sort(types, [](auto lhs, auto rhs) {
      return lhs->size < rhs->size;
    });
    return types;
  }();
  using result = typename array_to_list<sorted_types>::result;
};

int main() {
  run_test();
  run_value();
  run_next();
  run_steal();
  return 0;
}

