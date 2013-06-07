// Copyright 2013 Yandex

#include "ltr/optimization/population_info/population.h"
#include "ltr/optimization/population_info/population_info.hpp"

#include <iostream>

namespace optimization {
//  realization of Population

PointId Population::addPoint(const Point& point) {
  points_[id_counter_] = point;
  for (int population_info_counter = 0;
       population_info_counter < population_info_.size();
       ++population_info_counter) {
     population_info_[population_info_counter]->addPoint(point, id_counter_);
  }
  return id_counter_++;
}
void Population::removePoint(PointId point_id) {
  for (int population_info_counter = 0;
       population_info_counter < population_info_.size();
       ++population_info_counter) {
     population_info_[population_info_counter]->removePoint(point_id);
  }
  points_.erase(point_id);
}
void Population::getPoint(PointId point_id, Point* point) const {
  *point = points_.find(point_id)->second;
}
void Population::updatePoint(PointId point_id, const Point& updated_point) {
  points_[point_id] = updated_point;
}
int Population::size() const {
  return points_.size();
}
Population::Iterator Population::begin() const {
  return Population::Iterator(points_.begin());
}
Population::Iterator Population::end() const {
  return Population::Iterator(points_.end());
}
void Population::attachInfo(BasePopulationInfo* population_info) {
  population_info_.push_back(population_info);
}

// realization of Population::Iterator

Population::Iterator::Iterator(map<PointId, Point>::const_iterator iterator)
    : map_iterator_(iterator) { }
const Population::Iterator& Population::Iterator::operator++() {
  ++map_iterator_;
  return *this;
}
const Population::Iterator Population::Iterator::operator++(int n) {
  Population::Iterator iterator(*this);
  ++map_iterator_;
  return iterator;
}
const Population::Iterator& Population::Iterator::operator--() {
  --map_iterator_;
  return *this;
}
const Population::Iterator Population::Iterator::operator--(int n) {
  Population::Iterator iterator(*this);
  --map_iterator_;
  return iterator;
}
bool Population::Iterator::operator==(const Iterator& iterator) {
  return this->map_iterator_ == iterator.map_iterator_;
}
bool Population::Iterator::operator!=(const Iterator& iterator) {
  return map_iterator_ != iterator.map_iterator_;
}
PointId Population::Iterator::point_id() const {
  return map_iterator_->first;
}
Point Population::Iterator::point() const {
  return map_iterator_->second;
}
}
