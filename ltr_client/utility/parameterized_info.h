// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_PARAMETERIZED_INFO_H_
#define LTR_CLIENT_UTILITY_PARAMETERIZED_INFO_H_

#include <list>
#include <string>
#include <sstream>
#include <map>

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::stringstream;

class ObjectInfo;
typedef std::list<const ObjectInfo*> ObjectInfosList;

/**
 * Contains information about any object that inherits
 * the Parameterized interface including all the learners,
 * splitters, measures, metrics, scorers that was initialized
 * during the LTR Client session.
 */
class ObjectInfo {
 public:
  /**
   * Dictionary with string keys and the
   * ObjectInfo stored type
   */
  typedef std::map<string, ObjectInfo> ObjectInfos;
  /**
   * Basic constructor.
   * @returns ObjectInfo constructed object.
   */
  ObjectInfo();
  /**
   * Another constructor implementation.
   * @param category is the type of base class
   * @param alias is the name of the entity
   * @param type is the type of the derived class
   * @returns ObjectInfo constructed object.
   */
  ObjectInfo(const string& category,  // e.g. Measure, Learner
                   const string& alias,     // e.g. NDCGlistwise
                   const string& type,      // e.g. NDCG, KNN
                   const string& approach,  // e.g. Listwise, Pointwise
                   ltr::ParametersContainer parameters);
  /**
   * Deep copy constructor.
   * @returns ObjectInfo constructed object.
   */
  ObjectInfo(const ObjectInfo& other);
  /**
   * Deep copy '=' operator.
   * @returns ObjectInfo link to the modified object.
   */
  ObjectInfo& operator=(const ObjectInfo& other);
  /**
   * Basic destructor.
   */
  ~ObjectInfo();

  /**
   * @returns constant link to the name of the tag where
   * the Parameterized object has been initialized.
   */
  const string& get_tag_name() const;
  /**
   * @returns the constant link to the string, containing
   * the name of the Parameterized object.
   */
  const string& get_name() const;
  /**
   * @returns the constant link to the string, containing
   * the type of the Parameterized object.
   */
  const string& get_type() const;
  /**
   * @returns the constant link to the string, containing
   * the approach type of the Parameterized object.
   */
  const string& get_approach() const;
  /**
   * @returns the constant link to the dictionary ParamaetersContainer
   * where all the parameters of the parameterized object are stored.
   */
  const ltr::ParametersContainer& get_parameters() const;
  /**
   * @returns the constant link to the ObjectInfos that
   * contains dependency specifications.
   */
  const ObjectInfosList& dependency_infos() const;
  /**
   * Fills the dependency list.
   * @param object_infos - ObjectInfos constant object
   * link that contains all the registered objects.
   */
  void fill_dependency_list(
      const ObjectInfos& object_infos);

 private:
  string tag_name_;
  string object_name_;
  string object_type_;
  string approach_;
  ltr::ParametersContainer parameters_;
  ObjectInfosList dependency_infos_;

  friend class ConfigParser;
  friend class OnGeneralParameterized;
};
/**
 * Converts the ObjectInfo object into the
 * printable string.
 * @param Info - ObjectInfo to convert
 * @returns string with converted ObjectInfo
 */
string ToString(const ObjectInfo& Info);

#endif //LTR_CLIENT_UTILITY_PARAMETERIZED_INFO_H_

