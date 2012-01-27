// Copyright 2011 Yandex

#include <boost/algorithm/string.hpp>

#include "ltr_client/measures_initer.h"
#include "ltr_client/visitors.h"

#include "ltr/measures/abs_error.h"

#include "ltr/measures/dcg.h"

template<> MeasureInfo<ltr::Object>::MeasureInfo() {
    approach = PW;
}
template<> MeasureInfo<ltr::ObjectPair>::MeasureInfo() {
    approach = PRW;
}
template<> MeasureInfo<ltr::ObjectList>::MeasureInfo() {
    approach = LW;
}

MeasureIniter::MeasureIniter() {
}

VMeasureInfo MeasureIniter::init(const std::string& type_,
                                 const ltr::ParametersContainer& parameters) {
  std::string type = type_;
  boost::to_upper(type);
  if (initers.find(type) == initers.end())
      throw std::logic_error("unknown measure " + type);
  VMeasureInfo info = initers[type](parameters);

  boost::apply_visitor(SetApproachVisitor(), info);
  boost::apply_visitor(SetTypeVisitor(type), info);
  return info;
}

