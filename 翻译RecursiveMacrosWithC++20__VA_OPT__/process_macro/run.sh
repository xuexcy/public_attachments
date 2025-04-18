#!/bin/bash
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyunxue@gmail.com
# Date    :   2025/04/03 14:02:56
# Desc    :
########################################################################

# set -x
CUR_DIR=$(cd `dirname $0`; pwd)
cd ${CUR_DIR}

input=(
main.cc
context.cpp
macro.cpp
mini_seq.cpp
process_macro.cpp
seq.cpp
token.cpp
user_api.cpp
utils.cpp
)

output=main
clang++ ${input[@]} -std=c++23 -o $output
if [[ $? == 0 ]];then
  ./$output
fi


cd -
