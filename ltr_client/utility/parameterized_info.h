// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_PARAMETERIZED_INFO_H_
#define LTR_CLIENT_UTILITY_PARAMETERIZED_INFO_H_

#include <list>
#include <string>
#include <sstream>

#include "ltr/parameters_container/parameters_container.h"

using std::string;
using std::stringstream;

class ParametrizedInfo;
typedef std::list<const ParametrizedInfo*> ParametrizedInfosList;

/**
 * Contains information about any object that inherits
 * the Parameterized interface including all the learners,
 * splitters, measures, metrics, scorers that was initialized
 * during the LTR Client session.
 */
class ParametrizedInfo {
 public:
  /**
   * Dictionary with string keys and the
   * ParameterizedInfo stored type
   */
  typedef boost::unordered_map<string, ParametrizedInfo> ParametrizedInfos;
  /**
   * Basic constructor.
   * @returns ParameterizedInfo constructed object.
   */
  ParametrizedInfo();
  /**
   * Another constructor implementation.
   * @param category is the type of base class
   * @param alias is the name of the entity
   * @param type is the type of the derived class
   * @returns ParameterizedInfo constructed object.
   */
  ParametrizedInfo(const string& category,  // e.g. Measure, Learner
                   const string& alias,     // e.g. NDCGlistwise
                   const string& type,      // e.g. NDCG, KNN
                   const string& approach,  // e.g. Listwise, Pointwise
                   ltr::ParametersContainer parameters);
  /**
   * Deep copy constructor.
   * @returns ParameterizedInfo constructed object.
   */
  ParametrizedInfo(const ParametrizedInfo& other);
  /**
   * Deep copy '=' operator.
   * @returns ParameterizedInfo link to the modified object.
   */
  ParametrizedInfo& operator=(const ParametrizedInfo& other);
  /**
   * Basic destructor.
   */
  ~ParametrizedInfo();

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
   * @returns the constant link to the ParametrizedInfos that
   * contains dependency specifications.
   */
  const ParametrizedInfosList& dependency_specs() const;
  /**
   * Fills the dependency list.
   * @param token_specifications - ParametrizedInfos constant object
   * link that contains all the registered objects.
   */
  void fill_dependency_list(
      const ParametrizedInfos& token_specifications);

 private:
  string tag_name_;
  string object_name_;
  string object_type_;
  string approach_;
  ltr::ParametersContainer parameters_;
  ParametrizedInfosList dependency_specs_;

  friend class ConfigParser;
  friend class OnGeneralParameterized;
};
/**
 * Converts the ParameterizedInfo object into the
 * printable string.
 * @param Info - ParameterizedInfo to convert
 * @returns string with converted ParameterizedInfo
 */
string ToString(const ParametrizedInfo& Info);

#endif  // LTR_CLIENT_UTILITY_PARAMETERIZED_INFO_H_
