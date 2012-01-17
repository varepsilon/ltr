// Copyright 2011 Yandex

#ifndef LTR_CLIENT_TRAIN_VISITOR_H_
#define LTR_CLIENT_TRAIN_VISITOR_H_

#include <string>

#include "ltr_client/constants.h"
#include "ltr_client/learners_info.h"
#include "ltr_client/datas_info.h"
#include "ltr_client/measures_info.h"

#include "boost/variant.hpp"

class LtrClient;

/**
@class TrainVisitor
This class is used to run right train function for VLearnerInfo and VDataInfo
*/
class TrainVisitor : public boost::static_visitor<void> {
    mutable LtrClient* client;
    public:
        std::string name;
        TrainVisitor(string name, LtrClient* client = 0) : name(name),
            client(client) {}
        template<class TElement1, class TElement2>
        inline void operator()(LearnerInfo<TElement1>&, DataInfo<TElement2>&) const {}

        template<class TElement>
        inline void operator()(LearnerInfo<TElement>& l, DataInfo<TElement>&d) const;
};

template<class TElement>
inline void TrainVisitor::operator()(LearnerInfo<TElement>& l,
                              DataInfo<TElement>&d) const {
    if (client)
        client->train<TElement>(name, l, d);
}
#endif  // LTR_CLIENT_TRAIN_VISITOR_H_
