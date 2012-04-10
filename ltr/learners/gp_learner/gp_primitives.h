// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_GP_PRIMITIVES_H_
#define LTR_LEARNERS_GP_LEARNER_GP_PRIMITIVES_H_

#include <cmath>
#include <string>

#include "contrib/puppy/Puppy.hpp"
#include "ltr/interfaces/serializable.h"

using std::string;
using boost::lexical_cast;

namespace ltr {
namespace gp {
/** \class The Puppy::Primitive implements addition functor, it is used to
 * build Puppy::trees from.
 */
class Add : public Puppy::Primitive, public Serializable {
  public:
  Add() : Primitive(2, "ADD") {}
  virtual ~Add() {}
  /** The implementation of addition functor.
   */
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(0, &lResult, ioContext);
    getArgument(1, &lArg2, ioContext);
    lResult += lArg2;
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double rhs) { return lhs + rhs;}\n";
  }
};
/** \class The Puppy::Primitive implements subtraction functor, it is used to
 * build Puppy::trees from.
 */
class Subtract : public Puppy::Primitive, public Serializable {
  public:
  Subtract() : Primitive(2, "SUB") {}
  virtual ~Subtract() {}
  /** The implementation of subtraction functor.
   */
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(0, &lResult, ioContext);
    getArgument(1, &lArg2, ioContext);
    lResult -= lArg2;
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double rhs) { return lhs - rhs;}\n";
  }
};
/** \class The Puppy::Primitive implements multiplication functor, it is used to
 * build Puppy::trees from.
 */
class Multiply : public Puppy::Primitive, public Serializable {
  public:
  Multiply() : Primitive(2, "MUL") {}
  virtual ~Multiply() {}
  /** The implementation of multiplication functor.
   */
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(0, &lResult, ioContext);
    getArgument(1, &lArg2, ioContext);
    lResult *= lArg2;
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double rhs) { return lhs * rhs;}\n";
  }
};
/** \class The Puppy::Primitive implements division functor, it is used to
 * build Puppy::trees from.
 */
class Divide : public Puppy::Primitive, public Serializable {
  static const double safeDivisionEps;

  public:
  Divide() : Primitive(2, "DIV") {}
  virtual ~Divide() {}
  /** The implementation of division functor.
   */
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {
    double& lResult = *(static_cast<double*>(outDatum));
    double lArg2;
    getArgument(1, &lArg2, ioContext);
    if (std::fabs(lArg2) < safeDivisionEps) {
      lResult = 1.0;
    } else {
      getArgument(0, &lResult, ioContext);
      lResult /= lArg2;
    }
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    string code;
    code.append("#include <cmath>\n");
    code.append("inline double ");
    code.append(function_name);
    code.append("(double lhs, double rhs) {\n");
    code.append("  if (std::fabs(rhs) < ");
    code.append(lexical_cast<string>(safeDivisionEps));
    code.append(") {\n");
    code.append("    return 1.0;\n");
    code.append("  } else {\n");
    code.append("    return lhs / rhs;\n");
    code.append("  }\n");
    code.append("}\n");
    return code;
  }
};
/** \class The Puppy::Primitive implements if-then-else functor, it is used to
 * build Puppy::trees from.
 */
class IfThenFunc : public Puppy::Primitive, public Serializable {
  public:
  IfThenFunc() : Primitive(3, "IF") {}
  virtual ~IfThenFunc() {}
  /** The implementation of if-then-else functor functor.
   */
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
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double mhs, double rhs) {\n" +
      "  if (lhs > 0.0) {\n" +
      "    return mhs;\n" +
      "  } else {\n" +
       "    return rhs;\n" +
      "  }\n" +
      "}\n";
  }
};
/** \class The Puppy::Primitive implements the functor, that gives random
 * constants to build Puppy::Tree.
 */
class Ephemeral : public Puppy::Primitive {
  static const size_t doubleTokenPrecision = 20;
  public:
  Ephemeral() : Primitive(0, "E") {}
  virtual ~Ephemeral() {}
  virtual void execute(void* outDatum,
      Puppy::Context& ioContext) {}
  virtual Puppy::PrimitiveHandle giveReference(
      Puppy::Context& ioContext) {
    double lValue = ioContext.mRandom.rollUniform(-1.0, 1.0);
    std::ostringstream lOSS;
    lOSS.precision(doubleTokenPrecision);
    lOSS << lValue;
    return new Puppy::TokenT<double>(lOSS.str(), lValue);
  }
};
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_GP_PRIMITIVES_H_
