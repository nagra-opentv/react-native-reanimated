/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REACallNode : public REANode {
 public:
  REACallNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~REACallNode();
  REANodeData evaluate();

 private:
  folly::dynamic input_;
};


REANodeHandle REACallNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
