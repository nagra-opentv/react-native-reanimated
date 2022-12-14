/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAClockNode.h"
#include "REAParamNode.h"

namespace reanimated {

REANodeHandle REAClockNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAClockNode(nodeId,nodeConfig);
}

REANodeHandle REAClockStartNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAClockOpNode(nodeId,nodeConfig,ClockNodeTypeStart);
}

REANodeHandle REAClockStopNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAClockOpNode(nodeId,nodeConfig,ClockNodeTypeStop);
}

REANodeHandle REAClockTestNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAClockOpNode(nodeId,nodeConfig,ClockNodeTypeTest);
}

REAClockNode::REAClockNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig),
    isRunning{false} {

  RNS_LOG_DEBUG("REAClockNode nodeID:" << nodeId);

}

REAClockNode::~REAClockNode() {
}

REANodeData REAClockNode::evaluate() {
  return getCurrentAnimationTimestamp();
}

void REAClockNode::start() {
  if(isRunning) {
    return;
  }

  isRunning = true;

  auto animationClb = []() {
    //TODO check and reschedule animation callback again
    RNS_LOG_TRACE("Received animation frame callback");
  };
  postOnAnimation(animationClb);
}

void REAClockNode::stop() {
  isRunning = false;
  stopPostOnAnimation();
}


REAClockOpNode::REAClockOpNode(REANodeID nodeId,folly::dynamic nodeConfig,ClockNodeType type)
  : REANode(nodeId,nodeConfig),
    clockNodeType_{type} {

  RNS_LOG_DEBUG("REAClockNode nodeID:" << nodeId);
  clockNodeID_ = nodeConfig["clock"].asInt();

}

REAClockOpNode::~REAClockOpNode() {
}

REANodeData REAClockOpNode::evaluate() {
  REANodeHandle node = getNode(clockNodeID_);
  REANodeData res = REAValueEmpty;

  switch(clockNodeType_) {
    case ClockNodeTypeStart:
      startNode(node);
      break;
    case ClockNodeTypeStop:
      stopNode(node);
      break;
    case ClockNodeTypeTest:
      res = testNode(node);
      break;
  }
  return res;
}

void REAClockOpNode::startNode(REANodeHandle node) {
  if(isParamNodeInstance(node)) {
    (static_cast<REAParamNode*>(node))->start();
  } else {
    (static_cast<REAClockNode*>(node))->start();
  }
}

void REAClockOpNode::stopNode(REANodeHandle node) {
  if(isParamNodeInstance(node)) {
    (static_cast<REAParamNode*>(node))->stop();
  } else {
    (static_cast<REAClockNode*>(node))->stop();
  }
}

REANodeData REAClockOpNode::testNode(REANodeHandle node) {
  if(isParamNodeInstance(node)) {
    return (static_cast<REAParamNode*>(node))->isRunning() ? 1 : 0;
  } else {
    return (static_cast<REAClockNode*>(node))->isRunning;
  }
}


}// namespace reanimated
