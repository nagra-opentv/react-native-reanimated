/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "ReactSkia/utils/RnsLog.h"

#include "REALogger.h"

namespace reanimated {

std::unique_ptr<LoggerInterface> Logger::instance = std::make_unique<REALogger>();

void REALogger::log(const char* str) {
  RNS_LOG_INFO(str);
}

void REALogger::log(double d) {
  RNS_LOG_INFO(d);
}

void REALogger::log(int i) {
  RNS_LOG_INFO(i);
}

void REALogger::log(bool b) {
  RNS_LOG_INFO((b ? "true" : "false"));
}

}// namespace reanimated
