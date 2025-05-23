/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/23 15:31:48
# Desc   :
########################################################################
*/
#pragma once


namespace space4 {

template <typename T>
class TestTemp {
public:
  TestTemp();
  void set_value(T obj);
  T get_value();
private:
  T obj_;
};  // class TestTemp

}  // namespace space4

#include "test_temp_4.h"
