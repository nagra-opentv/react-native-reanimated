/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAAlwaysNode.h"

namespace reanimated {

REANodeHandle REAAlwaysNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig) {
  return new REAAlwaysNode(nodeId,nodeConfig);
}

REAAlwaysNode::REAAlwaysNode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REAAlwaysNode nodeID:" << nodeId);
  nodeToBeEvaluated_ = nodeConfig["what"].asInt();
}

REAAlwaysNode::~REAAlwaysNode() {
}

REANodeData REAAlwaysNode::evaluate() {
  return getNodeData(nodeToBeEvaluated_);
}

void REAAlwaysNode::update() {
  value();
}

}// namespace reanimated
