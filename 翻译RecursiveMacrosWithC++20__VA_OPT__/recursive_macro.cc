/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/03/27 15:32:18
# Desc   :
########################################################################
*/

#include <format>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using Str = std::string;
using View = std::string_view;
template <typename C>
std::string join_string(const C& c, const std::string& delimiter) {
  return std::accumulate(
    c.begin(), c.end(), std::string(), [&](const std::string& a, const auto& b) {
      return a + (a.empty() ? "" : delimiter) + std::string(b);
    });
}

/*
name: OL
type: Macro::Type::ObjectLike
parameters: {}
sl: ""
*/
#define OL 123

/*
name: FL
type: Macro::Type::FunctionLike
parameters: {"x"}
sl: "((x)+1)"
*/
#define FL(x) ((x) + 1)

struct Macro {
  Str name;
  enum class Type { ObjectLike, FunctionLike };  // enum class Type
  Type type;
  std::vector<Str> parameters;
  Str sl;
  std::string to_string() const {
    return std::format("#define {}{} {}", name, format_parameters(), sl);
  }
private:
  std::string format_parameters() const {
    if (Type::ObjectLike == type) {
      return "";
    } else {
      return std::format("({})", join_string(parameters, ", "));
    }
  }
};  // struct Macro

std::unordered_map<Str, Macro> name_to_macro;
std::unordered_map<Str, bool> name_to_rb;
std::unordered_map<Str, bool> token_to_ub;

std::pair<Str, View> process_tokens(View tokens, View rest);
Str recursive_macro(View input_) {
  return process_tokens(input_, "").first;
}

/*
OL45
input: OL, "45"
output: "123" "45"

FL(6)789
input: FL, "(6)789"
output: <"((6)+1)", "789">
*/
// 从 rest 中读取并处理参数 + 使用处理后的参数替换宏的 substitution list
std::pair<Str, View> expand_macro_once(const Macro& macro, View rest);

/*
OL45
input: OL, "45"
output: <"123", "45">

FL(6)789
input: FL, "(6)789"
output: <"((6)+1)"", "789">
*/
// 处理当前这个 token，以及这个 token 展开后的 tokens
std::pair<Str, View> process_token(const Str& token, View rest);

/*
input: "(6,7)89"
output: <{"6", "7"}, "789">
*/
std::pair<std::vector<View>, View> scan_arguments(View rest);

/*
input: ((x)+1), x, 6
output str: ((6)+1)
*/
void replace_all(Str& str, const Str& from, const Str& to) {
  if (from.empty()) return;
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != Str::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();  // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

/*
input: macro(FL), "(6)789"
output: <{"6"}, "789">
*/
std::pair<std::vector<View>, View> get_replacement_list(const Macro& macro, View rest);

/*
input: "OL45"
output: <"OL", "45">

input: "FL(6)789"
output: <"FL", "(6)789">
*/
std::pair<View, View> read_token(View input) {
  return {"", ""};
}

std::pair<Str, View> process_tokens(View input_tokens, View rest) {
  Str result;

  while (true) {
    auto [token, mid_rest] = read_token(input_tokens);
    if (token.empty()) {
      return { result, rest};
    }
    Str tmp_rest = std::string(mid_rest) + std::string(rest);
    auto [output_tokens, new_rest] = process_token(Str(token), tmp_rest);
    result.append(output_tokens);

    if (new_rest.size() >rest.size()) {
      input_tokens = new_rest.substr(0, new_rest.size() - rest.size());
    } else {
      input_tokens = {};
      rest = new_rest;
    }
  }
}

std::pair<Str, View> process_token(const Str& token, View rest) {
  auto it = name_to_macro.find(token);
  if (it == name_to_macro.end()) {
    return {token, rest};
  }
  // 1. If T is the name of a macro for which the replacing bit is true, cpp sets the unavailable
  // bit on token T
  if (name_to_rb[token]) {
    token_to_ub[token] = true;
  }
  // TODO: 解释这两步
  if ((Macro::Type::ObjectLike == it->second.type && !token_to_ub[token]) ||
      (Macro::Type::FunctionLike == it->second.type && !token_to_ub[token] &&
      rest.size() > 0 && '(' == rest[0])) {
    // 2. If T is the name of an object-like macro and T’s unavailable bit is clear, then T is
    // expanded.
    // 3. If T is the name of a function-like macro, T’s unavailable bit is clear, and T is followed
    // by (, then T is expanded.
    auto [new_tokens, rest2] = expand_macro_once(it->second, rest);

    // Finally, cpp sets the replacing bit to true on the macro named T.
    name_to_rb[token] = true;

    // With the replacing bit true, cpp continues processing input as usual from the tokens it just added to the input list.
    // This may result in more macro expansions, so is sometimes called the rescan phase.
    auto [output_str, rest3] = process_tokens(new_tokens, rest2);

    // Once cpp has consumed all tokens generated from the substitution list, it clears the replacing bit on the macro named T.
    name_to_rb[std::string(token)] = false;

    return {output_str, rest3};
  } else {
    // If cpp decides not to macro-expand T, it simply adds T to the current output token list
    return {token, rest};
  }

}

std::pair<Str, View> expand_macro_once(const Macro& macro, View rest) {
  std::vector<Str> replacement_args;
  View new_rest = rest;
  if (Macro::Type::FunctionLike == macro.type) {
    // When T is a function-like macro
    auto [replacement_args, tmp_rest] = get_replacement_list(macro, rest);
    new_rest = tmp_rest;
  }
  // Cpp takes T’s substitution list
  Str replaced_res{macro.sl};
  // if T had arguments, replaces any occurrences of parameter names with the corresponding argument
  // token lists computed in step 1
  // TODO 不能直接替换
  for (size_t idx{0}; idx < macro.parameters.size(); ++idx) {
    replace_all(replaced_res, macro.parameters[idx], replacement_args[idx]);
  }
  // It also performs stringification and pasting as indicated by # and ## in the substitution list.
  // TODO
  // replaced_res = process_hashtag(replaced_res);

  // It then logically prepends the resulting tokens to the input list.
  return {replaced_res, new_rest};
}

std::pair<std::vector<View>, View> get_replacement_list(const Macro& macro, View rest) {
  // cpp scans all of the arguments supplied to T
  auto [args, new_rest] = scan_arguments(rest);
  if (args.size() != macro.parameters.size()) {
    // if T is called with an invalid number of arguments, then the program is ill-formed.
    throw std::format(
      "ERROR: T is called with an invalid number of arguments, then the program is ill-formed.\n"
      "Macro: {}\nArguments: ({})\nSourceCode: {}", macro.to_string(), join_string(args, ", "), rest);
  }
  std::vector<View> new_args;
  for (auto& arg : args) {
    // and performs macro expansion on them
    // but instead of placing output tokens in the main preprocessor output,
    // it builds a replacement token list for each of T’s arguments
    new_args.emplace_back(recursive_macro(arg));
  }
  return {new_args, new_rest};
}
