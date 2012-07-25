// Copyright 2011 Yandex

#ifndef LTR_LEARNERS_GP_LEARNER_GP_PRIMITIVES_H_
#define LTR_LEARNERS_GP_LEARNER_GP_PRIMITIVES_H_

#include <cmath>
#include <string>

#include "contrib/puppy/Puppy.hpp"

#include "ltr/interfaces/serializable.h"

#include "ltr/utility/shared_ptr.h"

using std::string;

using boost::lexical_cast;

namespace ltr {
namespace gp {
class BaseGPOperation : public Puppy::Primitive {
 public:
  typedef ltr::utility::shared_ptr<BaseGPOperation> Ptr;

  BaseGPOperation(int number_of_arguments, string name)
  : Primitive(number_of_arguments, name) {}
};
/** \brief The Puppy::Primitive implements addition functor, it is used to
 * build Puppy::trees from.
 */
class Add : public BaseGPOperation, public Serializable {
 public:
  Add() : BaseGPOperation(2, "ADD") {}
  virtual ~Add() {}
  /** The implementation of addition functor.
   */
  virtual void execute(void* output,
      Puppy::Context& puppy_context) {
    double& first_argument = *(static_cast<double*>(output));
    double second_argument;
    getArgument(0, &first_argument, puppy_context);
    getArgument(1, &second_argument, puppy_context);
    first_argument += second_argument;
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double rhs) { return lhs + rhs;}\n";
  }
};
/** \brief The Puppy::Primitive implements subtraction functor, it is used to
 * build Puppy::trees from.
 */
class Subtract : public BaseGPOperation, public Serializable {
 public:
  Subtract() : BaseGPOperation(2, "SUB") {}
  virtual ~Subtract() {}
  /** The implementation of subtraction functor.
   */
  virtual void execute(void* output,
      Puppy::Context& puppy_context) {
    double& first_argument = *(static_cast<double*>(output));
    double second_argument;
    getArgument(0, &first_argument, puppy_context);
    getArgument(1, &second_argument, puppy_context);
    first_argument -= second_argument;
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double rhs) { return lhs - rhs;}\n";
  }
};
/** \brief The Puppy::Primitive implements multiplication functor, it is used to
 * build Puppy::trees from.
 */
class Multiply : public BaseGPOperation, public Serializable {
 public:
  Multiply() : BaseGPOperation(2, "MUL") {}
  virtual ~Multiply() {}
  /** The implementation of multiplication functor.
   */
  virtual void execute(void* output,
      Puppy::Context& puppy_context) {
    double& first_argument = *(static_cast<double*>(output));
    double second_argument;
    getArgument(0, &first_argument, puppy_context);
    getArgument(1, &second_argument, puppy_context);
    first_argument *= second_argument;
  }
  /** The function returns a string with the functor as cpp code. It is used
   * for serialization GPScorer to cpp code.
   */
  string generateCppCode(const string& function_name) const {
    return "inline double " + function_name +
      "(double lhs, double rhs) { return lhs * rhs;}\n";
  }
};
/** \brief The Puppy::Primitive implements division functor, it is used to
 * build Puppy::trees from.
 */
class Divide : public BaseGPOperation, public Serializable {
  static const double SAFE_DIVISION_EPS;
 public:
  Divide() : BaseGPOperation(2, "DIV") {}
  virtual ~Divide() {}
  /** The implementation of division functor.
   */
  virtual void execute(void* output,
      Puppy::Context& puppy_context) {
    double& first_argument = *(static_cast<double*>(output));
    double second_argument;
    getArgument(1, &second_argument, puppy_context);
    if (std::fabs(second_argument) < SAFE_DIVISION_EPS) {
      first_argument = 1.0;
    } else {
      getArgument(0, &first_argument, puppy_context);
      first_argument /= second_argument;
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
    code.append(lexical_cast<string>(SAFE_DIVISION_EPS));
    code.append(") {\n");
    code.append("    return 1.0;\n");
    code.append("  } else {\n");
    code.append("    return lhs / rhs;\n");
    code.append("  }\n");
    code.append("}\n");
    return code;
  }
};
/** \brief The Puppy::Primitive implements if-then-else functor, it is used to
 * build Puppy::trees from.
 */
class IfThenFunc : public BaseGPOperation, public Serializable {
  public:
  IfThenFunc() : BaseGPOperation(3, "IF") {}
  virtual ~IfThenFunc() {}
  /** The implementation of if-then-else functor functor.
   */
  virtual void execute(void* output,
      Puppy::Context& puppy_context) {
    double first_argument;
    getArgument(0, &first_argument, puppy_context);
    if (first_argument > 0.0) {
      getArgument(1, static_cast<double*>(output), puppy_context);
    } else {
      getArgument(2, static_cast<double*>(output), puppy_context);
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
/** \brief The Puppy::Primitive implements the functor, that gives random
 * constants to build Puppy::Tree.
 */
class Ephemeral : public BaseGPOperation {
  static const size_t precision = 20;
  public:
  Ephemeral() : BaseGPOperation(0, "E") {}
  virtual ~Ephemeral() {}
  virtual void execute(void* output,
      Puppy::Context& puppy_context) {}
  virtual Puppy::PrimitiveHandle giveReference(
      Puppy::Context& puppy_context) {
    double value = puppy_context.mRandom.rollUniform(-1.0, 1.0);
    std::ostringstream name;
    name.precision(precision);
    name << value;
    return new Puppy::TokenT<double>(name.str(), value);
  }
};
};
};
#endif  // LTR_LEARNERS_GP_LEARNER_GP_PRIMITIVES_H_
