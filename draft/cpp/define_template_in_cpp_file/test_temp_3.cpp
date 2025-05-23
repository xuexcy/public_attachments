/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/05/23 14:54:41
# Desc   :
########################################################################
*/

#include "test_temp_3.h"

#include "cpp_utils/util.h"

namespace space3 {

template <typename T>
TestTemp<T>::TestTemp() {
  PRINT_CURRENT_FUNCTION_NAME;
}

template <typename T>
void TestTemp<T>::set_value(T obj) {
  PRINT_CURRENT_FUNCTION_NAME;
  obj_ = obj;
}

template <typename T>
T TestTemp<T>::get_value() {
  PRINT_CURRENT_FUNCTION_NAME;
  return obj_;
}
}  // namespace space3
