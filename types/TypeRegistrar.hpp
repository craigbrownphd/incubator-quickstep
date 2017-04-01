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

#ifndef QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_
#define QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_

#include <type_traits>

#include "types/TypeID.hpp"
#include "types/TypeIDSelectors.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

template <TypeID type_id>
struct TypeClass;

#define REGISTER_TYPE(T, TID) \
  class T;\
  template <> struct TypeClass<TID> { typedef T type; };

REGISTER_TYPE(IntType, kInt);
REGISTER_TYPE(LongType, kLong);
REGISTER_TYPE(FloatType, kFloat);
REGISTER_TYPE(DoubleType, kDouble);
REGISTER_TYPE(DateType, kDate);
REGISTER_TYPE(DatetimeType, kDatetime);
REGISTER_TYPE(DatetimeIntervalType, kDatetimeInterval);
REGISTER_TYPE(YearMonthIntervalType, kYearMonthInterval);
REGISTER_TYPE(CharType, kChar);
REGISTER_TYPE(VarCharType, kVarChar);
REGISTER_TYPE(NullType, kNullType);

#undef REGISTER_TYPE


template <typename Selector = TypeIDSelectorAll, typename FunctorT>
auto InvokeOnTypeID(const TypeID type_id, const FunctorT &functor);


namespace internal {

template <int l, int r, typename Selector, typename FunctorT>
inline auto InvokeOnTypeIDInner(const int value,
                                const FunctorT &functor) {
  DCHECK_LE(l, r);
  if (l == r) {
    constexpr TypeID type_id = static_cast<TypeID>(r);
    return Selector::template Implementation<
        std::integral_constant<TypeID, type_id>, FunctorT>::Invoke(functor);
  }
  constexpr int m = (l + r) >> 1;
  if (value <= m) {
    return InvokeOnTypeIDInner<l, m, Selector, FunctorT>(value, functor);
  } else {
    return InvokeOnTypeIDInner<m+1, r, Selector, FunctorT>(value, functor);
  }
}

}  // namespace internal

template <typename Selector, typename FunctorT>
auto InvokeOnTypeID(const TypeID type_id,
                    const FunctorT &functor) {
  return internal::InvokeOnTypeIDInner<0, static_cast<int>(kNumTypeIDs)-1,
                                       Selector, FunctorT>(
      static_cast<int>(type_id), functor);
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_REGISTRAR_HPP_
