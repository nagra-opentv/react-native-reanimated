/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REASetNode : public REANode {
 public:
  REASetNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  ~REASetNode();
  REANodeData evaluate() override;

 private:
  REANodeID whatNodeID_{0};
  REANodeID valueNodeID_{0};
};


REANodeHandle REASetNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
