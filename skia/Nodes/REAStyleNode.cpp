/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAStyleNode.h"

namespace reanimated {

REANodeHandle REAStyleNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig) {
  return new REAStyleNode(nodeId,nodeConfig);
}

REAStyleNode::REAStyleNode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : REANode(nodeId,nodeConfig) {

  styleConfig_ = nodeConfig["style"];
  RNS_LOG_DEBUG("REAStyleNode nodeID:" << nodeId << " configSize:" << styleConfig_.size());
}
  
REAStyleNode::~REAStyleNode() {
}

REANodeData REAStyleNode::evaluate() {
  folly::dynamic styles = folly::dynamic::object();
  for(auto &styleprop : styleConfig_.items()) {
    styles[styleprop.first.asString()] = getNodeData(styleprop.second.asInt());
  }

  return styles;
}

}// namespace reanimated
