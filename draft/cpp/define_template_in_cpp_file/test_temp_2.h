/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/23 14:38:39
# Desc   :
########################################################################
*/
#pragma once


namespace space2 {

  template <typename T>
  class TestTemp {
  public:
    TestTemp();
    void set_value(T obj);
    T get_value();
  private:
    T obj_;
  };  // class TestTemp

  }  // namespace space2
