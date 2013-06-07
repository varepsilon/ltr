// Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_INFO_POPULATION_INFO_HPP_
#define LTR_OPTIMIZATION_POPULATION_INFO_POPULATION_INFO_HPP_

#include <map>
#include "ltr/utility/boost/shared_ptr.h"
#include "contrib/include_Eigen.h"
#include "ltr/optimization/population_info/population.h"
using std::map;

namespace optimization {
typedef int PointId;
typedef Eigen::VectorXd Point;
/**
 * Base class for PopulationInfo.
 *
 * \see PopulationInfo.
 */
class BasePopulationInfo {
 public:
  typedef ltr::utility::shared_ptr<BasePopulationInfo> Ptr;
  explicit BasePopulationInfo(Population* population) {
    population->attachInfo(this);
  }
  virtual ~BasePopulationInfo() { }

  virtual void addPoint(const Point& point, PointId point_id) = 0;
  virtual void removePoint(PointId point_id) = 0;
};
/**
 * \brief Abstract class for store info about population,
 * for example StopCriteria, Updaters, etc.
 */
template<class TInfo>
class PopulationInfo : public BasePopulationInfo {
 public:
  typedef typename ltr::utility::shared_ptr<PopulationInfo<TInfo> > Ptr;
  typedef typename map<PointId, typename TInfo::Ptr>::iterator Iterator;
  explicit PopulationInfo(Population* population,
                          typename TInfo::Ptr sample_info);
  Iterator begin();
  Iterator end();
  ~PopulationInfo() { }
  /**
   * Add PopulationInfo for point from population.
   */
  void addPoint(const Point& point, PointId point_id);
  /**
   * Remove PopulationInfo for point from population.
   */
  void removePoint(PointId point_id);
  /**
   * Get info for point with point_id.
   */
  typename TInfo::Ptr getInfoById(PointId point_id);
 private:
  map<PointId, typename TInfo::Ptr> infomap_;
  typename TInfo::Ptr sample_info_;
};

// template realization
template<class TInfo>
PopulationInfo<TInfo>::PopulationInfo(Population* population,
                                      typename TInfo::Ptr sample_info)
    : BasePopulationInfo(population),
      sample_info_(sample_info) {
  for (Population::Iterator it = population->begin();
       it != population->end();
       ++it) {
    typename TInfo::Ptr info = sample_info_->clone();
    info->init(it.point());
    infomap_[it.point_id()] = info;
  }
}

template<class TInfo>
typename PopulationInfo<TInfo>::Iterator PopulationInfo<TInfo>::begin() {
  return infomap_.begin();
}

template<class TInfo>
typename PopulationInfo<TInfo>::Iterator PopulationInfo<TInfo>::end() {
  return infomap_.end();
}

template<class TInfo>
void PopulationInfo<TInfo>::addPoint(const Point& point, PointId point_id) {
  typename TInfo::Ptr info = sample_info_->clone();
  info->init(point);
  infomap_[point_id] = info;
}

template<class TInfo>
void PopulationInfo<TInfo>::removePoint(PointId point_id) {
  infomap_.erase(point_id);
}

template<class TInfo>
typename TInfo::Ptr PopulationInfo<TInfo>::getInfoById(PointId point_id) {
  return infomap_[point_id];
}
}

#endif  // LTR_OPTIMIZATION_POPULATION_INFO_POPULATION_INFO_HPP_
