// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "ltr/interfaces.h"

using std::vector;

namespace ltr {

class Object;

/**
 * Score objects (not pairs or lists!). Is output of all learners.
 */
class Scorer : public Aliaser {
    public:
    typedef boost::shared_ptr<Scorer> Ptr;

    /**
     * Returns object's score.
     */
    virtual double operator()(const Object& obj) const = 0;

    /**
     * Generates C++ header-only code.
     * @param class_name - class name of generated class that MUST be used
     * @param tabbing - count of tabs should be pasted of beginning of each line of generated code
     */
    virtual std::string generateCppCode(
        const std::string& class_name, int tabbing = 0) const = 0;

    /**
     * Generates Java code.
     * @param class_name - class name of generated class that MUST be used
     * @param tabbing - count of tabs should be pasted of beginning of each line of generated code
     * @param is_static - if true, qualifier 'static' must be set for generated class (used fo embedded classes)
     */
    virtual std::string generateJavaCode(const std::string& class_name,
		int tabbing = 0, bool is_static = false) const = 0;

    virtual ~Scorer() {}
};
};

#endif // LTR_SCORERS_SCORER_H_
