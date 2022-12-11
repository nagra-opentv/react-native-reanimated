/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once
#include <memory>
#include "folly/dynamic.h"

#include "ReactSkia/utils/RnsLog.h"

#include "RSNodeManager.h"

namespace reanimated {

using REANodeProviderProtocol = reanimated::REANode*(*)(int,folly::dynamic);
typedef int REANodeID;
typedef folly::dynamic REANodeData;
typedef REANode* REANodeHandle;

typedef float REAValuef;
typedef int REAValueI;
typedef REAValuef REAValue;
#define REAValueEmpty 0.0
#define REAValueNull nullptr

#define isInstance( ptr, clazz ) (dynamic_cast<const clazz*>(ptr) != NULL)

//Class which triggers update of the node
//Inherited today by Props and AlwaysNode
class REAFinalNode {
 public:	
  virtual void update() = 0;
};

//Base class for all node types 
class REANode {
 public:
  REANode(REANodeID nodeID,folly::dynamic nodeConfig);	 
  virtual ~REANode();	 

  void addChild(REANodeHandle childNode);
  void removeChild(REANodeHandle childNode);
  static void runPropUpdates(std::vector<REAFinalNode*>& finalNodeList);

  virtual REANodeData evaluate();
  virtual REANodeData value();
  
  REANodeHandle getNode(REANodeID nodeID);
  REANodeData getNodeData(REANodeID nodeID);
  REANodeData getCurrentAnimationTimestamp();
  folly::dynamic getConfiguredUiProps();
  folly::dynamic getConfiguredNativeProps();
  REANodeID getNodeID();

  void sendEventWithName(std::string eventName , folly::dynamic eventData);
  void synchronouslyUpdateViewOnUiThread(REAValueI viewTag , folly::dynamic props);
  void enqueueUpdateViewOnNativeThread(REAValueI viewTag, folly::dynamic props);
  void scheduleEvaluate();
  void postOnAnimation(std::function<void()> animationCallback);
  void stopPostOnAnimation();
  
  RSNodeManager* nodeManager{nullptr};
 
 private:
  REANodeID nodeID_{0};
  std::vector<REANodeHandle> childNodes_;
};

}// namespace reanimated
