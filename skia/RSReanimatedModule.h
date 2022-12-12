/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#pragma once
#include <cxxreact/CxxModule.h>

#include "ReactSkia/LegacyNativeModules/uimanager/UiManagerModule.h"
#include "ReactSkia/utils/RnsUtils.h"

#include "RSNodeManager.h"

namespace facebook {
namespace xplat {

class RSReanimatedModule : public module::CxxModule {
 public:
  RSReanimatedModule();
  ~RSReanimatedModule();
  virtual auto getConstants() -> std::map<std::string, folly::dynamic>;
  virtual auto getMethods() -> std::vector<Method>;
  std::string getName();

  void sendEventWithName(std::string eventName , folly::dynamic eventData);

  uimanager::UimanagerModule* getUIManager();

 private:
  reanimated::RSNodeManager* nodeManager_{nullptr};
  uimanager::UimanagerModule* uiManager_{nullptr};
};

}// namespace xplat
}// namespace facebook

