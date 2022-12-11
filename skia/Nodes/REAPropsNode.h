/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"

namespace reanimated {

class REAPropsNode : public REANode, public REAFinalNode {
 public:
  REAPropsNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~REAPropsNode();
  REANodeData evaluate() override;
  void connectToView(REAValueI viewTag);
  void disconnectFromView(REAValueI viewTag);
  void update() override;

 private:
  REAValueI connectedViewTag_{-1};
  folly::dynamic propsConfig_;
};


REANodeHandle REAPropsNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
