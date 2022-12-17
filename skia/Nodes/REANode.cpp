/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "ReactSkia/utils/RnsLog.h"
#include "REANode.h"

namespace reanimated {

REANode::REANode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : nodeID_(nodeId) {
}

REANode::~REANode() {
}

void REANode::addChild(REANodeHandle childNode) {
  if(childNode != nullptr) {
    childNodes_.push_back(childNode);
    childNode->scheduleEvaluate();
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

void REANode::runPropUpdates(FinalNodes finalNodeList) {
  //TODO : PROCESS UPDATED NODES
  //Traverse all updated nodes and get the final Nodes list and process
  for(auto it = finalNodeList.rbegin();it != finalNodeList.rend(); it++) {
    REAFinalNode *finalNode = *it;
    if(finalNode) {
      finalNode->update();
    }
  }
}

REANodeData REANode::evaluate() {
  return nullptr;
}

REANodeData REANode::value() {
  //TODO : MEMOIZED VALUES
  //Process and save the process nodes value and use the same in next evaluation
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
  //TODO : PROCESS UPDATED NODES
  //Add the node to updatedNodes list
  nodeManager->postRunUpdatesAfterAnimation(this);
}

void REANode::postOnAnimation(REAAnimationClb animationCallback) {
  nodeManager->postOnAnimation(nodeID_, animationCallback);
}

void REANode::stopPostOnAnimation() {
  nodeManager->stopPostOnAnimation(nodeID_);
}

}// namespace reanimated
