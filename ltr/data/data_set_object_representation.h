// Copyright Yandex 2013

#ifndef LTR_DATA_DATA_SET_OBJECT_REPRESENTATION_H_
#define LTR_DATA_DATA_SET_OBJECT_REPRESENTATION_H_

#include "ltr/data/object_representation.h"
#include "ltr/data/inner_representation.h"

namespace ltr {
/** \brief Class for presenting column in the matrix as an object.
 */
class DataSetObjectRepresentation : public ObjectRepresentation {
 public:
  /** Shared pointer to presentation.
   */
  typedef shared_ptr<DataSetObjectRepresentation> Ptr;
  /** Constructor that uses column in the presentation and index
   * (number of column in feature matrix and index in label vectors)
   */
  DataSetObjectRepresentation(InnerRepresentation::Ptr presentation, int index);
  virtual ~DataSetObjectRepresentation();

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
  InnerRepresentation::Ptr presentation_;
  int index_;
};
}

#endif  // LTR_DATA_DATA_SET_OBJECT_REPRESENTATION_H_
