/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "ReactSkia/sdk/FollyTimer.h"
#include "ReactSkia/utils/RnsUtils.h"
#include "ReactSkia/utils/RnsLog.h"

#include "rns_shell/platform/linux/TaskLoop.h"

#include "Nodes/REAAlwaysNode.h"
#include "Nodes/REABezierNode.h"
#include "Nodes/REABlockNode.h"
#include "Nodes/REACallFuncNode.h"
#include "Nodes/REACallNode.h"
#include "Nodes/REAClockNode.h"
#include "Nodes/REAConcatNode.h"
#include "Nodes/REACondNode.h"
#include "Nodes/READebugNode.h"
#include "Nodes/REAEventNode.h"
#include "Nodes/REAFuncNode.h"
#include "Nodes/REANode.h"
#include "Nodes/REAOpNode.h"
#include "Nodes/REAParamNode.h"
#include "Nodes/REAPropsNode.h"
#include "Nodes/REASetNode.h"
#include "Nodes/REAStyleNode.h"
#include "Nodes/REATransformNode.h"
#include "Nodes/REAValueNode.h"

#include "RSNodeManager.h"
#include "RSReanimatedModule.h"


namespace reanimated {
using namespace facebook::xplat;
using namespace facebook::react;

RSNodeManager::RSNodeManager(RSReanimatedModule* module)
  :reanimatedModule_(module) {
}

RSNodeManager::~RSNodeManager() {
  if(nodesList_.size()) {
    for(auto &item : nodesList_) {
      delete(item.second);
    }
  }
}

void RSNodeManager::createNode(int nodeID,folly::dynamic nodeConfig) {
  static facebook::better::map<std::string, REANodeProviderProtocol> nodeClassMap = {
    {std::string("props"),REAPropsNodeProvider},
    {std::string("style"),REAStyleNodeProvider},
    {std::string("transform"),REATransformNodeProvider},
    {std::string("value"),REAValueNodeProvider},
    {std::string("block"),REABlockNodeProvider},
    {std::string("cond"),REACondNodeProvider},
    {std::string("op"),REAOpNodeProvider},
    {std::string("set"),REASetNodeProvider},
    {std::string("debug"),READebugNodeProvider},
    {std::string("clock"),REAClockNodeProvider},
    {std::string("clockStart"),REAClockStartNodeProvider},
    {std::string("clockStop"),REAClockStopNodeProvider},
    {std::string("clockTest"),REAClockTestNodeProvider},
    {std::string("call"),REACallNodeProvider},
    {std::string("bezier"),REABezierNodeProvider},
    {std::string("event"),REAEventNodeProvider},
    {std::string("always"),REAAlwaysNodeProvider},
    {std::string("concat"),REAConcatNodeProvider},
    {std::string("param"),REAParamNodeProvider},
    {std::string("func"),REAFuncNodeProvider},
    {std::string("callfunc"),REACallFuncNodeProvider},
  };

  RNS_LOG_DEBUG("createNode nodeID:" << nodeID << " type:" << nodeConfig["type"].c_str());
  auto p = nodeClassMap.find(nodeConfig["type"].asString());
  if (p != nodeClassMap.end()) {
    auto classProvider = p->second;
    REANode *node = classProvider(nodeID,nodeConfig);
    node->nodeManager = this;
    nodesList_[nodeID] = node;
  } else {
    RNS_LOG_WARN("Unknown node type[" << nodeConfig["type"] << "] for createNode nodeID:" << nodeID );
  }
}

void RSNodeManager::dropNode(int nodeID) {
  RNS_LOG_DEBUG("dropNode nodeID:" << nodeID);
  REANode* node = findNodeById(nodeID);
  if(node != nullptr) {
    nodesList_.erase(nodeID);
    delete node;
  }
}

void RSNodeManager::getValue(int nodeID,module::CxxModule::Callback callback) {
  RNS_LOG_DEBUG("getValue nodeID:" << nodeID);
  REANode* node = findNodeById(nodeID);
  folly::dynamic value;
  if(node != nullptr) {
    value = node->value();
  }
  std::vector<folly::dynamic> nodeValue;
  nodeValue.push_back(value);
  callback(nodeValue);
}

void RSNodeManager::connectNodes(int parentID,int childID) {
  RNS_LOG_DEBUG("connectNode parentID:" << parentID << " childID" << childID);
  REANode* parentNode = findNodeById(parentID);
  if(parentNode != nullptr) {
    REANode* childNode = findNodeById(childID);
    if(childNode != nullptr) {
      parentNode->addChild(childNode);
    }
  }
}

void RSNodeManager::disconnectNodes(int parentID,int childID) {
  RNS_LOG_DEBUG("disconnectNode parentID:" << parentID << " childID" << childID);
  REANode* parentNode = findNodeById(parentID);
  if(parentNode != nullptr) {
    REANode* childNode = findNodeById(childID);
    if(childNode != nullptr) {
      parentNode->removeChild(childNode);
    }
  }
}

void RSNodeManager::connectNodeToView(int nodeID,int viewTag) {
  RNS_LOG_DEBUG("connectNodeToView nodeID:" << nodeID << " viewTag:" << viewTag);
  REANode* node = findNodeById(nodeID);
  if((node != nullptr) && (isInstance(node,REAPropsNode))) {
    (static_cast<REAPropsNode*>(node))->connectToView(viewTag);
  }
}

void RSNodeManager::disconnectNodeFromView(int nodeID,int viewTag) {
  RNS_LOG_DEBUG("disconnectNodeFromView nodeID:" << nodeID << " viewTag:" << viewTag);
  REANode* node = findNodeById(nodeID);
  if((node != nullptr) && (isInstance(node,REAPropsNode))) {
    (static_cast<REAPropsNode*>(node))->disconnectFromView(viewTag);
  }
}

void RSNodeManager::attachEvent(int viewTag,std::string eventName,int eventNodeID) {
  RNS_LOG_TODO("attachEvent viewTag:" << viewTag << " eventName:" << eventName << " eventNodeID:" << eventNodeID);
}

void RSNodeManager::detachEvent(int viewTag,std::string eventName,int eventNodeID) {
  RNS_LOG_TODO("dettachEvent viewTag:" << viewTag << " eventName:" << eventName << " eventNodeID:" << eventNodeID);
}

void RSNodeManager::configureProps(folly::dynamic nativeProps,folly::dynamic uiProps) {
  configuredNativeProps = nativeProps;
  configuredUiProps = uiProps;
}

void RSNodeManager::setValue(int nodeID,int newValue) {
  RNS_LOG_DEBUG("setValue nodeID:" << nodeID << " newValue:" << newValue);
  REAValueNode* valueNode = static_cast<REAValueNode*>(findNodeById(nodeID));
  if(valueNode != nullptr) {
    valueNode->setValue(newValue);
  }
}

void RSNodeManager::triggerRender() {
  RNS_LOG_TODO("triggerRender");
}

REANode* RSNodeManager::findNodeById(int nodeID) {
  auto p = nodesList_.find(nodeID);
  return ((p != nodesList_.end()) ? p->second : nullptr);
}

void RSNodeManager::sendEventWithName(std::string eventName , folly::dynamic eventData) {
  if(reanimatedModule_ != nullptr) {
    RNS_LOG_INFO("Sending Event [" << eventName << "] by nodeID:" << eventData["id"]);
    reanimatedModule_->sendEventWithName(eventName,eventData);
  }
}

void RSNodeManager::scheduleEvaluate() {
  startUpdatingOnAnimationFrame();

  //TODO need to be executed on animation frame callback from renderer
  onAnimationFrame();

}

void RSNodeManager::markNodeUpdated(REAFinalNode* node) {
  finalNodes_.push_back(node);
}

inline void RSNodeManager::startUpdatingOnAnimationFrame() {
  currentAnimationTimestamp = rns::sdk::Timer::getCurrentTimeMSecs();
  //Enable the flag here requesting for animation frame callback
  animating_ = true;
}

void RSNodeManager::onAnimationFrame() {

  currentAnimationTimestamp = rns::sdk::Timer::getCurrentTimeMSecs();

  //TODO Process the events in eventqueue here
  auto callbacks = animationCallbacks_;

  for(auto &kv : callbacks) {
    kv.second();
  }

  performOperations();

  if(animationCallbacks_.size() == 0) {
    stopUpdatingOnAnimationFrame();
  } else {
    // TODO This needs to be removed
    // Today our compositor does provide us the callback for frame update
    // onAnimationFrame has to be called from the frame update callback,until animating_ unregistered
#if 0 //Temporarily disabling, until animation works
    onAnimationFrame();
#endif
  }

}

inline void RSNodeManager::performOperations() {
  REANode::runPropUpdates(finalNodes_);
  //TODO Process the enqueued operations/batch update operations
}

inline void RSNodeManager::stopUpdatingOnAnimationFrame() {
  //Disable the flag here requesting for animation frame callback
  animating_ = false;

}

void RSNodeManager::synchronouslyUpdateViewOnUiThread(int viewTag , folly::dynamic newViewProps) {
  RNS_LOG_DEBUG("synchronouslyUpdatViewOnUiThread for viewTag:" << viewTag);
  reanimatedModule_->getUIManager()->updateViewForReactTag(viewTag,newViewProps);
  //FIXME This need to be removed,when compositor support frame update callback
  if(animating_) {
    onAnimationFrame();
  }
}

void RSNodeManager::enqueueUpdateViewOnNativeThread(int viewTag , folly::dynamic newViewProps) {
  RNS_LOG_TODO("enqueueUpdatViewOnNativeThread");
  //TODO:Add the updation of the view in batch operation list and it will processed in next frame update
}

void RSNodeManager::postOnAnimation(int nodeID,std::function<void(void)> clb) {
  animationCallbacks_[nodeID] = clb;
  startUpdatingOnAnimationFrame();
}

void RSNodeManager::stopPostOnAnimation(int nodeID) {
  if(animationCallbacks_.find(nodeID) != animationCallbacks_.end()) {
    animationCallbacks_.erase(nodeID);
  }
  if(animationCallbacks_.size() == 0) {
    stopUpdatingOnAnimationFrame();
  }
}

}// namespace reanimated
