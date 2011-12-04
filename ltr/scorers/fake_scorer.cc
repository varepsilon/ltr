// Copyright 2011 Yandex
#include "fake_scorer.h"

namespace ltr
{
  std::string FakeScorer::generateCppCode(const std::string& class_name, int tabbing) const
  {
    std::string hpp_code;

    std::string tab_str(tabbing, '\t');

    hpp_code.
      append(tab_str).
        append("class ").
        append(class_name).
        append(" {\n").
      append(tab_str).
        append("public:\n").
      append(tab_str).
        append("\tstatic double score(const std::vector< double >& features) { return 0.0; }\n").
      append(tab_str).
        append("};\n");

    return hpp_code;
  }

  std::string FakeScorer::generateJavaCode(const std::string& class_name, int tabbing, bool is_static) const
  {
    std::string java_code;

    std::string tab_str(tabbing, '\t');

    java_code.
      append(tab_str).
        append("public ").
        append(std::string(is_static ? "static " : "" ) + "class ").
        append(class_name).
        append(" {\n").
      append(tab_str).
        append("\tpublic static double score(Vector<Double> features) { return 0.0; }\n").
      append(tab_str).
        append("}\n");

    return java_code;
  }

  std::string FakeScorer::brief() const {
    return "all ranks are 0.";
  }
    
  double FakeScorer::operator()(const Object& obj) const {
    return 0.0;
  }
};
