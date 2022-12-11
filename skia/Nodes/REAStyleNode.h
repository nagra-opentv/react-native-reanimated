/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REAStyleNode : public REANode {
 public:
  REAStyleNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~REAStyleNode();
  REANodeData evaluate() override;
 
 private:
  folly::dynamic styleConfig_;
};


REANodeHandle REAStyleNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
