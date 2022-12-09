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

  //TODO whether we need a seperate thread.
  //Once implementation ready,need to remove this schedulerThread
  schedulerTaskRunner_ = std::make_unique<RnsShell::TaskLoop>();
  schedulerThread_= std::thread (&REAScheduler::schedulerTaskThread,this);
  pthread_setname_np(schedulerThread_.native_handle(),"REASchedulerTask");

  schedulerTaskRunner_->waitUntilRunning();
}

void REAScheduler::schedulerTaskThread(){
  schedulerTaskRunner_->run();
}

void REAScheduler::scheduleOnUI(std::function<void()> job) {

  if (runtimeManager.lock() == nullptr) {
    RNS_LOG_WARN("runtimeManager not valid,cannot schedule!!");
    return;
  }

  Scheduler::scheduleOnUI(job);
  std::weak_ptr<RuntimeManager> blockRuntimeManager = runtimeManager;

  //FIXME Temporary dispatch of the task in schedulerTaskRunner_.
  //Need to use main taskloop,to schedule the task on main thread
  schedulerTaskRunner_->dispatch([blockRuntimeManager,this]() {
    if(blockRuntimeManager.lock()) {
      triggerUI();
    } else {
      RNS_LOG_WARN("runtimeManager not valid!!");
    }
  });
}

REAScheduler::~REAScheduler(){
  if(schedulerTaskRunner_->running()){
     schedulerTaskRunner_->stop();
  }
}

}// namespace reanimated

