/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/10/09 16:32:42
# Desc   :
########################################################################
*/

export module net:client;

import std;

import core;
import data;

export class Client {
private:
  UserManager& user_manager;
public:
  Client(UserManager& um) : user_manager(um) {}
  std::string fetch_username(UserId id) {
    std::string name;
    const auto status = user_manager.get_user(id, &name);
    if (status == Status::Success) {
      return std::format("User {}", std::to_string(id.value));
    }
    return std::format("Error: {}", to_string(status));
  }
};  // class Client

