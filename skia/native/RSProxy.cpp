/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include <jsi/JSCRuntime.h>
#include <ReactCommon/CallInvoker.h>

#include "ReactSkia/sdk/FollyTimer.h"
#include "ReactSkia/utils/RnsLog.h"

#include "NativeReanimatedModule.h"

#include "native/REAScheduler.h"
#include "native/REAErrorHandler.h"
#include "native/RSProxy.h"

namespace reanimated {

using namespace facebook;

void createNativeReanimatedModule(react::Instance* instance) {
  if(instance == nullptr) {
    RNS_LOG_ERROR("Instance handle not valid,cannot create native module!!");
    return;
  }

  std::shared_ptr<react::CallInvoker> jsCallInvoker = instance->getJSCallInvoker();
  jsi::Runtime* jsRuntime = reinterpret_cast<jsi::Runtime*>(instance->getJavaScriptContext());

  if((jsCallInvoker.get() == nullptr) || (jsRuntime == nullptr)){
    RNS_LOG_ERROR("js handle not valid,cannot create native module!!");
    return;
  }

  std::shared_ptr<Scheduler> scheduler = std::make_shared<REAScheduler>(jsCallInvoker);
  std::unique_ptr<jsi::Runtime> animatedRuntime = jsc::makeJSCRuntime();
  std::shared_ptr<ErrorHandler> errorHandler = std::make_shared<REAErrorHandler>(scheduler);

  auto propObtainer = [](jsi::Runtime &rt, const int viewTag, const jsi::String &propName) -> jsi::Value {
    RNS_LOG_TODO("propObtainer obtain props for viewTag:" << viewTag << " propName:" << propName.utf8(rt).c_str());
    return jsi::Value::undefined();

    //NSString* propNameConverted = [NSString stringWithFormat:@"%s",propName.utf8(rt).c_str()];
    //  std::string resultStr = std::string([[reanimatedModule.nodesManager obtainProp:[NSNumber numberWithInt:viewTag] propName:propNameConverted] UTF8String]);
    //  jsi::Value val = jsi::String::createFromUtf8(rt, resultStr);
    //  return val;
  };

  auto requestRender = [](std::function<void(double)> onRender, jsi::Runtime &rt) {
    RNS_LOG_TODO("requestRender execute onRender task");

    //[reanimatedModule.nodesManager postOnAnimation:^(CADisplayLink *displayLink) {
    //  double frameTimestamp = displayLink.targetTimestamp * 1000;
    //  rt.global().setProperty(rt, "_frameTimestamp", frameTimestamp);
    //  onRender(frameTimestamp);
    //  rt.global().setProperty(rt, "_frameTimestamp", jsi::Value::undefined());
    //}];
  };

  auto propUpdater = [](jsi::Runtime &rt, int viewTag, const jsi::Value& viewName, const jsi::Object &props) -> void {
    RNS_LOG_TODO("propUpdater updateProps for viewTag:" << viewTag);

    //NSString *nsViewName = [NSString stringWithCString:viewName.asString(rt).utf8(rt).c_str() encoding:[NSString defaultCStringEncoding]];
    //NSDictionary *propsDict = convertJSIObjectToNSDictionary(rt, props);
    //[reanimatedModule.nodesManager updateProps:propsDict ofViewWithTag:[NSNumber numberWithInt:viewTag] withName:nsViewName];
  };

  auto scrollToFunction = [](int viewTag, double x, double y, bool animated) {
    RNS_LOG_TODO("scrollTo viewTag:" << viewTag << " x:" << x << " y:" << y << " animated:"<< animated);

    //scrollTo(viewTag, uiManager, x, y, animated);
  };

  auto measuringFunction = [](int viewTag) -> std::vector<std::pair<std::string, double>> {
    RNS_LOG_TODO("measuringFunction provide measure for viewTag:" << viewTag);
    std::vector<std::pair<std::string, double>> result;
    result.push_back({"x", 0});
    result.push_back({"y", 0});

    result.push_back({"width", 0});
    result.push_back({"height", 0});

    result.push_back({"pageX", 0});
    result.push_back({"pageY", 0});
    return result;
  };

  auto getCurrentTime = []() {
    return rns::sdk::Timer::getCurrentTimeSecs();
  };

  PlatformDepMethodsHolder platformDepMethodsHolder = {
    requestRender,
    propUpdater,
    scrollToFunction,
    measuringFunction,
    getCurrentTime,
  };

  std::shared_ptr<NativeReanimatedModule> module = std::make_shared<NativeReanimatedModule>(jsCallInvoker,
                                                         scheduler,
                                                         std::move(animatedRuntime),
                                                         errorHandler,
                                                         propObtainer,
                                                         platformDepMethodsHolder);

  scheduler->setRuntimeManager(module);

  jsRuntime->global().setProperty(
        *jsRuntime,
        jsi::PropNameID::forAscii(*jsRuntime, "__reanimatedModuleProxy"),
        jsi::Object::createFromHostObject(*jsRuntime, module));

  return;
}

}// namespace reanimated
