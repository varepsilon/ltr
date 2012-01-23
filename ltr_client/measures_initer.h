// Copyright 2011 Yandex

#ifndef LTR_CLIENT_MEASURES_INITER_H_
#define LTR_CLIENT_MEASURES_INITER_H_

#include <string>
#include <map>

#include "ltr_client/constants.h"
#include "ltr_client/measures_info.h"

/**
@class MeasureIniter
This class is used to init measure of given type.
*/
class MeasureIniter {
    public:
        MeasureIniter();
        /**
        Function creates measure object.
        @param type - type of measure to create.
        @param parameters - parameters to create measure.
        */
        VMeasureInfo init(const std::string& type,
                          const ltr::ParametersContainer& parameters);

    private:
        typedef VMeasureInfo (*TMeasureIniter)(const ltr::ParametersContainer&);
        map<std::string, TMeasureIniter> initers;
};

#endif  // LTR_CLIENT_MEASURES_INITER_H_
