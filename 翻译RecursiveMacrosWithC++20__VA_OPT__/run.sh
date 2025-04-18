#!/bin/bash
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author  :   xuechengyun
# E-mail  :   xuechengyunxue@gmail.com
# Date    :   2025/04/18 15:14:46
# Desc    :
########################################################################

# set -x
CUR_DIR=$(cd `dirname $0`; pwd)
cd ${CUR_DIR}

main_name=variable_argument_for_each_main
c++ $main_name.cc -o $main_name -std=c++23 && ./$main_name > $main_name.stdout

cd -
