/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAScheduler.h"
#include "RuntimeManager.h"

namespace reanimated {

REAScheduler::REAScheduler(std::shared_ptr<facebook::react::CallInvoker> jsInvoker) {
  this->jsCallInvoker_ = jsInvoker;
#if USE(SCHEDULER_OWN_TASKRUNNER)
  schedulerTaskRunner_ = std::make_unique<RnsShell::TaskLoop>();
  schedulerThread_= std::thread (&REAScheduler::schedulerTaskThread,this);
  pthread_setname_np(schedulerThread_.native_handle(),"REASchedulerTask");

  schedulerTaskRunner_->waitUntilRunning();
#endif
}

#if USE(SCHEDULER_OWN_TASKRUNNER)
void REAScheduler::schedulerTaskThread(){
  schedulerTaskRunner_->run();
}
#endif

void REAScheduler::scheduleOnUI(std::function<void()> job) {

  if (runtimeManager.lock() == nullptr) {
    RNS_LOG_WARN("runtimeManager not valid,cannot schedule!!");
    return;
  }

  Scheduler::scheduleOnUI(job);
  std::weak_ptr<RuntimeManager> blockRuntimeManager = runtimeManager;

#if USE(SCHEDULER_OWN_TASKRUNNER)
  schedulerTaskRunner_->dispatch([blockRuntimeManager,this]() {
    if(blockRuntimeManager.lock()) {
      triggerUI();
    } else {
      RNS_LOG_WARN("runtimeManager not valid!!");
    }
  });
#else
  //Dispatch on main task loop
  RnsShell::TaskLoop::main().dispatch([blockRuntimeManager,this]() {
    if(blockRuntimeManager.lock()) {
      triggerUI();
    } else {
      RNS_LOG_WARN("runtimeManager not valid!!");
    }
  });
#endif
}

REAScheduler::~REAScheduler(){
#if USE(SCHEDULER_OWN_TASKRUNNER)
  if(schedulerTaskRunner_->running()){
     schedulerTaskRunner_->stop();
  }
#endif
}

}// namespace reanimated

