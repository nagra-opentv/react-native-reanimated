/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REAFuncNode : public REANode {
 public:
  REAFuncNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  ~REAFuncNode();
  REANodeData evaluate() override;

 private:
  REANodeID nodeTobeEvaluated_;
};


REANodeHandle REAFuncNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
