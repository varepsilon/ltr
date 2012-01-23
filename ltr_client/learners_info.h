// Copyright 2011 Yandex School Practice

#ifndef LTR_CLIENT_LEARNERS_INFO_H_
#define LTR_CLIENT_LEARNERS_INFO_H_

#include <string>
#include "boost/variant.hpp"
#include "ltr/learners/learner.h"
#include "ltr_client/measures_info.h"


using std::string;

/**
@class LearnerInfo
Class contains all necessary information about one learner.
*/
template< class TElement>
struct LearnerInfo {
  typename ltr::BaseLearner<TElement>::Ptr learner;
  typename ltr::Measure<TElement>::Ptr measure;
  ltr::ParametersContainer parameters;
  string weak_learner;
  string measure_name;
  string type;
  string approach;
};

typedef boost::variant<LearnerInfo<ltr::ObjectList>,
                       LearnerInfo<ltr::ObjectPair>,
                       LearnerInfo<ltr::Object> > VLearnerInfo;
#endif  // LTR_CLIENT_LEARNERS_INFO_H_
