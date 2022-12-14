/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/
#include "REASetNode.h"
#include "REAValueNode.h"

namespace reanimated {

REANodeHandle REASetNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REASetNode(nodeId,nodeConfig);
}

REASetNode::REASetNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  whatNodeID_ = nodeConfig["what"].asInt();
  valueNodeID_ = nodeConfig["value"].asInt();
  RNS_LOG_DEBUG("REASetNode nodeID:" << nodeId << " whatNodeID:" << whatNodeID_ << " valueNodeID_:" << valueNodeID_);
}

REASetNode::~REASetNode() {
}

REANodeData REASetNode::evaluate() {
  REANodeData newValue = getNodeData(valueNodeID_);
  REAValueNode* whatNode = static_cast<REAValueNode*>(getNode(whatNodeID_));
  if(whatNode != nullptr) {
    whatNode->setValue(newValue);
  }

  RNS_LOG_DEBUG("REASetNode  newValue:" << newValue);
  return newValue;
}


}// namespace reanimated
