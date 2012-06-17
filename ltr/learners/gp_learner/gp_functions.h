// Copyright 2011 Yandex School Practice

#ifndef LTR_LEARNERS_GP_LEARNER_GP_FUNCTIONS_H_
#define LTR_LEARNERS_GP_LEARNER_GP_FUNCTIONS_H_

#include <stdexcept>

#include "ltr/data/data_set.h"
#include "contrib/puppy/Puppy.hpp"
#include "ltr/interfaces/serializable.h"

namespace ltr {
namespace gp {

/**
* The function sets up the values for primitives that represent feature
* values to the concrete values from the given object.
* \param context pointer to the context to set up primitive values in.
* \param object, whose feature values would be set up.
*/
void setContextToObject(Puppy::Context* context, const Object& object);

/**
* The function sets predicted labels in the dataset to the values calculated
* using the Puppy::Tree(formula, individ).
* \param data dataset to mark up.
* \param tree pointer to the Puppy::Tree(formula, individ) to mark up with.
*/
template <typename TElement>
void markDataSetWithTree(const DataSet<TElement>& data,
    Puppy::Context* context, Puppy::Tree* tree);

/** 
* The function casts the Puppy::PrimitiveHandle to the const Serializable*
* using dynamic_cast. If the given object does't implement Serializable
* interface a std::logic_error is thrown. The function is used within
* serialization to Cpp code.
*/
const Serializable* puppyPrimitiveHandleToPSerializable(
    const Puppy::PrimitiveHandle puppy_primitive);
/** 
* The function writes down the given Puppy::Tree(formala, individ) as line
* of cpp function calls. The function is used within serialization to Cpp
* code.
*/
void writeTreeAsStringOfCppCalls(
    const Puppy::Tree& tree,
    std::ostream& output,
    size_t node_index);
}
}
#endif  // LTR_LEARNERS_GP_LEARNER_GP_FUNCTIONS_H_
