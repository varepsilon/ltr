// Copyright 2013 Yandex

#ifndef LTR_DATA_STAND_ALONE_OBJECT_REPRESENTATION_H_
#define LTR_DATA_STAND_ALONE_OBJECT_REPRESENTATION_H_

#include "ltr/data/object_representation.h"

namespace ltr {
/** \brief class for presenting Eigen::VectorXd as an object.
 */
class StandAloneObjectRepresentation : public ObjectRepresentation {
 public:
  /** Shared pointer to presentation.
   */
  typedef shared_ptr<StandAloneObjectRepresentation> Ptr;
  /** Constructor, creates independent vector with given size
   */
  explicit StandAloneObjectRepresentation(int size = 0);
  virtual ~StandAloneObjectRepresentation();

  virtual MetaInfoPtr meta_info();
  virtual const MetaInfoPtr meta_info() const;
  virtual void set_meta_info(const MetaInfo& meta_info);

  virtual void push_back(double feature);
  virtual double& at(int index);
  virtual const double& at(int index) const;

  virtual EigenColumn getEigenColumn();
  virtual const EigenColumn getEigenColumn() const;
  virtual void setEigenColumn(const VectorXd& col);

  virtual void deepCopyFrom(const ObjectRepresentation& other);
  virtual void clear();

  virtual int size() const;
  virtual void resize(int new_size);

  virtual double actual_label() const;
  virtual void set_actual_label(double label);

  virtual double predicted_label() const;
  virtual void set_predicted_label(double label);

  virtual bool allowLightCopy() const;
 private:
  /** Vector of features. presented in dynamic-size matrix form
   * to conform with interface's getEigenColumn() method
   * mutable to return const Block<MatrixXd, -1, 1> in the const method,
   * not the const Block<const MatrixXd, -1, 1>.
   */
  mutable MatrixXd features_;
  double predicted_label_;
  double actual_label_;
  MetaInfoPtr meta_info_;
};
}

#endif  // LTR_DATA_STAND_ALONE_OBJECT_REPRESENTATION_H_
