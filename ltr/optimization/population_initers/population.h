//  Copyright 2013 Yandex

#ifndef LTR_OPTIMIZATION_POPULATION_INITERS_POPULATION_H_
#define LTR_OPTIMIZATION_POPULATION_INITERS_POPULATION_H_

#include <string>
#include <map>
#include "Eigen/Core"
#include "ltr/utility/macros.h"
#include "ltr/interfaces/aliaser.h"
#include "ltr/utility/boost/shared_ptr.h"
using std::map;
using std::pair;

namespace optimization {
typedef int PointId;
typedef Eigen::VectorXd Point;
/**
 * \brief Class for population of points.
 *
 * Every point in population has Id. <br>
 * Numerations of points in population starts from 0. <br>
 * Next point ID = current point ID + 1.
 */
class Population : public ltr::Aliaser {
 public:
  typedef ltr::utility::shared_ptr<Population> Ptr;

  explicit Population() : id_counter_(0) { }
  ~Population() { }

  friend class Iterator;
  /**
   * Iterator for points in population.
   */
  class Iterator {
   public:
    Iterator(map<PointId, Point>::const_iterator iterator);
    ~Iterator() { }
    /**
     * Ordinary operathors and methods for iterators.
     */

    const Iterator& operator++();
    const Iterator operator++(int n);

    const Iterator& operator--();
    const Iterator operator--(int n);

    bool operator==(const Iterator& iterator);
    bool operator!=(const Iterator& iterator);
    Iterator& operator=(const Iterator& iterator) {
      if (this == &iterator) {
        return *this;
      } else {
        this->map_iterator_ = iterator.map_iterator_;
      }
      return *this;
    }
    /**
     * Get point ID.
     */
    PointId point_id() const;
    /**
     * Get point.
     */
    Point point() const;
   private:
    map<PointId, Point>::const_iterator map_iterator_;
  };
  /**
   * Add point to population.
   *
   * \return id of this point.
   */
  PointId addPoint(const Point& point);
  /**
   * Remove point from population by Id.
   */
  void removePoint(PointId point_id);
  /**
   * Get point from population by Id.
   */
  void getPoint(PointId point_id, Point* point) const;
  /**
   * Update point from population by Id.
   */
  void updatePoint(PointId, const Point& updated_point);
  /**
   * Get size of population.
   */
  int size() const;
  /**
   * Iterator to begin of population.
   */
  Iterator begin() const;
  /**
   * Iterator to end of population.
   */
  Iterator end() const;

  virtual string getDefaultAlias() const { return "Population"; }
 private:
  /**
   * Map of pointId, point.
   */
  map<PointId, Point> points_;
  /**
   * Total count of added points.
   */
  int id_counter_;
};
}

#endif  // LTR_OPTIMIZATION_POPULATION_INITERS_POPULATION_H_
