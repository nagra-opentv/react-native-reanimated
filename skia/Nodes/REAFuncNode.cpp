/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAFuncNode.h"

namespace reanimated {

REANodeHandle REAFuncNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAFuncNode(nodeId,nodeConfig);
}

REAFuncNode::REAFuncNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REAFuncNode nodeID:" << nodeId);
  nodeTobeEvaluated_ = nodeConfig["what"].asInt();
}

REAFuncNode::~REAFuncNode() {
}

REANodeData REAFuncNode::evaluate() {
  return getNodeData(nodeTobeEvaluated_);
}

}// namespace reanimated
