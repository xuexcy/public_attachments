#!/bin/bash
########################################################################
#
# Copyright (c) 2325 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2325/11/05 14:31:54
# Desc   :
########################################################################

# set -x
CUR_DIR=$(cd `dirname $0`; pwd)
cd ${CUR_DIR}
mkdir -p build

cxx=g++

# ubuntu 25.04
# gcc version = 15.2.0
# 编译 std 模块, 请忽略链接器提示
$cxx -std=c++23 -fmodules -fsearch-include-path bits/std.cc

# 编译 core
$cxx -std=c++23 -fmodules -c core/base.cppm -o build/base.o
$cxx -std=c++23 -fmodules -c core/utils.cppm -o build/utils.o
$cxx -std=c++23 -fmodules -c core/core.ixx -o build/core.o

# 编译 data
$cxx -std=c++23 -fmodules -c data/user.cppm -o build/user.o
$cxx -std=c++23 -fmodules -c data/data.ixx -o build/data.o

# 编译 net
$cxx -std=c++23 -fmodules -c net/client.cppm -o build/client.o
$cxx -std=c++23 -fmodules -c net/net.ixx -o build/net.o

$cxx -std=c++23 -fmodules -c hello_world.cppm -o build/hello_world.o

# 编译 main
$cxx -std=c++23 -fmodules main.cc -o build/main \
    build/core.o build/base.o build/utils.o \
    build/data.o build/user.o \
    build/net.o build/client.o \
    build/hello_world.o


