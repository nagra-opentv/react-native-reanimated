/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REACondNode : public REANode {
 public:
  REACondNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  ~REACondNode();
  REANodeData evaluate() override;

 private:
  REANodeID condNodeID_{0};
  REANodeID ifBlockID_{0};
  REANodeID elseBlockID_{0};
};


REANodeHandle REACondNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
