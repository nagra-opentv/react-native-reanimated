/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAEventNode.h"

namespace reanimated {

REANodeHandle REAEventNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAEventNode(nodeId,nodeConfig);
}

REAEventNode::REAEventNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_TODO("REAEventNode nodeID:" << nodeId);  
}
  
REAEventNode::~REAEventNode() {
}

}// namespace reanimated
