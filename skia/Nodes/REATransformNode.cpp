/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REATransformNode.h"

namespace reanimated {

REANodeHandle REATransformNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REATransformNode(nodeId,nodeConfig);
}

REATransformNode::REATransformNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  transformConfigs_ = nodeConfig["transform"];
  RNS_LOG_DEBUG("REATransformNode nodeID:" << nodeId << " count:" << transformConfigs_.size());
}

REATransformNode::~REATransformNode() {
}

REANodeData REATransformNode::evaluate() {
  folly::dynamic transform = folly::dynamic::array();
  for(auto &config : transformConfigs_){
    REANodeID nodeID = config["nodeID"].asInt();

    folly::dynamic value = nodeID ? getNodeData(nodeID) : config["value"];

    folly::dynamic property = folly::dynamic::object();
    property[config["property"].getString()] = value;
    transform.push_back(property);
  }

  return transform;
}

}// namespace reanimated
