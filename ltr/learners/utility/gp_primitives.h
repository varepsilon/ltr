// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_UTILITY_GP_PRIMITIVES_H_
#define LTR_LEARNERS_UTILITY_GP_PRIMITIVES_H_

#include <cmath>
#include <string>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/utility/functor.h"

using std::string;

namespace ltr {
namespace gp {

class Add : public Puppy::Primitive {
  public:
  Add() : Primitive(2, "ADD") {}
  virtual ~Add() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(0, &lResult, ioContext);
    getArgument(1, &lArg2, ioContext);
    lResult += lArg2;
  }
};

class Subtract : public Puppy::Primitive {
  public:
  Subtract() : Primitive(2, "SUB") {}
  virtual ~Subtract() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(0, &lResult, ioContext);
    getArgument(1, &lArg2, ioContext);
    lResult -= lArg2;
  }
};

class Multiply : public Puppy::Primitive {
  public:
  Multiply() : Primitive(2, "MUL") {}
  virtual ~Multiply() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(0, &lResult, ioContext);
    getArgument(1, &lArg2, ioContext);
    lResult *= lArg2;
  }
};

class Divide : public Puppy::Primitive {
  public:
  Divide() : Primitive(2, "DIV") {}
  virtual ~Divide() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(1, &lArg2, ioContext);
    if (std::fabs(lArg2) < 0.001) {
      lResult = 1.0;
    } else {
      getArgument(0, &lResult, ioContext);
      lResult /= lArg2;
    }
  }
};

class IfThenFunc : public Puppy::Primitive {
  public:
  IfThenFunc() : Primitive(3, "IF") {}
  virtual ~IfThenFunc() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lFirstArg;
    getArgument(0, &lFirstArg, ioContext);
    if (lFirstArg > 0.0) {
      getArgument(1, &lResult, ioContext);
    } else {
      getArgument(2, &lResult, ioContext);
    }
  }
};

class Ephemeral : public Puppy::Primitive {
  public:
  Ephemeral() : Primitive(0, "E") {}
  virtual ~Ephemeral() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {}
  virtual Puppy::PrimitiveHandle giveReference(
      Puppy::Context& ioContext) {
    double lValue = ioContext.mRandom.rollUniform(-1.0, 1.0);
    std::ostringstream lOSS;
    lOSS << lValue;
    return new Puppy::TokenT<double>(lOSS.str(), lValue);
  }
};
}
}
#endif  // LTR_LEARNERS_UTILITY_GP_PRIMITIVES_H_
