/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "ReactSkia/utils/RnsLog.h"
#include "REANode.h"

namespace reanimated {

REANode::REANode(REANodeID nodeId,folly::dynamic nodeConfig) 
  : nodeID_(nodeId) {
}

REANode::~REANode() {
}

void REANode::addChild(REANodeHandle childNode) {
  if(childNode != nullptr) {
    childNodes_.push_back(childNode);
  }
}
  
void REANode::removeChild(REANodeHandle childNode) {
  size_t index = 0;
  for (auto iter = childNodes_.begin(); iter != childNodes_.end(); ++iter, index++) {
    if (*iter != childNode) {
      continue;
    }
    childNodes_.erase(childNodes_.begin() + index);
    return;
  }
}

void REANode::runPropUpdates(std::vector<REAFinalNode*>& finalNodeList) {
  while(finalNodeList.size()) {
    REAFinalNode* node = finalNodeList.back();
    node->update();
    finalNodeList.pop_back();
  }
}

REANodeData REANode::evaluate() {
  return nullptr;
}

REANodeData REANode::value() {
  //TODO Provide value from memoized value
  REANodeData value = evaluate();
  return value;
}

REANodeHandle REANode::getNode(REANodeID nodeID) {
  return nodeManager->findNodeById(nodeID);
}

REANodeData REANode::getNodeData(REANodeID nodeID) {
  REANodeHandle node = getNode(nodeID);
  if(node != nullptr) {
    return node->value();
  }
  return REAValueEmpty;
}

REANodeData REANode::getCurrentAnimationTimestamp() {
  return nodeManager->currentAnimationTimestamp;
}

folly::dynamic REANode::getConfiguredUiProps(){
  return nodeManager->configuredUiProps;
}

folly::dynamic REANode::getConfiguredNativeProps() {
  return nodeManager->configuredNativeProps;
}

REANodeID REANode::getNodeID() {
  return nodeID_;
}

void REANode::sendEventWithName(std::string eventName,folly::dynamic eventData) {
  nodeManager->sendEventWithName(eventName,eventData);
}

void REANode::synchronouslyUpdateViewOnUiThread(REAValueI viewTag,folly::dynamic newViewProps) {
  nodeManager->synchronouslyUpdateViewOnUiThread(viewTag,newViewProps);
}

void REANode::enqueueUpdateViewOnNativeThread(REAValueI viewTag,folly::dynamic newViewProps) {
  nodeManager->enqueueUpdateViewOnNativeThread(viewTag,newViewProps);
}

void REANode::scheduleEvaluate() {
  if(isInstance(this,REAFinalNode)) {
    nodeManager->markNodeUpdated(dynamic_cast<REAFinalNode*>(this));
  }
  nodeManager->scheduleEvaluate();
}

void REANode::postOnAnimation(std::function<void()> animationCallback) {
  nodeManager->postOnAnimation(nodeID_, animationCallback);
}

void REANode::stopPostOnAnimation() {
  nodeManager->stopPostOnAnimation(nodeID_);
}

}// namespace reanimated
