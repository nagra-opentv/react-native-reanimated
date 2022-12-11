/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REACondNode.h"

namespace reanimated {

REANodeHandle REACondNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REACondNode(nodeId,nodeConfig);
}

REACondNode::REACondNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REACondNode nodeID:" << nodeId);
  condNodeID_ = nodeConfig["cond"].asInt();
  ifBlockID_ = nodeConfig["ifBlock"].asInt();

  auto elseNodeID = nodeConfig["elseBlock"];
  elseBlockID_ = elseNodeID.isNull() ? 0 : elseNodeID.asInt();
}
  
REACondNode::~REACondNode() {
}

REANodeData REACondNode::evaluate() {
  REANodeData condNodeValue = getNodeData(condNodeID_);
  auto cond = condNodeValue.isNull() ? 0 : condNodeValue.asDouble();
  if(cond) {
    REANodeData ifNodeValue = getNodeData(ifBlockID_);
    RNS_LOG_DEBUG("CondNode ifnodeID:" << ifBlockID_ << "  value:" << ifNodeValue);
    return ifNodeValue;
  } else {
    REANodeData elseNodeValue =  ((elseBlockID_ == 0) ? 0 : getNodeData(elseBlockID_));
    RNS_LOG_DEBUG("CondNode elsenodeID:" << elseBlockID_ << "  value:" << elseNodeValue);
    return elseNodeValue;
  }
}

}// namespace reanimated
