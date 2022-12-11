/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REABezierNode.h"

namespace reanimated {

#define EPSILON 1e-5

REANodeHandle REABezierNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REABezierNode(nodeId,nodeConfig);
}

REABezierNode::REABezierNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REABezierNode nodeID:" << nodeId);

  inputNodeID_ = nodeConfig["input"].asInt();

  REAValuef mX1 = nodeConfig["mX1"].asDouble();
  REAValuef mY1 = nodeConfig["mY1"].asDouble();
  REAValuef mX2 = nodeConfig["mX2"].asDouble();
  REAValuef mY2 = nodeConfig["mY2"].asDouble();

  // Calculate the polynomial coefficients, implicit first and last control points are (0,0) and (1,1).
  cx_ = 3.0 * mX1;
  bx_ = 3.0 * (mX2 - mX1) - cx_;
  ax_ = 1.0 - cx_ -bx_;

  cy_ = 3.0 * mY1;
  by_ = 3.0 * (mY2 - mY1) - cy_;
  ay_ = 1.0 - cy_ - by_;

}

REABezierNode::~REABezierNode() {
}

inline REAValuef REABezierNode::sampleCurveX(REAValuef t) {
  // `ax t^3 + bx t^2 + cx t' expanded using Horner's rule.
  return ((ax_ * t + bx_) * t + cx_) * t;
}

inline REAValuef REABezierNode::sampleCurveY(REAValuef t) {
  return ((ay_ * t + by_) * t + cy_) * t;
}

inline REAValuef REABezierNode::sampleCurveDerivativeX(REAValuef t) {
  return (3.0 * ax_ * t + 2.0 * bx_) * t + cx_;
}

inline REAValuef REABezierNode::solveCurveX(REAValuef x) {
  REAValuef t0, t1, t2, x2, d2;
  REAValueI i;

  // First try a few iterations of Newton's method -- normally very fast.
  for (t2 = x, i = 0; i < 8; i++) {
    x2 = sampleCurveX(t2) - x;
    if (fabs(x2) < EPSILON)
      return t2;
    d2 = sampleCurveDerivativeX(t2);
    if (fabs(d2) < 1e-6)
      break;
    t2 = t2 - x2 / d2;
  }

  // Fall back to the bisection method for reliability.
  t0 = 0.0;
  t1 = 1.0;
  t2 = x;

  if (t2 < t0)
    return t0;
  if (t2 > t1)
    return t1;

  while (t0 < t1) {
    x2 = sampleCurveX(t2);
    if (fabs(x2 - x) < EPSILON)
      return t2;
    if (x > x2)
      t0 = t2;
    else
      t1 = t2;
    t2 = (t1 - t0) * .5 + t0;
  }

  // Failure.
  return t2;

}

REANodeData REABezierNode::evaluate() {
  REAValuef x = getNodeData(inputNodeID_).asDouble();
  REAValuef y = sampleCurveY(solveCurveX(x));

  return y;
}

}// namespace reanimated
