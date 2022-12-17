/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAValueNode.h"

namespace reanimated {

REANodeHandle REAValueNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig) {
  return new REAValueNode(nodeId,nodeConfig);
}

REAValueNode::REAValueNode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : REANode(nodeId,nodeConfig) {

  value_ = nodeConfig["value"].isNull()? 0 : nodeConfig["value"];
  RNS_LOG_DEBUG("REAValueNode nodeID:" << nodeId << " value:" << value_);
}

REAValueNode::~REAValueNode() {
}

void REAValueNode::setValue(REANodeData value) {
  value_ = value.isNull() ? REAValueEmpty : value;
  RNS_LOG_DEBUG("REAValueNode nodeID:" << getNodeID() << " Setvalue:" << value_);
}

REANodeData REAValueNode::evaluate() {
  return value_;
}

}// namespace reanimated
