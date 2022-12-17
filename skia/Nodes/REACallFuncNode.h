/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REACallFuncNode : public REANode {
 public:
  REACallFuncNode(REANodeID nodeId,folly::dynamic &nodeConfig);
  ~REACallFuncNode();
  REANodeData evaluate() override;

 private:
  void beginContext();
  void endContext();

  REANodeID whatNodeID_{0};
  folly::dynamic args_;
  folly::dynamic params_;
};


REANodeHandle REACallFuncNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig);

}// namespace reanimated
