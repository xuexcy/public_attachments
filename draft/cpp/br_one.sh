#!/bin/bash
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyun@gmail.com
# Date   : 2025/07/03 13:50:20
# Desc   :
########################################################################

# set -x
CUR_DIR=$(cd `dirname $0`; pwd)
cd ${CUR_DIR}

sh build_one.sh
status=$?
if [ $status != 0 ]; then
    exit $status
fi
sh run_one.sh
cd -






