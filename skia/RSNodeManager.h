/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#pragma once

#include <better/map.h>
#include <cxxreact/CxxModule.h>
#include "ReactSkia/utils/RnsJsRaf.h"
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
  void sendEventWithName(std::string eventName , folly::dynamic eventData); //Helper function to send events to JS
  void postRunUpdatesAfterAnimation(REANode* node); // Node helper function : To trigger update of nodes in next animation frame
  void postOnAnimation(int nodeID,std::function<void(void)> clb); // Node helper function : To register callbacks for animation frame notification
  void stopPostOnAnimation(int nodeID); // Node helper function : To unregister callbacks from animation frame notification
  void startUpdatingOnAnimationFrame(); // Node manager function : To start animation frame request
  void stopUpdatingOnAnimationFrame();  // Node manager function : To stop animation frame request
  void onAnimationFrame(double timestamp); // Node manager function : Callback to animation frame request
  void performOperations(); // Node manager function : Update nodes and enqueued operations
  void synchronouslyUpdateViewOnUiThread(int viewTag, folly::dynamic newViewProps); // Node helper function: update view props in current animation frame
  void enqueueUpdateViewOnNativeThread(int viewTag, folly::dynamic newViewProps); // Node helper function : update view props in next animation frame

  double currentAnimationTimestamp{0.0};
  folly::dynamic configuredUiProps;
  folly::dynamic configuredNativeProps;

 private:
  facebook::better::map<int,REANode*> nodesList_;
  std::vector<REAFinalNode*> finalNodes_;
  facebook::better::map<int,std::function<void(void)>> animationCallbacks_;
  facebook::xplat::RSReanimatedModule* reanimatedModule_;
  facebook::react::RnsJsRequestAnimation* animRequest_{nullptr};
};

}// namespace reanimated
