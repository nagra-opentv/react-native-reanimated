/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REACallNode.h"

namespace reanimated {

REANodeHandle REACallNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig) {
  return new REACallNode(nodeId,nodeConfig);
}

REACallNode::REACallNode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REACallNode nodeID:" << nodeId);
  input_ = nodeConfig["input"];
}

REACallNode::~REACallNode() {
}

REANodeData REACallNode::evaluate() {
  folly::dynamic eventArgs = folly::dynamic::array();
  for(auto &id : input_) {
    REANodeData value = getNodeData(id.asInt());
    if(!value.isNull()) {
       eventArgs.push_back(value);
    }
  }

  folly::dynamic eventData = folly::dynamic::object();
  eventData["id"] = getNodeID();
  eventData["args"] = eventArgs;
  sendEventWithName(std::string("onReanimatedCall"), eventData);

  return REAValueEmpty;
}

}// namespace reanimated
