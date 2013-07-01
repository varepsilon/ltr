// Copyright 2012 Yandex

#ifndef LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_
#define LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_

#include <string>

#include "ltr/utility/macros.h"
#include "ltr_client/configuration.h"
#include "ltr_client/factory.h"
#include "ltr_client/utility/object_info.h"

using std::string;

/**
 * Manages object creation and checks for circular dependencies
 */
class DependencyResolvingFactory {
 public:
  DependencyResolvingFactory() {}
  /**
   * Checks for circular dependencies. Throws exception if found
   */
  void checkCircularDependencyAbsence() const;
  /**
   * Creates an object
   * @param name - string with the object name
   */
  Any Create(const string& name) const;
  /**
   * Reads object names stored in ParametersContainer, creates these
   * objects and returns a ParametersContainer with created objects
   * @param parameters - ParametersContainer with object names
   */
  ParametersContainer Create(const ParametersContainer& parameters) const;
  /**
   * Creates an object, declared in config
   *
   * \param object_name - string with the object name
   * \param type_name - name of the requested type for checking tags
   */
  template <typename RequestedType>
  RequestedType CreateObject(const string& object_name,
                             const string& type_name);
  /**
   * Initializes factory by given configuration.
   *
   * \param configuration - initializing configuration
   */
  void init(const Configuration::Ptr configuration);

 protected:
  /**
   * Generates a name for an object
   * @param object_info - all the object's properties
   */
  static string getObjectAlias(const ObjectInfo::Ptr object_info);

 private:
  bool tryBuildingObjectCreationChain(
      const ObjectInfo::Ptr object_info,
      ObjectInfosList* info_queue,
      ObjectInfosList* circularity_check_queue) const;

  Configuration::Ptr configuration_;
};

// TODO(skyhawk): do this function constant by using const_shared_ptr
template <typename RequestedType>
RequestedType DependencyResolvingFactory::CreateObject(
    const string& object_name,
    const string& type_name) {
  ObjectInfo::Ptr object_info;
  try {
    object_info = configuration_->object_infos.safeAt(object_name);
  } catch (logic_error) {
    throw logic_error("Object " + object_name +" is used but not declared");
  }

  CHECK_MSG(type_name == object_info->tag_name, "Object " + object_name +
    " is declared as " + object_info->tag_name + " but used as " + type_name);
  ParametersContainer parameters = Create(object_info->parameters);
  Any object = Factory::instance()->Create(getObjectAlias(object_info),
                                           parameters);
  try {
    return any_cast<RequestedType>(object);
  } catch (bad_any_cast exception) {
    throw logic_error("Object " + object_info->name +
        " of type " + object_info->type + " declared as " +
        object_info->tag_name + " but it's actually not");
  }
}

#endif  // LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_
