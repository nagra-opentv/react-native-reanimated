/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "ReanimatedHiddenHeaders.h"

namespace reanimated {

class REALogger : public LoggerInterface {
 public:
  void log(const char* str) override;
  void log(double d) override;
  void log(int i) override;
  void log(bool b) override;
  virtual ~REALogger() {}
};

}// namespace reanimated
