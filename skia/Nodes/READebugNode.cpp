/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "READebugNode.h"

namespace reanimated {

REANodeHandle READebugNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new READebugNode(nodeId,nodeConfig);
}

READebugNode::READebugNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("READebugNode nodeID:" << nodeId);
  message_ = nodeConfig["message"].getString();
  valueNodeID_ = nodeConfig["value"].asInt();
}

READebugNode::~READebugNode() {
}

REANodeData READebugNode::evaluate() {
  REANodeData value = getNodeData(valueNodeID_);
  RNS_LOG_INFO(message_ << "  " << value);
  return value;
}

}// namespace reanimated
