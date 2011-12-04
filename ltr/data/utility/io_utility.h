// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_IO_UTILITY_H_
#define LTR_DATA_UTILITY_IO_UTILITY_H_

#include <string>
#include <vector>
#include <map>

#include "ltr/data/data_set.h"
#include "ltr/data/utility/parsers/object_parser.h"

#include "ltr/scorers/scorer.h"

#include "utility/numerical.h"

using std::string;
using std::map;
using std::vector;

namespace ltr {
  namespace io_utility {
    template<class TElement>
    DataSet<TElement> loadDataSet(string filename,
                                  string format);
    template<class TElement>
    void saveDataSet(const DataSet<TElement> data,
                     string filename,
                     string format);

    template<class TElement>
    DataSet<TElement> buildDataSet(const map<size_t, vector<Object> >& object);

    template<class TElement>
    void savePredictions(const DataSet<TElement>& data,
                         Scorer::Ptr scorer,
                         string filename);
  };
};


// Template realization
namespace ltr {
  namespace io_utility {
    template<class TElement>
    DataSet<TElement> loadDataSet(string filename,
                                  string format) {
      std::ifstream file(filename.c_str());
      if (file.fail())
        throw std::logic_error("File " + filename + " not found");
      IParser::Ptr parser = getParser(format);
      parser->init(&file);
      map<size_t, vector<Object> > objects;
      std::string line;
      while (std::getline(file, line)) {
        Object obj;
        obj = parser->parseToObject(line);
        objects[obj.queryId()].push_back(obj);
      }
      file.close();
      return buildDataSet<TElement>(objects);
    }

    template<>
    DataSet<Object> buildDataSet(const map<size_t, vector<Object> >& objects) {
      DataSet<Object> data;
      typedef map<size_t, vector<Object> >::const_iterator object_iterator;

      for (object_iterator it = objects.begin(); it != objects.end(); it++)
        for (size_t i = 0; i < it->second.size(); i++)
          data << it->second.at(i);
      return data;
    }

    template<>
    DataSet<ObjectPair> buildDataSet(
        const map<size_t, vector<Object> >& objects) {
      DataSet<ObjectPair> data;
      typedef map<size_t, vector<Object> >::const_iterator object_iterator;

      for (object_iterator it = objects.begin(); it != objects.end(); it++)
        for (size_t i = 0; i < it->second.size(); i++)
          for (size_t j = 0; j < i; j++)
            data << ObjectPair(it->second.at(i), it->second.at(j));
      return data;
    }

    template<>
    DataSet<ObjectList> buildDataSet(
        const map<size_t, vector<Object> >& objects) {
      DataSet<ObjectList> data;
      typedef map<size_t, vector<Object> >::const_iterator object_iterator;

      ObjectList olist;
        for (object_iterator it = objects.begin(); it != objects.end(); it++)
          for (size_t i = 0; i < it->second.size(); i++)
            olist << it->second.at(i);
      data << olist;
      return data;
    }

    template<class TElement>
    void saveDataSet(const DataSet<TElement>& data,
                     string filename,
                     string format) {
      std::ofstream file(filename.c_str());
      if (file.fail())
        throw std::logic_error("can't open " + filename + " for writing");
      IParser::Ptr parser = getParser(format);
      parser->writeHeader(data);
      for (size_t i = 0; i < data.size(); i++)
        for (size_t j = 0; j < data[i].size(); j++) {
          parser->writeString(data[i][j], &file);
          file << std::endl;
        }
      file.close();
    }

    template<class TElement>
    void savePredictions(const DataSet<TElement>& data,
                         Scorer::Ptr scorer,
                         string filename) {
      std::ofsream file(filename.c_str());
      if (file.fail())
        throw std::logic_error("can't open " + filename + " for writing");
      file.precision(utility::DOUBLE_PRECISION);
      for (size_t i = 0; i < data.size(); i++)
        for (size_t j = 0; j < data[i].size(); j++)6
          file << (*scorer)(data[i][j]) << std::endl;
      file.close();
    }
  };
};
#endif  // LTR_DATA_UTILITY_IO_UTILITY_H_
