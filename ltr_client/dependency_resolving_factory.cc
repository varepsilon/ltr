// Copyright 2012 Yandex

#include "ltr/utility/boost/string_utils.h"
#include "ltr/utility/container_utility.h"

#include "ltr_client/dependency_resolving_factory.h"

using ltr::utility::split;
using ltr::utility::GetValues;

typedef string ObjectDependency;


bool DependencyResolvingFactory::tryBuildingObjectCreationChain(
    const ObjectInfo* object_info,
    ObjectInfosList* info_queue,
    ObjectInfosList* circularity_check_queue) const {
  if (find(info_queue->begin(), info_queue->end(), object_info) !=
      info_queue->end()) {
    return true;
  }
  // check for circular dependencies
  ObjectInfosList::const_iterator begin_iterator = find(
      circularity_check_queue->begin(),
      circularity_check_queue->end(),
      object_info);

  if (begin_iterator != circularity_check_queue->end()) {
    throw logic_error("circular dependency detected when processing " +
        object_info->get_name());
  }
  circularity_check_queue->push_back(object_info);

  // add dependencies
  for (ObjectInfosList::const_iterator iterator =
        object_info->dependency_infos().begin();
        iterator != object_info->dependency_infos().end();
        ++iterator) {
    const ObjectInfo* dep_info = *iterator;
    if (!tryBuildingObjectCreationChain(dep_info,
                                        info_queue,
                                        circularity_check_queue)) {
      throw logic_error("can not resolve dependencies");
    }
  }
  // add self
  info_queue->push_back(object_info);
  return true;
}

void DependencyResolvingFactory::checkCircularDependencyAbsence() const {
  const ObjectInfosList& all_infos =
    GetValues(configurator_.objectInfos());

  ObjectInfosList info_queue;
  for (ObjectInfosList::const_iterator iterator = all_infos.begin();
       iterator != all_infos.end();
       ++iterator) {
    const ObjectInfo* object_info = *iterator;
    ObjectInfosList circularity_check_queue;
    tryBuildingObjectCreationChain(object_info,
                                   &info_queue,
                                   &circularity_check_queue);
  }
}

Any DependencyResolvingFactory::Create(const string& name) const {
  const ConfigParser::ObjectInfos& all_infos =
      configurator_.objectInfos();
  ConfigParser::ObjectInfos::const_iterator iter = all_infos.find(name);
  assert(iter != all_infos.end());
  const ObjectInfo* object_info = &iter->second;
  const ParametersContainer& parameters =
    Create(object_info->get_parameters());
  try {
    return Factory::instance()->Create(object_info->get_type(), parameters);
  } catch (const logic_error& err) {
    rDebug("Exception '%s' was successfully processed", err.what());
    return Factory::instance()->Create(object_info->get_type() +
                                       object_info->get_approach(), parameters);
  }
}

ParametersContainer DependencyResolvingFactory::Create(
      const ParametersContainer& parameters) const {
  ParametersContainer result;
  for (ParametersContainer::StringAnyHash::const_iterator iterator =
       parameters.begin();
       iterator != parameters.end();
       ++iterator) {
    Any& parameter = const_cast<Any&>(iterator->second);
    const string& name = iterator->first;

    if (const ObjectDependency* dependency =
        any_cast<ObjectDependency>(&parameter)) {
      typedef vector<string> TStringVector;
      TStringVector strings = split(split(split(*dependency, "\t"), " "), ",");

      assert(strings.size());
      if (strings.size() == 1 &&
          dependency->find_first_of(',') == string::npos) {
        result.AddNew(name, Create(strings[0]));
      } else {
        vector<Any> list;
        for (TStringVector::const_iterator str_iterator = strings.begin();
             str_iterator != strings.end();
             ++str_iterator) {
          if (!str_iterator->empty()) {
              // TODO(username): remove if after getting rid of boost::split
            list.push_back(Create(*str_iterator));
          }
        }
        result.AddNew(name, list);
      }
    } else if (const ParametersContainer::Ptr container =
               any_cast<ParametersContainer>(&parameter)) {
      result.AddNew(name, Create(*container));
    } else {
      result.AddNew(name, parameter);
    }
  }
  return result;
}

string DependencyResolvingFactory::getObjectAlias(
          const ObjectInfo& object_info) {
  return object_info.get_type() + object_info.get_approach();
}
