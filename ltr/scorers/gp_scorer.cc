// Copyright 2012 Yandex

#include <logog/logog.h>

#include "ltr/scorers/gp_scorer.h"

namespace ltr {
namespace gp {
double GPScorer::scoreImpl(const Object& object) const {
  INFO("Starting to score an object with the gp_learner");
  assert(feature_count_ == object.feature_count());
  setContextToObject(&context_, object);
  double score;
  best_tree_.interpret(&score, context_);
  return score;
}

string GPScorer::generateCppCodeImpl(const string& function_name) const {
  string code;
  code.append("#include <vector>\n");
  // generate primitive-functions code.
  vector<Puppy::PrimitiveHandle>::const_iterator function_iterator =
    context_.mFunctionSet.begin();
  for (; function_iterator != context_.mFunctionSet.end();
       ++function_iterator) {
    const Serializable* serializable = puppyPrimitiveHandleToPSerializable(
      *function_iterator);
    string primirive_function_name =
      serializable->getDefaultSerializableObjectName();
    primirive_function_name += (*function_iterator)->getName();
    code.append(serializable->generateCppCode(primirive_function_name));
  }
  // generate the function from tree.
  stringstream sstream_for_calls;
  writeTreeAsStringOfCppCalls(best_tree_, sstream_for_calls, 0);
  // generate scoring function
  code.append("double ");
  code.append(function_name);
  code.append("(const std::vector< double >& feature) {\n");
  code.append("  return ");
  code.append(sstream_for_calls.str());
  code.append(";\n");
  code.append("}\n");

  return code;
}
}
}
