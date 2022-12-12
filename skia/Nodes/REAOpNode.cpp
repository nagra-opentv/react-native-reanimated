/*
* Copyright (C) 1994-2022 OpenTV, Inc. and Nagravision S.A.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#include "REAOpNode.h"

namespace reanimated {

#define REA_REDUCE(operation) [this](folly::dynamic inputNodes){ \
  /*TODO Handle extraction of numeric value from string value*/\
  REANodeData value = getNodeData(inputNodes[0].asInt()); \
  double acc = value.isString() ? 0 : value.asDouble();\
  for(int i=1 ; i < inputNodes.size() ; i++) { \
    double a = acc; \
    value = getNodeData(inputNodes[i].asInt()); \
    double b = value.isString()? 0 : value.asDouble(); \
    acc = operation; \
    RNS_LOG_TRACE("node " << inputNodes[i-1] << "," << inputNodes[i] << " = " << a << "," << b << "=" << acc);\
  } \
  return acc; \
}

#define REA_SINGLE(operation) [this](folly::dynamic inputNodes){ \
  /*TODO Handle extraction of numeric value from string value*/\
  REANodeData value = getNodeData(inputNodes[0].asInt()); \
  double a = value.isString() ? 0 : value.asDouble(); \
  return operation; \
}

#define REA_INFIX(operation) [this](folly::dynamic inputNodes){ \
  /*TODO Handle extraction of numeric value from string value*/\
  REANodeData value = getNodeData(inputNodes[0].asInt()); \
  double a = value.isString() ? 0 : value.asDouble(); \
  value = getNodeData(inputNodes[1].asInt()); \
  double b = value.isString() ? 0 : value.asDouble(); \
  return operation; \
}

#define REA_AND_OP [this](folly::dynamic inputNodes){ \
  /*TODO Handle extraction of numeric value from string value*/\
  REANodeData value = getNodeData(inputNodes[0].asInt()); \
  bool res = value.isString() ? 0 : value.asBool(); \
  for(int i=1 ; i < inputNodes.size() && res ; i++) { \
    value = getNodeData(inputNodes[i].asInt()); \
    res = res && (value.isString() ? 0 : value.asDouble()); \
  } \
  return res ? 1.0 : 0.0 ; \
}

#define REA_OR_OP [this](folly::dynamic inputNodes){ \
  /*TODO Handle extraction of numeric value from string value*/\
  REANodeData value = getNodeData(inputNodes[0].asInt()); \
  bool res = value.isString() ? 0 : value.asBool(); \
  for(int i=1 ; i < inputNodes.size() && (!res) ; i++) { \
    value = getNodeData(inputNodes[i].asInt()); \
    res = res || (value.isString() ? 0 : value.asDouble()); \
  } \
  return res ? 1.0 : 0.0 ; \
}

#define REA_DEFINED_OP [this](folly::dynamic inputNodes){ \
  REANodeData val = getNodeData(inputNodes[0].asInt()).asDouble(); \
  return val.isNull(); \
}

REANodeHandle REAOpNodeProvider(REANodeID nodeId,folly::dynamic nodeConfig) {
  return new REAOpNode(nodeId,nodeConfig);
}

REAOpNode::REAOpNode(REANodeID nodeId,folly::dynamic nodeConfig)
  : REANode(nodeId,nodeConfig) {

  RNS_LOG_DEBUG("REAOpNode nodeID:" << nodeId);

  static facebook::better::map<std::string,std::function<REANodeData(folly::dynamic)>> operationList = {
    {"add", REA_REDUCE(a+b)},
    {"sub", REA_REDUCE(a-b)},
    {"multiply", REA_REDUCE(a*b)},
    {"divide", REA_REDUCE(a/b)},
    {"pow", REA_REDUCE(pow(a,b))},
    {"modulo", REA_REDUCE(fmod(fmod(a,b)+b,b))},
    {"sqrt", REA_SINGLE(sqrt(a))},
    {"log", REA_SINGLE(log(a))},
    {"sin", REA_SINGLE(sin(a))},
    {"cos", REA_SINGLE(cos(a))},
    {"tan", REA_SINGLE(tan(a))},
    {"acos", REA_SINGLE(acos(a))},
    {"asin", REA_SINGLE(asin(a))},
    {"atan", REA_SINGLE(atan(a))},
    {"exp", REA_SINGLE(exp(a))},
    {"round", REA_SINGLE(round(a))},
    {"abs", REA_SINGLE(fabs(a))},
    {"ceil", REA_SINGLE(ceil(a))},
    {"floor", REA_SINGLE(floor(a))},
    {"max", REA_REDUCE(std::max(a,b))},
    {"min", REA_REDUCE(std::min(a,b))},
    {"lessThan", REA_INFIX(a < b)},
    {"greaterThan", REA_INFIX(a > b)},
    {"eq", REA_INFIX(a == b)},
    {"neq", REA_INFIX(a != b)},
    {"lessOrEq", REA_INFIX(a <= b)},
    {"greaterOrEq", REA_INFIX(a >= b)},
    {"not", REA_SINGLE(!a)},
    {"and", REA_AND_OP},
    {"or", REA_OR_OP},
    {"defined", REA_DEFINED_OP},
  };

  inputNodes_ = nodeConfig["input"];

  op_ = nodeConfig["op"].asString();
  operation_ = operationList[op_];

  if(!operation_) {
    RNS_LOG_WARN("Operation not found:" << nodeConfig["op"]);
  }

}

REAOpNode::~REAOpNode() {
}

REANodeData REAOpNode::evaluate() {
  RNS_LOG_TRACE("OperationNode nodeID:" << getNodeID() << " op" << op_);
  REANodeData res = operation_(inputNodes_);
  RNS_LOG_TRACE("OperationNode nodeID:" << getNodeID() << " value:" << res);
  return res;

}

}// namespace reanimated
