/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/10/09 16:28:27
# Desc   :
########################################################################
*/

export module data:user;

import std;

import core;

namespace detail {
  std::unordered_map<UserId, std::string> users;
}

export class UserManager {
public:
  Status add_user(UserId id, std::string name) {
    if (detail::users.contains(id)) {
      return Status::Error;
    }
    detail::users[id] = std::move(name);
    return Status::Success;
  }

  Status get_user(UserId id, std::string* out_name) {
    if (auto it = detail::users.find(id); it != detail::users.end()) {
      *out_name = it->second;
      return Status::Success;
    } else {
      return Status::NotFound;
    }
  }
};  // class UserManager
