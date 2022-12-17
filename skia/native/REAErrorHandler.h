/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once
#include "ErrorHandler.h"
#include "Scheduler.h"

namespace reanimated {

class REAErrorHandler : public ErrorHandler {
 public:
  REAErrorHandler(std::shared_ptr<Scheduler> scheduler);
  virtual ~REAErrorHandler() {}

  std::shared_ptr<Scheduler> getScheduler() override;
  std::shared_ptr<ErrorWrapper> getError() override;
  void setError(std::string message) override;

 private:
  void raiseSpec() override;
  std::shared_ptr<Scheduler> scheduler_;
  std::shared_ptr<ErrorWrapper> error_;
};

}// namespace reanimated
