// Copyright 2011 Yandex

#ifndef LTR_DATA_OBJECT_H_
#define LTR_DATA_OBJECT_H_

#include <boost/shared_ptr.hpp>

#include <vector>

namespace ltr {

typedef std::vector< double > Features;

class Object {
    public:
    typedef boost::shared_ptr< Object > Ptr;

    Object();

    const Features& features() const;
    Features& features();
    int queryId() const;
    void setQuieryId(int id);

    Object& operator<<(double feature);

    const Object& operator[](size_t i) const;
    Object& operator[](size_t i);
    const Object& at(const size_t i)const;
    Object& at(const size_t i);

    Object& operator=(const Object& other);

    size_t featureCount() const;

    double actualLabel() const;
    double predictedLabel() const;
    void setActualLabel(double label);
    void setPredictedLabel(double label) const;

    Object deepCopy() const;

    /*
     * Returns always 1.
     * */
    size_t size() const;

    friend bool operator==(const Object& o1, const Object& o2);

    private:
    boost::shared_ptr<Features> features_;
    double actual_label_;
    int qid_;
    mutable double predicted_label_;
};

bool operator==(const Object& o1, const Object& o2);
bool operator!=(const Object& o1, const Object& o2);
std::ostream& operator<<(std::ostream& istr, const Object& obj);
}

#endif  // LTR_DATA_OBJECT_H_
