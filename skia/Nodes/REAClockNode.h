/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

enum ClockNodeType{
  ClockNodeTypeStart = 0,
  ClockNodeTypeStop,
  ClockNodeTypeTest,
};

class REAClockNode : public REANode {
 public:
  REAClockNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  virtual ~REAClockNode();
  REANodeData evaluate() override;
  void start();
  void stop();

  bool isRunning{false};
};

class REAClockOpNode : public REANode {
 public:
  REAClockOpNode(REANodeID nodeId,folly::dynamic &nodeConfig, ClockNodeType type);
  virtual ~REAClockOpNode();
  REANodeData evaluate() override;

 private:
  void startNode(REANodeHandle node);
  void stopNode(REANodeHandle node);
  REANodeData testNode(REANodeHandle node);

  REANodeID clockNodeID_;
  ClockNodeType clockNodeType_;
};

REANodeHandle REAClockNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);
REANodeHandle REAClockStartNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);
REANodeHandle REAClockStopNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);
REANodeHandle REAClockTestNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
