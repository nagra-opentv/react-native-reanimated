/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REABlockNode.h"

namespace reanimated {

REANodeHandle REABlockNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig) {
  return new REABlockNode(nodeId,nodeConfig);
}

REABlockNode::REABlockNode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REABlockNode nodeID:" << nodeId);

  block_ = nodeConfig["block"];
}
  
REABlockNode::~REABlockNode() {
}

REANodeData REABlockNode::evaluate() {
  REANodeData result;
  for(auto &id : block_) {
    result = getNodeData(id.asInt());
  }
  return result;
}

}// namespace reanimated
