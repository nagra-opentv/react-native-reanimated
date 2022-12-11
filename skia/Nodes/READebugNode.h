/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class READebugNode : public REANode {
 public:
  READebugNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~READebugNode();
  REANodeData evaluate() override;

 private:
  std::string message_;
  REANodeID valueNodeID_;
};


REANodeHandle READebugNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
