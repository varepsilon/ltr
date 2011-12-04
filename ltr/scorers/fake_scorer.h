// Copyright 2011 Yandex
#pragma once

#include "ltr/scorers/scorer.h"

namespace ltr
{
  /**
   * Fake Ranker. Simply always return 0.0.
   */
  class FakeScorer : public IScorer {
  public:
    typedef boost::shared_ptr< FakeScorer > Ptr;
    
    FakeScorer() : IScorer("FakeScorer") {
    }
    virtual std::string brief() const;
    virtual double operator()(const Object& obj) const;
    virtual std::string generateCppCode(const std::string& class_name, int tabbing) const;
    virtual std::string generateJavaCode(const std::string& class_name, int tabbing, bool is_static) const;
  };
};
