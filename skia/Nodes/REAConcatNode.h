/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REAConcatNode : public REANode {
 public:
  REAConcatNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~REAConcatNode();
  REANodeData evaluate() override;

 private:
  folly::dynamic input_;
};


REANodeHandle REAConcatNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
