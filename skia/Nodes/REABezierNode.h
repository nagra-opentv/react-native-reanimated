/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REABezierNode : public REANode {
 public:
  REABezierNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  ~REABezierNode();
  REANodeData evaluate() override;

 private:
  REAValuef ax_{0.0};
  REAValuef bx_{0.0};
  REAValuef cx_{0.0};
  REAValuef ay_{0.0};
  REAValuef by_{0.0};
  REAValuef cy_{0.0};

  REANodeID inputNodeID_;

  inline REAValuef sampleCurveX(REAValuef t);
  inline REAValuef sampleCurveY(REAValuef t);
  inline REAValuef sampleCurveDerivativeX(REAValuef t);
  inline REAValuef solveCurveX(REAValuef x);
};


REANodeHandle REABezierNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
