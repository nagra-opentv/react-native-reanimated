/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

using REAOperatorBlock = REANodeData(*)(folly::dynamic inputNodes);

class REAOpNode : public REANode {
 public:
  REAOpNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~REAOpNode();
  REANodeData evaluate() override;

 private:
  folly::dynamic inputNodes_;
  std::function<REANodeData(folly::dynamic)> operation_;
  std::string op_; // used solely for debugging purpose
};


REANodeHandle REAOpNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
