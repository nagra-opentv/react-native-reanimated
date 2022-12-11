/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAPropsNode.h"
#include "REAStyleNode.h"

namespace reanimated {

REANodeHandle REAPropsNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAPropsNode(nodeId,nodeConfig);
}

REAPropsNode::REAPropsNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  propsConfig_ = nodeConfig["props"];
  RNS_LOG_DEBUG("REAPropsNode nodeID:" << nodeId << " propsConfig count:" << propsConfig_.size());

}
  
REAPropsNode::~REAPropsNode() {
}

void REAPropsNode::connectToView(REAValueI viewTag) {
  connectedViewTag_ = viewTag;

  scheduleEvaluate();
}

void REAPropsNode::disconnectFromView(REAValueI viewTag) {
  connectedViewTag_ = -1;
}

REANodeData REAPropsNode::evaluate() {
  folly::dynamic uiProps = folly::dynamic::object();
  folly::dynamic nativeProps = folly::dynamic::object();
  folly::dynamic jsProps = folly::dynamic::object();

  auto updateProps = [&](std::string propName , folly::dynamic propValue) {
    auto uiPropsList = getConfiguredUiProps();
    auto nativePropsList = getConfiguredNativeProps();

    if(std::find(uiPropsList.begin(),uiPropsList.end(),propName) != uiPropsList.end()) {
      uiProps[propName] = propValue;
    } else if (std::find(nativePropsList.begin(),nativePropsList.end(),propName) != nativePropsList.end()) {
      nativeProps[propName] = propValue;
    } else {
      jsProps[propName] = propValue;
    }
  };

  for(auto &prop : propsConfig_.items()) {
    REANodeHandle node = getNode(prop.second.asInt());
    if(isInstance(node,REAStyleNode)) {
      folly::dynamic styleNodeValue = node->value();
      for(auto &style : styleNodeValue.items()) {
        updateProps(style.first.asString() , style.second);
      }
    } else {
      updateProps(prop.first.asString(),node->value());
    }
  }

  if(connectedViewTag_ != -1) {
    //TODO props updation way 
    //a.uiProps need to be updated synchronously
    //b.nativeProps need to be updated in next frame as a batch update
    //Today we dont have native frame update notification
    //So we follow the supported way of updating ui & native props in next frame
    if(uiProps.size()) {
      synchronouslyUpdateViewOnUiThread(connectedViewTag_,uiProps);
    }
    if(nativeProps.size()) {
      enqueueUpdateViewOnNativeThread(connectedViewTag_,uiProps);
    }
    if(jsProps.size()) {
      folly::dynamic eventData = folly::dynamic::object();
      eventData["viewTag"] = connectedViewTag_;
      eventData["props"] = jsProps;
      sendEventWithName("onReanimatedPropsChange",eventData);
    }
  }
  return REAValueEmpty;
}

void REAPropsNode::update() {
  if(connectedViewTag_ != -1) {
    value();
  }
}

}// namespace reanimated
