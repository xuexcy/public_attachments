/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/10/09 15:51:01
# Desc   :
########################################################################
*/
export module hello_world;

import std;

export namespace hello_world {
  int global_data;
  void say_hello() {
    std::println("Hello module! Data is {}", global_data);
  }
}  // namespace hello_world

export auto hello() {
  return "Hello module! C++ 20!";
}
