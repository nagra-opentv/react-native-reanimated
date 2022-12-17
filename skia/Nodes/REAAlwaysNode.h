/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REAAlwaysNode : public REANode,public REAFinalNode {
 public:
  REAAlwaysNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  ~REAAlwaysNode();
  REANodeData evaluate() override;
  void update() override;
 private:
  REANodeID nodeToBeEvaluated_;

};

REANodeHandle REAAlwaysNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
