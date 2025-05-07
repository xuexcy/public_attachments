/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/05/07 22:34:09
# Desc   : ref: https://zhuanlan.zhihu.com/p/101004501
########################################################################

*/

#include <compare>
#include <set>
#include <string>
#include <vector>

#include "cpp_utils/util.h"

struct InternalName {
  std::string first_name;
  std::string mid_name;
  std::string last_name;
  std::string str() const {
    return std::format("first_name: {}, mid_name: {}, last_name: {}", first_name, mid_name, last_name);
  }
};  // struct Name

namespace before_cpp11 {
struct Name : public InternalName {
  bool operator<(const Name& other) const {
    return first_name < other.first_name ||
        first_name == other.first_name && mid_name < other.mid_name ||
        first_name == other.first_name && mid_name == other.mid_name &&
            last_name < other.last_name;
  }
};  // struct Name


}  // namespace before_cpp11

namespace after_cpp11 {
struct Name : public InternalName {
  bool operator<(const Name& other) const {
    return std::tie(first_name, mid_name, last_name) <
        std::tie(other.first_name, other.mid_name, other.last_name);
  }
};  // struct Name

}  // namespace after_cpp11

// c++20 中的三向比较 three way comparation
namespace cpp20 {
// 好像不能这么写
// struct Name : public InternalName {
//   std::strong_ordering operator<=>(const Name&) const = default;
// };  // struct Name

struct Name {
  std::string first_name;
  std::string mid_name;
  std::string last_name;
  std::string str() const {
    return std::format("first_name: {}, mid_name: {}, last_name: {}", first_name, mid_name, last_name);
  }
  std::strong_ordering operator<=>(const Name&) const = default;
};

}
template <typename T>
void run_name() {
  PRINT_CURRENT_FUNCTION_NAME;
  std::set<T> s;
  auto add = [&](auto... args) {
    s.emplace(T{args...});
  };
  add("a", "b", "c");
  add("b", "c", "d");
  add("a", "b", "d");
  for (auto& elem : s) {
    std::println("{}", elem.str());
  }
}

void run() {
  run_name<before_cpp11::Name>();
  run_name<after_cpp11::Name>();
  run_name<cpp20::Name>();
}

// strong_ordering: 严格比较 + 全部成员
// weak_ordering: 其他


struct ID {
  int id_number;
  // strong_ordering
  auto operator<=>(const ID&) const = default;
};  // struct ID

struct Person {
  ID id;
  std::string name;
  std::string email;
  // 只比较了 id, 所以是 weak_ordering
  std::weak_ordering operator<=>(const Person& other) const {
    return id <=> other.id;
  }
};  // struct Person

//成员是容器类型， operator== 单独列出来
struct SomeType {
  int i;
  std::vector<int> v;
  std::strong_ordering operator<=>(const SomeType&) const = default;
  bool operator==(const SomeType&) const = default;

};  // struct SomeType

int main() {
  run();
  return 0;
}
