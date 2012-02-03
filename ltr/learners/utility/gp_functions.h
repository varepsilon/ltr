// Copyright 2011 Yandex School Practice

#ifndef LTR_LEARNERS_UTILITY_GP_FUNCTIONS_H_
#define LTR_LEARNERS_UTILITY_GP_FUNCTIONS_H_

#include <stdexcept>

#include "ltr/data/data_set.h"
#include "contrib/puppy/Puppy.hpp"
#include "ltr/utility/serializable.h"

namespace ltr {
namespace gp {

void setContextToObject(Puppy::Context* context, const Object& obj);

template <typename TElement>
void markDataSetWithTree(const DataSet<TElement>& data,
    Puppy::Context* context, Puppy::Tree* tree);

const Serializable* puppyPrimitiveHandleToPSerializable(
    const Puppy::PrimitiveHandle mPrimitive);

void writeTreeAsStringOfCppCalls(
    const Puppy::Tree& tree,
    std::ostream* pIoOS,
    size_t inIndex);
}
}
#endif  // LTR_LEARNERS_UTILITY_GP_FUNCTIONS_H_
