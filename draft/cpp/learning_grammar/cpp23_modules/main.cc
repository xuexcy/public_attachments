/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/10/09 15:50:54
# Desc   :
########################################################################
refs:
  http://www.purecpp.cn/detail?id=2447
  https://www.cnblogs.com/xiaohaigegede/p/19025505
*/

import std;

import core;
import data;
import net;

import hello_world;

void run() {
  UserManager um;
  const UserId id1{1001};
  const UserId id2{1002};
  um.add_user(id1, "Alice");
  um.add_user(id2, "Bob");

  Client client(um);
  std::println("{}", client.fetch_username(id1));
  std::println("{}", client.fetch_username(UserId{999}));
}

int main() {
  std::println("{}", hello());
  hello_world::global_data = 100;
  hello_world::say_hello();

  run();
  return 0;
}
