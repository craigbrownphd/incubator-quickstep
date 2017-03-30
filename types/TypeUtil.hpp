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

#ifndef QUICKSTEP_TYPES_TYPE_UTIL_HPP_
#define QUICKSTEP_TYPES_TYPE_UTIL_HPP_

#include <type_traits>

#include "types/CharType.hpp"
#include "types/DateType.hpp"
#include "types/DatetimeIntervalType.hpp"
#include "types/DatetimeType.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/NullType.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "types/YearMonthIntervalType.hpp"
#include "utility/Macros.hpp"
#include "utility/TemplateUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

template <TypeID type_id>
struct TypeClass {};

#define REGISTER_TYPE(T) \
  template <> struct TypeClass<T::kStaticTypeID> { typedef T type; };

REGISTER_TYPE(IntType);
REGISTER_TYPE(LongType);
REGISTER_TYPE(FloatType);
REGISTER_TYPE(DoubleType);
REGISTER_TYPE(DateType);
REGISTER_TYPE(DatetimeType);
REGISTER_TYPE(DatetimeIntervalType);
REGISTER_TYPE(YearMonthIntervalType);
REGISTER_TYPE(CharType);
REGISTER_TYPE(VarCharType);
REGISTER_TYPE(NullType);

#undef REGISTER_TYPE

namespace type_util_internal {

template <bool require_parameterized>
struct TypeIDSelectorParameterizedHelper {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
    inline static auto Invoke(const FunctorT &functor)
        -> decltype(functor(TypeIDConstant())) {
      LOG(FATAL) << "Unexpected TypeID: "
                 << kTypeNames[static_cast<int>(TypeIDConstant::value)];
    }
#pragma GCC diagnostic pop
  };
};

template <bool require_non_parameterized>
template <typename TypeIDConstant, typename FunctorT>
struct TypeIDSelectorParameterizedHelper<require_non_parameterized>::Implementation<
    TypeIDConstant, FunctorT,
    std::enable_if_t<TypeClass<TypeIDConstant::value>::type::kParameterized
                         ^ require_non_parameterized>> {
  inline static auto Invoke(const FunctorT &functor) {
    return functor(TypeIDConstant());
  }
};

}  // namespace type_util_internal

struct TypeIDSelectorAll {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
    inline static auto Invoke(const FunctorT &functor) {
      return functor(TypeIDConstant());
    }
  };
};

using TypeIDSelectorNonParameterized =
    type_util_internal::TypeIDSelectorParameterizedHelper<true>;

using TypeIDSelectorParameterized =
    type_util_internal::TypeIDSelectorParameterizedHelper<false>;

template <TypeID ...candidates>
struct TypeIDSelectorEqualsAny {
  template <typename TypeIDConstant, typename FunctorT, typename EnableT = void>
  struct Implementation {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
    inline static auto Invoke(const FunctorT &functor)
        -> decltype(functor(TypeIDConstant())) {
      LOG(FATAL) << "Unexpected TypeID: "
                 << kTypeNames[static_cast<int>(TypeIDConstant::value)];
    }
#pragma GCC diagnostic pop
  };
};

template <TypeID ...candidates>
template <typename TypeIDConstant, typename FunctorT>
struct TypeIDSelectorEqualsAny<candidates...>::Implementation<
    TypeIDConstant, FunctorT,
    std::enable_if_t<
        EqualsAny<TypeIDConstant,
                  std::integral_constant<TypeID, candidates>...>::value>> {
  inline static auto Invoke(const FunctorT &functor) {
      return functor(TypeIDConstant());
  }
};

template <typename Selector = TypeIDSelectorAll, typename FunctorT>
inline auto InvokeOnTypeID(const TypeID type_id,
                           const FunctorT &functor) {
#define REGISTER_TYPE_ID(type_id) \
  case type_id: \
    return Selector::template Implementation< \
        std::integral_constant<TypeID, type_id>, FunctorT>::Invoke(functor)

  switch (type_id) {
    REGISTER_TYPE_ID(kInt);
    REGISTER_TYPE_ID(kLong);
    REGISTER_TYPE_ID(kFloat);
    REGISTER_TYPE_ID(kDouble);
    REGISTER_TYPE_ID(kDate);
    REGISTER_TYPE_ID(kDatetime);
    REGISTER_TYPE_ID(kDatetimeInterval);
    REGISTER_TYPE_ID(kYearMonthInterval);
    REGISTER_TYPE_ID(kChar);
    REGISTER_TYPE_ID(kVarChar);
    REGISTER_TYPE_ID(kNullType);
    default:
      LOG(FATAL) << "Unrecognized TypeID in InvokeOnTypeID()";
  }

#undef REGISTER_TYPE_ID
}

class TypeUtil {
 public:
  static bool IsParameterized(const TypeID type_id) {
    return InvokeOnTypeID(
        type_id,
        [&](auto tid) -> bool {  // NOLINT(build/c++11)
      return TypeClass<decltype(tid)::value>::type::kParameterized;
    });
  }

 private:
  TypeUtil() {}

  DISALLOW_COPY_AND_ASSIGN(TypeUtil);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_UTIL_HPP_
