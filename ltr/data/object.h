// Copyright 2011 Yandex

#ifndef LTR_DATA_OBJECT_H_
#define LTR_DATA_OBJECT_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <map>
#include <string>

using std::map;
using std::string;

namespace ltr {

typedef std::vector< double > Features;
typedef map<string, string> MetaInfo;
class Object {
  public:
  typedef boost::shared_ptr< Object > Ptr;

  Object();
  Object(const Object& object);
  /**
   * makes object from the first object in the vector
   */
  Object(const std::vector<Object>& objects);

  const Features& features() const;
  Features& features();

  const string& getMetaInfo(string name) const;
  void setMetaInfo(string name, string value);
  const MetaInfo& metaInfo() const;
  MetaInfo& metaInfo();


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
  boost::shared_ptr<map<string, string> > meta_info_;
  double actual_label_;
  mutable double predicted_label_;
};
bool operator==(const Object& ob1, const Object& ob2);
bool operator!=(const Object& ob1, const Object& ob2);
}

#endif  // LTR_DATA_OBJECT_H_
