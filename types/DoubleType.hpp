/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_TYPES_DOUBLE_TYPE_HPP_
#define QUICKSTEP_TYPES_DOUBLE_TYPE_HPP_

#include <cstdio>
#include <limits>
#include <string>

#include "types/NumericSuperType.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A type representing a double-precision floating-point number.
 **/
class DoubleType : public NumericSuperType<DoubleType, kDouble, double> {
 public:
  bool isSafelyCoercibleFrom(const Type &original_type) const override;

  int getPrintWidth() const override {
    return kPrintWidth;
  }

  std::string printValueToString(const TypedValue &value) const override;

  void printValueToFile(const TypedValue &value,
                        FILE *file,
                        const int padding = 0) const override;

  bool parseValueFromString(const std::string &value_string,
                            TypedValue *value) const override;

  TypedValue coerceValue(const TypedValue &original_value,
                         const Type &original_type) const override;

 private:
  static_assert((std::numeric_limits<double>::max_exponent10 < 1000)
                    && (std::numeric_limits<double>::min_exponent10 > -1000),
                "Exponent range for double can not be represented with 3 "
                "decimal digits.");

  static constexpr int kPrintWidth
      = std::numeric_limits<double>::max_digits10  // Maximum mantissa digits
        + 1   // Decimal point '.'
        + 1   // Minus sign '-'
        + 2   // Exponent signifier 'e' and sign '+' or '-'
        + 3;  // Maximum exponent digits. Range of normalized base-10 exponents
              // for 64-bit double is -308 to +308. Smallest denormalized
              // exponent is -324. The static_assert above ensures that the
              // exponent never takes more than 3 base-10 digits to represent.

  explicit DoubleType(const bool nullable)
      : NumericSuperType<DoubleType, kDouble, double>(nullable) {
  }

  template <typename, bool> friend class TypeInstance;

  DISALLOW_COPY_AND_ASSIGN(DoubleType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DOUBLE_TYPE_HPP_
