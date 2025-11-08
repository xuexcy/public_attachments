/**
########################################################################
#
# Copyright (c) 2025 xx.com, Inc. All Rights Reserved
#
########################################################################
# Author : xuechengyun
# E-mail : xuechengyunxue@gmail.com
# Date   : 2025/10/09 16:25:28
# Desc   :
########################################################################
*/

export module core:utils;
import :base;

import std;

export Status to_status(int code) {
  switch (code) {
    case 0: return Status::Success;
    case 1: return Status::Error;
    case 2: return Status::NotFound;;
    default: return Status::Error;
  }
}

export std::string to_string(Status s) {
  switch (s) {
    case Status::Success: return "Success";
    case Status::Error: return "Error";
    case Status::NotFound: return "NotFound";
    default: return "Unknown";
  }
}
