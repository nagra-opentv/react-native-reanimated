/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REAValueNode : public REANode {
 public:
  REAValueNode(REANodeID nodeId,folly::dynamic nodeConfig);
  virtual ~REAValueNode();
  void setValue(REANodeData value);
  REANodeData evaluate() override;

 private:
  folly::dynamic value_;
};


REANodeHandle REAValueNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
