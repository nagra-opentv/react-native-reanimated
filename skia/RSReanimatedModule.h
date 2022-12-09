/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#pragma once
#include <cxxreact/CxxModule.h>

#include "ReactSkia/utils/RnsUtils.h"

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
};

#ifdef __cplusplus
extern "C" {
#endif

RNS_USED xplat::module::CxxModule* RSReanimatedModuleCls(void);	

#ifdef __cplusplus
}
#endif

}// namespace xplat
}// namespace facebook

