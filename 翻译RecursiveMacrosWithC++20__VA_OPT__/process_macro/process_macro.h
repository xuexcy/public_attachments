/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/04/01 14:09:57
# Desc   :
########################################################################
*/
#pragma once

#include <compare>
#include <cstdint>
#include <format>
#include <memory>
#include <numeric>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "context.h"
#include "macro.h"
#include "utils.h"

namespace process_macro {

std::string expand(Context* context);
/**
 * @brief 处理宏展开后产生的新的 tokens
 *
 * @param tokens 处理 token 后产生的新的 tokens
 * @param rest 输入中未处理的部分
 * @return std::pair<std::string, View>
 */
void process_tokens(Context *context);
// std::pair<std::string, View> process_tokens(Context* context);

std::tuple<StartIdx, Length, MacroPtr> get_a_macro(const Seq& input);

/**
 * @brief 扫描 input 并输出一个合法标识符的起始下标和长度，
 *    如果没有这样一个标识符，则 output.first == std::string::npos
 *
 * @param input
 * @return std::pair<
 *    size_t : 标识符的起始下标
 *    size_t : 标识符的长度>
e.g.
  input: "OL45"
  output: <0, 2>

  input: " 123 FL(6)789"
  output: <5, 2>
 */

/**
 * @brief 处理当前这个 token, 得到新的 tokens
 *
 * @param token : 从输入流中扫描到的一个 token
 * @param rest : 输入流中未处理的部分,以提供宏需要的参数
 * @return std::pair<
 *    std::string : 处理完 token 后得到的新 tokens,
 *    View : 输入流中未处理的部分>
e.g.
  input: OL, "45"
  output: <"123", "45">

  input: FL, "(6)789"
  output: <"((6) + 1)", "789">
 */
void process_macro(Token token, MacroPtr macro, Context* context);

/**
 * @brief 对宏展开一次,包含读取和处理参数,并使用处理后的参数对宏的替换进行替换
 *
 * @param macro
 * @param rest
 * @return std::pair<
      std::string : 宏展开一次得到的新 tokens,
      View : 输入流中未处理的部分>
e.g.
  input: OL, "45"
  output: "123" "45"

  input: FL, "(FL(6))789"
  output: <"((((6) + 1)) + 1)", "789">
 */
Seq::consumed_type expand_macro_once(MacroPtr macro, Context* context);

/*
 */

/**
 * @brief 扫描参数
 *
 * @param rest
 * @return std::pair<
      std::vector<View> : 参数列表,
      View : 输入流中未处理的部分>
e.g.
  input: "(6,7)89"
  output: <{"6", "7"}, "789">
 */
// std::pair<std::vector<View>, size_t> scan_arguments(View rest);
// std::pair<std::vector<std::pair<View, size_t>>, size_t> scan_arguments(View rest);
std::optional<std::vector<std::pair<Seq, Seq>>> scan_arguments(Context* context);

// std::tuple<View, size_t, size_t> scan_argument(View rest);
std::optional<std::pair<Seq, Seq>> scan_argument(Context* context);

}  // namespace process_macro
