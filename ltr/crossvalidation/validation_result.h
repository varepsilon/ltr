// Copyright 2011 Yandex

#include <string>
#include <vector>

#include "scorers/scorer.h"


namespace ltr {
  namespace cv {

    class ValidationResult {
     public:
      void addSplitInfo(Scorer::Ptr in_scorer, const string& in_report, double in_measure_value);

      size_t getSplitCount() const;
      Scorer::Ptr getScorer(size_t split_index) const;
      const string& getReport(size_t split_index) const;
      double getMeasureValue(size_t split_index) const;
     private:
      struct OneSplitData {
        Scorer::Ptr scorer;
        std::string report;
        double measure_value;

        OneSplitData(Scorer::Ptr in_scorer, const string& in_report,
            double in_measure_value) :
          scorer(in_scorer),
          report(in_report),
          measure_value(in_measure_value) {}
      };

      std::vector<OneSplitData> datas_;
    };

  };
};

namespace ltr {
  namespace cv {
    size_t ValidationResult::getSplitCount() const {
      return datas_.size();
    }

    void ValidationResult::addSplitInfo(Scorer::Ptr in_scorer,
        const string& in_report, double in_measure_value) {
      datas_.push_back(OneSplitData(in_scorer, in_report, in_measure_value));
    }

    Scorer::Ptr ValidationResult::getScorer(size_t split_index) const {
      return datas_[split_index].scorer;
    }

    const string& ValidationResult::getReport(size_t split_index) const {
      return datas_[split_index].report;
    }

    double ValidationResult::getMeasureValue(size_t split_index) const {
      return datas_[split_index].measure_value;
    }
  };
};
