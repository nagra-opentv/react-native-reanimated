/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#pragma once

#include <better/map.h>
#include <cxxreact/CxxModule.h>
#include "ReactSkia/utils/RnsUtils.h"

namespace facebook {
namespace xplat {

class RSReanimatedModule;

}// namespace xplat
}// namespace facebook

namespace reanimated {

class REANode;
class REAFinalNode;

class RSNodeManager {
 public:
  RSNodeManager(facebook::xplat::RSReanimatedModule* module);
  ~RSNodeManager();
  //Functions exposed to ReanimatedModule
  void createNode(int nodeID,folly::dynamic nodeConfig);
  void dropNode(int nodeID);
  void getValue(int nodeID,facebook::xplat::module::CxxModule::Callback callback);
  void connectNodes(int parentID,int childID);
  void disconnectNodes(int parentID,int childID);
  void connectNodeToView(int nodeID,int viewTag);
  void disconnectNodeFromView(int nodeID,int viewTag);
  void attachEvent(int viewTag,std::string eventName,int eventNodeID);
  void detachEvent(int viewTag,std::string eventName,int eventNodeID);
  void configureProps(folly::dynamic nativeProps,folly::dynamic uiProps);
  void setValue(int nodeID,int newValue);
  void triggerRender();

  //Functions for internal usage by self & nodes
  REANode* findNodeById(int nodeID);
  void sendEventWithName(std::string eventName , folly::dynamic eventData);
  void scheduleEvaluate();
  void markNodeUpdated(REAFinalNode* node);
  void startUpdatingOnAnimationFrame();
  void onAnimationFrame();
  void performOperations();
  void stopUpdatingOnAnimationFrame();
  void synchronouslyUpdateViewOnUiThread(int viewTag, folly::dynamic newViewProps);
  void enqueueUpdateViewOnNativeThread(int viewTag, folly::dynamic newViewProps);
  void postOnAnimation(int nodeID,std::function<void(void)> clb);
  void stopPostOnAnimation(int nodeID);

  double currentAnimationTimestamp{0.0};
  folly::dynamic configuredUiProps;
  folly::dynamic configuredNativeProps;
 private:

  facebook::better::map<int,REANode*> nodesList_;
  facebook::xplat::RSReanimatedModule* reanimatedModule_;
  std::vector<REAFinalNode*> finalNodes_;
  facebook::better::map<int,std::function<void(void)>> animationCallbacks_;
  std::atomic<bool> animating_{false}; // TODO this need to be flag which is used for notify frame update callback by compositor
};

}// namespace reanimated
