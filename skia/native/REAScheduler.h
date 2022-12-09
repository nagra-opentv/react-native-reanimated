/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include <stdio.h>

#include <ReactCommon/CallInvoker.h>

#include "ReactSkia/utils/RnsLog.h"
#include "rns_shell/platform/linux/TaskLoop.h"

#include "Scheduler.h"

namespace reanimated {

class REAScheduler : public Scheduler {
 public:
  REAScheduler(std::shared_ptr<facebook::react::CallInvoker> jsInvoker);
  virtual ~REAScheduler();
  void scheduleOnUI(std::function<void()> job) override;

 private:
  void schedulerTaskThread();
  std::unique_ptr<RnsShell::TaskLoop> schedulerTaskRunner_{nullptr};
  std::thread schedulerThread_;
};

} // namespace reanimated

