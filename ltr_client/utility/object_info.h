// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_OBJECT_INFO_H_
#define LTR_CLIENT_UTILITY_OBJECT_INFO_H_

#include <list>
#include <string>

#include "ltr/parameters_container/parameters_container.h"
#include "ltr/utility/safe_map.hpp"
#include "ltr/utility/macros.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/interfaces/printable.h"

using std::string;
using std::list;

using ltr::utility::SafeMap;
using ltr::Printable;
using ltr::ParametersContainer;
using ltr::utility::shared_ptr;

class ObjectInfo;
typedef list<shared_ptr<ObjectInfo> > ObjectInfosList;

/**
 * \brief Contains information about any object that inherits
 * the Parameterized interface including all the learners,
 * splitters, measures, metrics, scorers that was initialized
 * during the LTR Client session.
 */
struct ObjectInfo: public Printable {
  ALLOW_SHARED_PTR_ONLY_CREATION(ObjectInfo);
 public:
  string toString() const;
  /**
   * Dictionary with string keys and the ObjectInfo stored type
   */
  typedef SafeMap<string, ObjectInfo::Ptr> ObjectInfos;
  /**
   * Basic constructor.
   */
  ObjectInfo() { }
  /**
   * Another constructor implementation.
   *
   * \param category is a type of the base class
   * \param alias is a name of the entity
   * \param type is a type of the derived class
   * \param approach is an approach of the derived class
   * \param parameters is a container of parameters
   * \returns ObjectInfo constructed object.
   */
  ObjectInfo(const string& category,  // e.g. Measure, Learner
             const string& alias,     // e.g. NDCGlistwise
             const string& type,      // e.g. NDCG, KNN
             const string& approach,  // e.g. Listwise, Pointwise
             ParametersContainer parameters);
  /**
   * Deep copy constructor.
   *
   * \returns ObjectInfo constructed object.
   */
  ObjectInfo(const ObjectInfo& other);
  /**
   * Deep copy '=' operator.
   *
   * \returns ObjectInfo link to the modified object.
   */
  ObjectInfo& operator=(const ObjectInfo& other);

  /**
   * Fills the dependency list.
   *
   * \param object_infos - ObjectInfos constant object
   * link that contains all the registered objects.
   */
  void fill_dependency_list(const ObjectInfos& object_infos);

  /**
   * Name of the tag (e.g. "learner")
   */
  string tag_name;
  /**
   * Name of the object (e.g. "BF1")
   */
  string name;
  /**
   * Type of the object (e.g. "BestFeatureLearner")
   */
  string type;
  /**
   * Approach of the object (e.g. "listwise")
   */
  string approach;
  /**
   * Container with parameters
   */
  ParametersContainer parameters;
  /**
   * Info about dependences of this object
   */
  ObjectInfosList dependency_infos;
};

#endif  // LTR_CLIENT_UTILITY_OBJECT_INFO_H_
