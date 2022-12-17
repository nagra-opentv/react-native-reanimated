/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REACallFuncNode.h"
#include "REAParamNode.h"

namespace reanimated {

REANodeHandle REACallFuncNodeProvider(REANodeID nodeId,folly::dynamic &nodeConfig) {
  return new REACallFuncNode(nodeId,nodeConfig);
}

REACallFuncNode::REACallFuncNode(REANodeID nodeId,folly::dynamic &nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REACallFuncNode nodeID:" << nodeId);
  whatNodeID_ = nodeConfig["what"].asInt();
  args_ = nodeConfig["args"];
  params_ = nodeConfig["params"];
}

REACallFuncNode::~REACallFuncNode() {
}

inline void REACallFuncNode::beginContext() {
  for(int i=0; i< params_.size() ; i++) {
    REANodeHandle node = getNode(params_[i].asInt());
    if(node && isParamNodeInstance(node)) {
      REAParamNode* param = static_cast<REAParamNode*>(node);
      param->beginContext(args_[i]);
    }
  }
}

inline void REACallFuncNode::endContext() {
  for(int i=0; i< params_.size() ; i++) {
    REANodeHandle node = getNode(params_[i].asInt());
    if(node && isParamNodeInstance(node)) {
      REAParamNode* param = static_cast<REAParamNode*>(node);
      param->endContext();
    }
  }
}

REANodeData REACallFuncNode::evaluate() {
  beginContext();

  REANodeData whatValue = getNodeData(whatNodeID_);

  endContext();

  return whatValue;
}

}// namespace reanimated
