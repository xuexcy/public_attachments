/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/10/09 16:24:11
# Desc   :
########################################################################
*/

export module core:base;

import std;

export enum class Status {
  Success,
  Error,
  NotFound
};  // enum class Status

export struct UserId {
  unsigned int value;
  bool operator==(const UserId&) const = default;
};  // struct UserId

namespace std {
template <>
struct hash<UserId> {
  size_t operator()(const UserId& user_id) const noexcept {
    return std::hash<decltype(UserId::value)>()(user_id.value);
  }
};
}
