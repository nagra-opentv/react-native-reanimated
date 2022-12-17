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
  animRequest_ = new RnsJsRequestAnimation([this](double timestamp){
    RNS_LOG_DEBUG("[" << this->animRequest_ << "] [" << timestamp << "]");
    onAnimationFrame(timestamp);
  });
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

    //TODO : PROCESS UPDATED NODES
    //FinalNodes list to be derived from updated nodes list.
    //Removal of the node from finalNodesList to be removed from here when we support it.
    if(isFinalNodeInstance(node)) {
      REAFinalNode *finalNode = dynamic_cast<REAFinalNode*>(node);
      std::vector<REAFinalNode*>::iterator it = std::find(finalNodes_.begin(),finalNodes_.end(),finalNode);
      if(it != finalNodes_.end()) {
        finalNodes_.erase(it);
      }
    }

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

      //TODO : PROCESS UPDATED NODES
      //FinalNodes list to be derived from updated nodes list.
      //Removal of the node from finalNodesList to be removed from here when we support it.
      if(isFinalNodeInstance(childNode)) {
        REAFinalNode *finalNode = dynamic_cast<REAFinalNode*>(childNode);
        std::vector<REAFinalNode*>::iterator it = std::find(finalNodes_.begin(),finalNodes_.end(),finalNode);
        if(it != finalNodes_.end()) {
          finalNodes_.erase(it);
        }
      }

    }
  }
}

void RSNodeManager::connectNodeToView(int nodeID,int viewTag) {
  RNS_LOG_DEBUG("connectNodeToView nodeID:" << nodeID << " viewTag:" << viewTag);
  REANode* node = findNodeById(nodeID);
  if((node != nullptr) && (isPropsNodeInstance(node))) {
    (static_cast<REAPropsNode*>(node))->connectToView(viewTag);
  }
}

void RSNodeManager::disconnectNodeFromView(int nodeID,int viewTag) {
  RNS_LOG_DEBUG("disconnectNodeFromView nodeID:" << nodeID << " viewTag:" << viewTag);
  REANode* node = findNodeById(nodeID);
  if((node != nullptr) && (isPropsNodeInstance(node))) {
    (static_cast<REAPropsNode*>(node))->disconnectFromView(viewTag);
  }
}

void RSNodeManager::attachEvent(int viewTag,std::string eventName,int eventNodeID) {
  RNS_LOG_NOT_IMPL_MSG("attachEvent");
  RNS_LOG_DEBUG("attachEvent viewTag:" << viewTag << " eventName:" << eventName << " eventNodeID:" << eventNodeID);
}

void RSNodeManager::detachEvent(int viewTag,std::string eventName,int eventNodeID) {
  RNS_LOG_NOT_IMPL_MSG("detachEvent");
  RNS_LOG_DEBUG("dettachEvent viewTag:" << viewTag << " eventName:" << eventName << " eventNodeID:" << eventNodeID);
}

void RSNodeManager::configureProps(folly::dynamic nativeProps,folly::dynamic uiProps) {
  configuredNativeProps = nativeProps;
  configuredUiProps = uiProps;
}

void RSNodeManager::setValue(int nodeID,int newValue) {
  RNS_LOG_DEBUG("setValue nodeID:" << nodeID << " newValue:" << newValue);
  REANode* valueNode = findNodeById(nodeID);
  if(valueNode && isValueNodeInstance(valueNode)) {
    (static_cast<REAValueNode*>(valueNode))->setValue(newValue);
  }
}

void RSNodeManager::triggerRender() {
  RNS_LOG_NOT_IMPL_MSG("triggerRender");
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

void RSNodeManager::postOnAnimation(int nodeID,REAAnimationClb clb) {
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

void RSNodeManager::postRunUpdatesAfterAnimation(REANode* node) {
  //Evaluation trigger on FinalNode types only
  if(isFinalNodeInstance(node)) {
    //TODO : PROCESS UPDATED NODES
    //FinalNodes list to be derived from updated nodes list.
    //Adding to finalNodes_ to be removed from here when we support it
    REAFinalNode * finalNode = dynamic_cast<REAFinalNode*>(node);
    if(std::find(finalNodes_.begin(),finalNodes_.end(),finalNode) == finalNodes_.end()) {
      finalNodes_.push_back(finalNode);
    }

    startUpdatingOnAnimationFrame();

    //FIXME : MEMOIZED VALUES
    //animRequest start need to be moved to startUpdatingOnAnimationFrame
    //We are starting here, since we do not support memoized value
    //Today,every performOperations evaluates all nodes including ClockStartNode,
    //Processing on clockStartNode will start animRequest again
    //This will be avoided when we support memoized value
    animRequest_->start();
  }
}

inline void RSNodeManager::startUpdatingOnAnimationFrame() {
  currentAnimationTimestamp = rns::sdk::Timer::getCurrentTimeMSecs();
}

inline void RSNodeManager::stopUpdatingOnAnimationFrame() {
  animRequest_->stop();
}

void RSNodeManager::onAnimationFrame(double timestamp) {
  currentAnimationTimestamp = timestamp;

  //TODO Process the events in eventqueue here

  //Take a copy of the callbacks here , since in callback new callbacks can be added
  auto callbacks = animationCallbacks_;
  for(auto &kv : callbacks) {
    kv.second();
  }

  performOperations();

  if(animationCallbacks_.size() == 0) {
    stopUpdatingOnAnimationFrame();
  }
}

inline void RSNodeManager::performOperations() {
  REANode::runPropUpdates(finalNodes_);

  //TODO Process the enqueued operations/batch update operations
}

void RSNodeManager::synchronouslyUpdateViewOnUiThread(int viewTag , folly::dynamic newViewProps) {
  RNS_LOG_DEBUG("synchronouslyUpdatViewOnUiThread for viewTag:" << viewTag);
  reanimatedModule_->getUIManager()->updateViewForReactTag(viewTag,newViewProps);
}

void RSNodeManager::enqueueUpdateViewOnNativeThread(int viewTag , folly::dynamic newViewProps) {
  RNS_LOG_NOT_IMPL_MSG("enqueueUpdatViewOnNativeThread");
  //TODO:Add the updation of the view in batch operation list and it will processed in next frame update
}

}// namespace reanimated
