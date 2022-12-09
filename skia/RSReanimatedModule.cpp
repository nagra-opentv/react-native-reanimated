/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include <cxxreact/JsArgumentHelpers.h>
#include <cxxreact/Instance.h>

#include "ReactSkia/utils/RnsLog.h"

#include "native/RSProxy.h"
#include "RSReanimatedModule.h"

namespace facebook {
namespace xplat {

using namespace folly;
using namespace reanimated;

RSReanimatedModule::RSReanimatedModule() {
  //TODO Nodemanager handle create
}

RSReanimatedModule::~RSReanimatedModule() {
  //TODO Nodemanager handle delete
}

auto RSReanimatedModule::getConstants() -> std::map<std::string, folly::dynamic> {
  return {};
}

std::string RSReanimatedModule::getName() {
  return "ReanimatedModule";
}

auto RSReanimatedModule::getMethods() -> std::vector<Method> {
  return {
      Method(
          "createNode",
          [] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::createNode invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "dropNode",
          [] (dynamic args) {
            if(args.size() !=1) {
              RNS_LOG_ERROR("RSReanimatedModule::dropNode invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "getValue",
          [] (dynamic args,CxxModule::Callback cb) {
            if(args.size() !=1) {
              RNS_LOG_ERROR("RSReanimatedModule::getValue invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "setValue",
          [] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::setValue invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "connectNodes",
          [] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::connectNodes invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "disconnectNodes",
          [] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::disconnectNodes invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "connectNodeToView",
          [] (dynamic args){
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::connectNodeToView invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "disconnectNodeFromView",
          [] (dynamic args){
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::disconnectNodeFromView invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "attachEvent",
          [] (dynamic args) {
            if(args.size() !=3) {
              RNS_LOG_ERROR("RSReanimatedModule::attachEvent invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "detachEvent",
          [] (dynamic args) {
            if(args.size() !=3) {
              RNS_LOG_ERROR("RSReanimatedModule::detachEvent invalid args");
              return;
            }
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "configureProps",
          [] (dynamic args) {
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "addListener",
          [] (dynamic args) {
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "removeListeners",
          [] (dynamic args) {
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "removeAllListeners",
          [] (dynamic args) {
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "animateNextTransition",
          [] (dynamic args) {
            RNS_LOG_NOT_IMPL;
            return;
          }),
      Method(
          "installTurboModule",
          [this] (dynamic args) {
            auto instance = getInstance().lock();
            if(instance == nullptr) {
              RNS_LOG_ERROR("instance is not valid !!!");
              return;
            }
            reanimated::createNativeReanimatedModule(instance.get());
            return;
          }),
      Method(
          "triggerRender",
          [] (dynamic args) {
            RNS_LOG_NOT_IMPL;
            return;
          }),
  };
}

void RSReanimatedModule::sendEventWithName(std::string eventName, folly::dynamic eventData) {
  auto instance = getInstance().lock();
  if(instance) {
    instance->callJSFunction(
            "RCTDeviceEventEmitter", "emit",
            (eventData != nullptr) ?
            folly::dynamic::array(folly::dynamic::array(eventName),eventData):
            folly::dynamic::array(eventName));
  }
}


#ifdef __cplusplus
extern "C" {
#endif

RNS_EXPORT_MODULE(RSReanimatedModule)

#ifdef __cplusplus
}
#endif


}// namespace xplat
}// namespace facebook
