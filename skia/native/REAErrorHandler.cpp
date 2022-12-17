/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "ReactSkia/utils/RnsLog.h"

#include "REAErrorHandler.h"

namespace reanimated {

REAErrorHandler::REAErrorHandler(std::shared_ptr<Scheduler> scheduler) {
  scheduler_ = scheduler;
  error_ = std::make_shared<ErrorWrapper>();
}

void REAErrorHandler::raiseSpec() {
  if (error_->handled) {
    return;
  }
  RNS_LOG_ERROR(error_->message.c_str());
  error_->handled = true;
}

std::shared_ptr<Scheduler> REAErrorHandler::getScheduler() {
  return scheduler_;
}

std::shared_ptr<ErrorWrapper> REAErrorHandler::getError() {
  return error_;
}

void REAErrorHandler::setError(std::string message) {
  if (error_->handled) {
    error_->message = message;
    error_->handled = false;
  }
}

}// namespace reanimated
