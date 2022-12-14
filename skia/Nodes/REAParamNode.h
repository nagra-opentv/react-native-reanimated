/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#pragma once

#include "REANode.h"
#include "REAValueNode.h"

namespace reanimated {

class REAParamNode : public REAValueNode {
 public:
  REAParamNode(REANodeID nodeId,folly::dynamic nodeConfig);
  ~REAParamNode();

  REANodeData evaluate() override;
  void beginContext(folly::dynamic arg);
  void endContext();
  void start();
  void stop();
  bool isRunning();
  void setValue(REANodeData value) override;

 private:
  std::vector<REANodeID> argsStack_;
};

REANodeHandle REAParamNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig);

}// namespace reanimated
