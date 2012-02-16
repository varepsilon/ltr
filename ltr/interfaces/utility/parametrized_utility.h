// Copyright 2011 Yandex

#ifndef LTR_INTERFACES_UTILITY_PARAMETRIZED_UTILITY_H_
#define LTR_INTERFACES_UTILITY_PARAMETRIZED_UTILITY_H_

#include <string>

#include "ltr/interfaces/parameterized.h"

using std::string;
using std::logic_error;

namespace ltr {
/** \fn The function checks if the int parameter in the given Parameterized is
 * greater than 0, otherwise it throws a logical_error.
 * \param parametrized reference to an Parameterized object.
 * \param parameterName the name of a parameter that should be of int type.
 */
void checkIntParameterGreaterZero(const Parameterized& parametrized,
    const string& parameterName);
/** \fn The function checks if the int parameter in the given Parameterized is
 * greater than the given value, otherwise it throws a logical_error.
 * \param parametrized reference to an Parameterized object.
 * \param parameterName the name of a parameter that should be of int type.
 * \param value the value for evaluation.
 */
void checkIntParameterGreaterThan(const Parameterized& parametrized,
    const string& parameterName,
    int value);

/** \fn The function checks if the double parameter in the given Parameterized
 * is in [0, 1], otherwise it throws a logical_error.
 * \param parametrized reference to an Parameterized object.
 * \param parameterName the name of a parameter that should be of double type.
 */
void checkDoubleParameterGreaterOrEqualZeroLessOrEqualOne(
    const Parameterized& parametrized,
    const string& parameterName);

/** \fn The function checks if the double parameter in the given Parameterized
 * is in [segmentBegins, segmentEnds], otherwise it throws a logical_error.
 * \param parametrized reference to an Parameterized object.
 * \param parameterName the name of a parameter that should be of double type.
 * \param segmentBegins the begin of the segment.
 * \param segmentEnds the end of the segment.
 */
void checkDoubleParameterGreaterInsegment(const Parameterized& parametrized,
    const string& parameterName,
    double segmentBegins,
    double segmentEnds);
}
#endif  // LTR_INTERFACES_UTILITY_PARAMETRIZED_UTILITY_H_
