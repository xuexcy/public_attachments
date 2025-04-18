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

output=main
clang++ main.cc context.cpp macro.cpp process_macro.cpp mini_seq.cpp seq.cpp token.cpp user_api.cpp utils.cpp -std=c++23 -o $output
#clang++ main.cc macro.cpp context.cpp user_api.cpp utils.cpp -std=c++23 -o $output
if [[ $? == 0 ]];then
  ./$output
fi


cd -
