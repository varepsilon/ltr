// Copyright 2011 Yandex School Practice

#ifndef LTR_LEARNERS_UTILITY_GP_FUNCTIONS_H_
#define LTR_LEARNERS_UTILITY_GP_FUNCTIONS_H_

#include "ltr/data/data_set.h"
#include "contrib/puppy/Puppy.hpp"

namespace ltr {
namespace gp {

void setContextToObject(Puppy::Context* context, const Object& obj);

template <typename TElement>
void markDataSetWithTree(const DataSet<TElement>& data,
    Puppy::Context* context, Puppy::Tree* tree);
}
}

#endif  // LTR_LEARNERS_UTILITY_GP_FUNCTIONS_H_
