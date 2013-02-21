// Copyright 2012 Yandex

#ifndef LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_
#define LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_

#include <string>

#include "ltr_client/configurator.h"
#include "ltr_client/factory.h"
#include "ltr_client/utility/parameterized_info.h"


using std::string;

/**
 * Manages object creation and checks for circular dependencies
 */
class DependencyResolvingFactory {
public:
  /**
   * Constructor for DependencyResolvingFactory
   */
  DependencyResolvingFactory(const ConfigParser& configurator):
      configurator_(configurator) {}
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
   * @param object_name - string with the object name
   */
  template <typename RequestedType>
  RequestedType CreateObject(const string& object_name) const;

protected:
  /**
   * Generates a name for an object
   * @param object_info - all the object's properties
   */
  static string getObjectAlias(const ObjectInfo& object_info);

private:
  bool tryBuildingObjectCreationChain(const ObjectInfo* object_info,
    ObjectInfosList* info_queue,
    ObjectInfosList* circularity_check_queue) const;

  const ConfigParser& configurator_;
};


template <typename RequestedType>
RequestedType DependencyResolvingFactory::CreateObject(
          const string& object_name) const {
  const ObjectInfo& object_info =
      configurator_.findObject(object_name);
  const ParametersContainer& parameters =
      Create(object_info.get_parameters());
  Any object = Factory::instance()->Create(getObjectAlias(object_info),
                                           parameters);
  return any_cast<RequestedType>(object);
}

#endif  // LTR_CLIENT_DEPENDENCY_RESOLVING_FACTORY_H_
