// Copyright 2013 Yandex

#ifndef LTR_UTILITY_BOOST_LEXICAL_CAST_CONFIG_HPP_
#define LTR_UTILITY_BOOST_LEXICAL_CAST_CONFIG_HPP_

#include <sstream>
#include <string>

using std::stringstream;
using std::string;

namespace ltr {
namespace utility {
/**
  * \brief A singleton class used to configure lexical_cast
  * output. Mostly used for floating point numbers.
*/
class LexicalCastConfig {
  public:
    /**
     * \brief A method to get a singleton instance.
     */
    static LexicalCastConfig& getInstance();

    /**
     * \brief Stop using any additional settings
     * for output.
    */
    void restoreDefaults();
    /**
     * \brief Set precision for floating point numbers.
     * \param precision new precision value.
    */
    void setPrecision(int precision);
    /**
     * \brief Toggle fixed output for floating point numbers.
     * \param fixed true if fixed output should be used.
    */
    void setFixed(bool fixed);
    /**
     * \brief Service function used to configure
     * given stringstream with set values.
     * \param stream stringstream to configure.
    */
    void configure(stringstream* stream) const;

  private:
    LexicalCastConfig();

    static LexicalCastConfig* instance_;

    int precision_;
    int fixed_;
};
}
}

#endif  // LTR_UTILITY_BOOST_LEXICAL_CAST_CONFIG_HPP_
