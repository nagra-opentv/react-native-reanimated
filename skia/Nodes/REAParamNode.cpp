/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAClockNode.h"
#include "REAParamNode.h"
#include "REAValueNode.h"

namespace reanimated {

REANodeHandle REAParamNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAParamNode(nodeId,nodeConfig);
}

REAParamNode::REAParamNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REAValueNode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REAParamNode nodeID:" << nodeId);
}
  
REAParamNode::~REAParamNode() {
}

void REAParamNode::beginContext(folly::dynamic arg) {
  argsStack_.push_back(arg.asInt());
}

void REAParamNode::endContext() {
  if(argsStack_.size()){
    argsStack_.pop_back();
  }
}

void REAParamNode::start() {
  REANodeHandle node = getNode(argsStack_.back());
  if(isInstance(node,REAParamNode)) {
    (static_cast<REAParamNode*>(node))->start();
  } else {
    (static_cast<REAClockNode*>(node))->start();
  }

}

void REAParamNode::stop() {
  REANodeHandle node = getNode(argsStack_.back());
  if(isInstance(node,REAParamNode)) {
    (static_cast<REAParamNode*>(node))->stop();
  } else {
    (static_cast<REAClockNode*>(node))->stop();
  }

}

bool REAParamNode::isRunning() {
  REANodeHandle node = getNode(argsStack_.back());
  if(isInstance(node,REAParamNode)) {
    return (static_cast<REAParamNode*>(node))->isRunning();
  } else {
    return (static_cast<REAClockNode*>(node))->isRunning;
  }
  return false;
}

REANodeData REAParamNode::evaluate() {
  return getNodeData(argsStack_.back());
}

void REAParamNode::setValue(REANodeData value) {
  REANodeHandle node = getNode(argsStack_.back());
  (static_cast<REAValueNode*>(node))->setValue(value);
}

}// namespace reanimated
