/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAConcatNode.h"

namespace reanimated {

REANodeHandle REAConcatNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAConcatNode(nodeId,nodeConfig);
}

REAConcatNode::REAConcatNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REAConcatNode nodeID:" << nodeId);
  input_ = nodeConfig["input"];
}

REAConcatNode::~REAConcatNode() {
}

REANodeData REAConcatNode::evaluate() {
  std::string result;
  for(auto &id : input_) {
    REANodeData value = getNodeData(id.asInt());
    if(value.isDouble()) {
      result = result + std::to_string(value.asDouble());
    } else if(value.isInt()) {
      result = result + std::to_string(value.asInt());
    } else if (value.isString()) {
      result = result + value.asString();
    }
  }
  return result;
}

}// namespace reanimated
