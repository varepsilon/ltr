// Copyright 2011 Yandex

#ifndef LTR_DATA_UTULITY_OBJECT_UTILITY_H_
#define LTR_DATA_UTULITY_OBJECT_UTILITY_H_

#include "ltr/utility/numerical.h"

namespace ltr {

bool equalWithNaN(const Features& a, const Features& b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (size_t i = 0; i < a.size(); ++i) {
        if (!utility::equalWithNaN(a[i], b[i])) return false;
    }
    return true;
}

class LessPredicted {
    public:
    bool operator()(const Object& first, const Object& second) const {
        return first.predictedLabel() < second.predictedLabel();
    };
};

class LessActual {
    public:
    bool operator()(const Object& first, const Object& second) const {
        return first.actualLabel() < second.actualLabel();
    };
};

class GreaterPredicted {
    public:
    bool operator()(const Object& first, const Object& second) const {
        return first.predictedLabel() > second.predictedLabel();
    };
};

class GreaterActual {
    public:
    bool operator()(const Object& first, const Object& second) const {
        return first.actualLabel() > second.actualLabel();
    };
};

}

#endif /* LTR_DATA_UTULITY_OBJECT_UTILITY_H_ */
