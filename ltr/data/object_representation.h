// Copyright Yandex 2013

#ifndef LTR_DATA_OBJECT_REPRESENTATION_H_
#define LTR_DATA_OBJECT_REPRESENTATION_H_

#include "contrib/include_Eigen.h"
#include "ltr/utility/boost/shared_ptr.h"
#include "ltr/data/inner_representation.h"

namespace ltr {

using ltr::utility::shared_ptr;

/**
 * This is an interface for storing object data in different ways.
 */
class ObjectRepresentation {
 public:
  /**
   * Shared pointer to this object.
   */
  typedef shared_ptr<ObjectRepresentation> Ptr;
  /**
   * Destructor (has to be virtual).
   */
  virtual ~ObjectRepresentation() { }
  /**
   * Return link to object's meta info.
   */
  virtual MetaInfoPtr meta_info() = 0;
  /**
   * Return constant link to object's meta info.
   */
  virtual const MetaInfoPtr meta_info() const = 0;
  /**
   * Set presentation's meta info.
   */
  virtual void set_meta_info(const MetaInfo& meta_info) = 0;
  /**
   * Add a feature to presentation.
   */
  virtual void push_back(double feature) = 0;
  /**
   * Return reference to feature with given index.
   */
  virtual double& at(int index) = 0;
  /**
   * Return constant reference to feature with given index.
   */
  virtual const double& at(int index) const = 0;
  /**
   * Returns features presented in eigen column form. This operation is lazy.
   */
  virtual EigenColumn getEigenColumn() = 0;
  /**
   * Return constant eigen column of features.
   */
  virtual const EigenColumn getEigenColumn() const = 0;
  /**
   * Set features from eigen vector.
   */
  virtual void setEigenColumn(const VectorXd& column) = 0;
  /**
   * Copy features, labels and info from another presentation.
   */
  virtual void deepCopyFrom(const ObjectRepresentation& other) = 0;
  /**
   * Delete all features.
   */
  virtual void clear() = 0;
  /**
   * Return number of features.
   */
  virtual int size() const = 0;
  /**
   * Set number of features.
   */
  virtual void resize(int new_size) = 0;
  /**
   * Return actual scoring label of an object.
   */
  virtual double actual_label() const = 0;
  /**
   * Set actual scoring label of an object.
   */
  virtual void set_actual_label(double label) = 0;
  /**
   * Return predicted scoring label of an object.
   */
  virtual double predicted_label() const = 0;
  /**
   * Set predicted scoring label of an object.
   */
  virtual void set_predicted_label(double label) = 0;
  /**
   * Return whether this type of presentation supports light copying,
   * i.e. whether we can do smth like
   * ObjectRepresentation* presentation1;
   * ObjectRepresentation* presentation2;
   * presentation1 = presentation2
   * with no damage to presentation1.
   */
  virtual bool allowLightCopy() const = 0;
};
}
#endif  // LTR_DATA_OBJECT_REPRESENTATION_H_
