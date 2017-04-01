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

#ifndef QUICKSTEP_TYPES_NUMERIC_TYPE_SAFE_COERCIBILITY_HPP_
#define QUICKSTEP_TYPES_NUMERIC_TYPE_SAFE_COERCIBILITY_HPP_

#include "utility/TemplateUtil.hpp"

namespace quickstep {

class BoolType;
class DoubleType;
class FloatType;
class IntType;
class LongType;

/** \addtogroup Types
 *  @{
 */

using NumericTypeSafeCoersions = TypeList<
    TypeList<BoolType, IntType>,
    TypeList<IntType, FloatType>,
    TypeList<IntType, LongType>,
    TypeList<FloatType, DoubleType>,
    TypeList<LongType, DoubleType>
>;

using NumericTypeSafeCoersionClosure = TransitiveClosure<NumericTypeSafeCoersions>;


template <typename LeftType, typename RightType>
struct NumericTypeSafeCoercibility {
  static constexpr bool value =
      NumericTypeSafeCoersionClosure::contains<
          TypeList<LeftType, RightType>>::value;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_TYPE_SAFE_COERCIBILITY_HPP_
