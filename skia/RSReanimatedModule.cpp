/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include <cxxreact/JsArgumentHelpers.h>
#include <cxxreact/Instance.h>

#include "ReactSkia/ReactSkiaApp.h"
#include "ReactSkia/utils/RnsLog.h"

#include "native/RSProxy.h"
#include "RSNodeManager.h"
#include "RSReanimatedModule.h"

namespace facebook {
namespace xplat {

using namespace folly;
using namespace reanimated;


RSReanimatedModule::RSReanimatedModule() {
  nodeManager_ = new RSNodeManager(this);
}

RSReanimatedModule::~RSReanimatedModule() {
  if(nodeManager_) {
    delete nodeManager_;
  }
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
          [this] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::createNode invalid args");
              return;
            }
            nodeManager_->createNode(args[0].asInt(), args[1]);
            return;
          }),
      Method(
          "dropNode",
          [this] (dynamic args) {
            if(args.size() !=1) {
              RNS_LOG_ERROR("RSReanimatedModule::dropNode invalid args");
              return;
            }
            nodeManager_->dropNode(args[0].asInt());
            return;
          }),
      Method(
          "getValue",
          [this] (dynamic args,CxxModule::Callback cb) {
            if(args.size() !=1) {
              RNS_LOG_ERROR("RSReanimatedModule::getValue invalid args");
              return;
            }
            nodeManager_->getValue(args[0].asInt(), cb);
            return;
          }),
      Method(
          "setValue",
          [this] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::setValue invalid args");
              return;
            }
            nodeManager_->setValue(args[0].asInt(), args[1].asInt());
            return;
          }),
      Method(
          "connectNodes",
          [this] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::connectNodes invalid args");
              return;
            }
            nodeManager_->connectNodes(args[0].asInt(), args[1].asInt());
            return;
          }),
      Method(
          "disconnectNodes",
          [this] (dynamic args) {
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::disconnectNodes invalid args");
              return;
            }
            nodeManager_->disconnectNodes(args[0].asInt(), args[1].asInt());
            return;
          }),
      Method(
          "connectNodeToView",
          [this] (dynamic args){
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::connectNodeToView invalid args");
              return;
            }
            nodeManager_->connectNodeToView(args[0].asInt(), args[1].asInt());
            return;
          }),
      Method(
          "disconnectNodeFromView",
          [this] (dynamic args){
            if(args.size() !=2) {
              RNS_LOG_ERROR("RSReanimatedModule::disconnectNodeFromView invalid args");
              return;
            }
            nodeManager_->disconnectNodeFromView(args[0].asInt(), args[1].asInt());
            return;
          }),
      Method(
          "attachEvent",
          [this] (dynamic args) {
            if(args.size() !=3) {
              RNS_LOG_ERROR("RSReanimatedModule::attachEvent invalid args");
              return;
            }
            nodeManager_->attachEvent(args[0].asInt(), args[1].asString(), args[2].asInt());
            return;
          }),
      Method(
          "detachEvent",
          [this] (dynamic args) {
            if(args.size() !=3) {
              RNS_LOG_ERROR("RSReanimatedModule::detachEvent invalid args");
              return;
            }
            nodeManager_->detachEvent(args[0].asInt(), args[1].asString(), args[2].asInt());
            return;
          }),
      Method(
          "configureProps",
          [this] (dynamic args) {
            nodeManager_->configureProps(jsArgAsArray(args,0),jsArgAsArray(args,1));
            return;
          }),
      Method(
          "addListener",
          [] (dynamic args) {
            RNS_LOG_TODO("RSReanimatedModule::addListener:" << args[0]);
            return;
          }),
      Method(
          "removeListeners",
          [] (dynamic args) {
            RNS_LOG_TODO("RSReanimatedModule::removeListeners: " << args[0]);
            return;
          }),
      Method(
          "removeAllListeners",
          [] (dynamic args) {
            RNS_LOG_TODO("RSReanimatedModule::removeAllListeners");
            return;
          }),
      Method(
          "animateNextTransition",
          [] (dynamic args) {
            RNS_LOG_TODO("RSReanimatedModule::animateNextTransition");
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
          [this] (dynamic args) {
            nodeManager_->triggerRender();
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

uimanager::UimanagerModule* RSReanimatedModule::getUIManager() {
  if(uiManager_ == nullptr) {
    auto instance = getInstance().lock();
    if(instance) {
      uiManager_ = static_cast<uimanager::UimanagerModule*>(ReactSkiaApp::currentBridge()->moduleForName("UIManager"));
    }
  }
  return uiManager_;
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
