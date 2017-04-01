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

#ifndef QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_
#define QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_

#include <cstddef>
#include <tuple>
#include <utility>

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Represents a compile-time sequence of integers.
 *
 * Sequence is defined here for C++11 compatibility. For C++14 and above,
 * std::integer_sequence can be used to achieve the same functionality.
 *
 * TODO(jianqiao): directly use std::integer_sequence if having C++14 support.
 */
template<std::size_t ...>
struct Sequence {};

/**
 * @brief The helper class for creating Sequence. MakeSequence<N>::type is
 *        equivalent to Sequence<1,2,...,N>.
 *
 * MakeSequence is defined here for C++11 compatibility. For C++14 and above,
 * std::make_index_sequence can be used to achieve the same functionality.
 *
 * TODO(jianqiao): directly use std::make_index_sequence if having C++14 support.
 */
template<std::size_t N, std::size_t ...S>
struct MakeSequence : MakeSequence<N-1, N-1, S...> {};

template<std::size_t ...S>
struct MakeSequence<0, S...> {
  typedef Sequence<S...> type;
};


template <typename T, typename EnableT = void>
struct IsTypeTrait {
  static constexpr bool value = false;
};

template <typename T>
struct IsTypeTrait<T, std::enable_if_t<
    std::is_same<typename T::type, typename T::type>::value>> {
  static constexpr bool value = true;
};


template<class...> struct Disjunction : std::false_type {};
template<class B1> struct Disjunction<B1> : B1 {};
template<class B1, class... Bn>
struct Disjunction<B1, Bn...>
    : std::conditional_t<bool(B1::value), B1, Disjunction<Bn...>>  {};

template <typename check, typename ...cases>
struct EqualsAny {
  static constexpr bool value =
      Disjunction<std::is_same<check, cases>...>::value;
};

template <char ...c>
struct StringLiteral {
  inline static std::string ToString() {
    return std::string({c...});
  }
};

template <typename LeftT, typename RightT>
struct PairSelectorLeft {
  typedef LeftT type;
};

template <typename LeftT, typename RightT>
struct PairSelectorRight {
  typedef RightT type;
};

/**
 * @brief Invoke the functor with the compile-time bool values wrapped as
 *        integral_constant types.
 */
template <typename FunctorT, bool ...bool_values>
inline auto InvokeOnBoolsInner(const FunctorT &functor) {
  return functor(std::integral_constant<bool, bool_values>()...);
}

/**
 * @brief Recursive dispatching.
 */
template <typename FunctorT, bool ...bool_values, typename ...Bools>
inline auto InvokeOnBoolsInner(const FunctorT &functor,
                               const bool tparam,
                               const Bools ...rest_params) {
  if (tparam) {
    return InvokeOnBoolsInner<FunctorT, bool_values..., true>(
        functor, rest_params...);
  } else {
    return InvokeOnBoolsInner<FunctorT, bool_values..., false>(
        functor, rest_params...);
  }
}

/**
 * @brief Move the functor to the first position in argument list.
 */
template <std::size_t last, std::size_t ...i, typename TupleT>
inline auto InvokeOnBoolsInner(TupleT &&args, Sequence<i...> &&indices) {
  return InvokeOnBoolsInner(std::get<last>(std::forward<TupleT>(args)),
                            std::get<i>(std::forward<TupleT>(args))...);
}

template <typename Out, typename Rest,
          template <typename> class Op, typename EnableT = void>
struct MapInner;

template <typename Out, typename Rest,
          template <typename> class Op, typename EnableT = void>
struct FlatMapInner;

template <typename Out, typename Rest,
          template <typename> class Op, typename EnableT = void>
struct FilterInner;

template <typename Out, typename Rest,
          template <typename> class Op, typename EnableT = void>
struct FilterMapInner;

template <typename Out, typename Rest, typename EnableT = void>
struct UniqueInner;

template <typename Out, typename Rest, typename ...DumbT>
struct UniqueInnerHelper {
  using type = typename UniqueInner<Out, Rest>::type;
  static constexpr bool kDumbSize = sizeof...(DumbT);
};

template <typename Out, typename Rest,
          typename Subtrahend, typename EnableT = void>
struct SubtractInner;

template <typename ...Ts>
class TypeList;

template <typename ...Ts>
class TypeListCommon {
 private:
  template <typename ...Tail>
  struct AppendInner {
    using type = TypeList<Ts..., Tail...>;
  };

 public:
  static constexpr std::size_t length = sizeof...(Ts);

  template <template <typename ...> class Target>
  using bind = Target<Ts...>;

  template <typename T>
  using push_front = TypeList<T, Ts...>;

  template <typename T>
  using push_back = TypeList<Ts..., T>;

  template <typename T>
  using contains = EqualsAny<T, Ts...>;

  template <typename TL>
  using append = typename TL::template bind<AppendInner>::type;

  template <template <typename> class Op>
  using map = typename MapInner<TypeList<>, TypeList<Ts...>, Op>::type;

  template <template <typename> class Op>
  using flatmap = typename FlatMapInner<TypeList<>, TypeList<Ts...>, Op>::type;

  template <template <typename> class Op>
  using filter = typename FilterInner<TypeList<>, TypeList<Ts...>, Op>::type;

  template <template <typename> class Op>
  using filtermap = typename FilterMapInner<TypeList<>, TypeList<Ts...>, Op>::type;

  template <typename ...DumbT>
  using unique = typename UniqueInnerHelper<TypeList<>, TypeList<Ts...>, DumbT...>::type;

  template <typename Subtrahend>
  using subtract = typename SubtractInner<TypeList<>, TypeList<Ts...>, Subtrahend>::type;
};

/**
 * @brief A helper function for bool branched template specialization.
 *
 * Usage example:
 * --
 * bool c1 = true, c2 = false;
 *
 * InvokeOnBools(
 *     c1, c2,
 *     [&](auto c1, auto c2) -> SomeBaseClass* {
 *   using T1 = decltype(c1);  // T1 == std::true_type
 *   using T2 = decltype(c2);  // T2 == std::false_type
 *
 *   constexpr bool cv1 = T1::value;  // cv1 == true
 *   constexpr bool cv2 = T2::value;  // cv2 == false
 *
 *   SomeFunction<cv1, cv2>(...);
 *   return new SomeClass<cv1, cv2>(...);
 * });
 * --
 */
template <typename ...ArgTypes>
inline auto InvokeOnBools(ArgTypes ...args) {
  constexpr std::size_t last = sizeof...(args) - 1;
  return InvokeOnBoolsInner<last>(
      std::forward_as_tuple(args...),
      typename MakeSequence<last>::type());
}

template <typename ...Ts>
class TypeList : public TypeListCommon<Ts...> {
 private:
  template <typename Head, typename ...Tail>
  struct HeadTailInner {
    using head = Head;
    using tail = TypeList<Tail...>;
  };

 public:
  using head = typename HeadTailInner<Ts...>::head;
  using tail = typename HeadTailInner<Ts...>::tail;
};

template <>
class TypeList<> : public TypeListCommon<> {
};


template <typename Out, typename Rest, template <typename> class Op>
struct MapInner<Out, Rest, Op,
                std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename> class Op>
struct MapInner<Out, Rest, Op,
                std::enable_if_t<Rest::length != 0>>
    : MapInner<typename Out::template push_back<typename Op<typename Rest::head>::type>,
               typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename> class Op>
struct FlatMapInner<Out, Rest, Op,
                    std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename> class Op>
struct FlatMapInner<Out, Rest, Op,
                    std::enable_if_t<Rest::length != 0>>
    : FlatMapInner<typename Out::template append<typename Op<typename Rest::head>::type>,
                   typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename> class Op>
struct FilterInner<Out, Rest, Op,
                   std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename> class Op>
struct FilterInner<Out, Rest, Op,
                   std::enable_if_t<Op<typename Rest::head>::value>>
    : FilterInner<typename Out::template push_back<typename Rest::head>,
                  typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename> class Op>
struct FilterInner<Out, Rest, Op,
                   std::enable_if_t<!Op<typename Rest::head>::value>>
    : FilterInner<Out, typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename> class Op>
struct FilterMapInner<Out, Rest, Op,
                      std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, template <typename> class Op>
struct FilterMapInner<Out, Rest, Op,
                      std::enable_if_t<Rest::length != 0 &&
                                       IsTypeTrait<Op<typename Rest::head>>::value>>
    : FilterMapInner<typename Out::template push_back<typename Op<typename Rest::head>::type>,
                     typename Rest::tail, Op> {};

template <typename Out, typename Rest, template <typename> class Op>
struct FilterMapInner<Out, Rest, Op,
                      std::enable_if_t<Rest::length != 0 &&
                                       !IsTypeTrait<Op<typename Rest::head>>::value>>
    : FilterMapInner<Out, typename Rest::tail, Op> {};

template <typename Out, typename Rest>
struct UniqueInner<Out, Rest,
                   std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest>
struct UniqueInner<Out, Rest,
                   std::enable_if_t<Out::template contains<typename Rest::head>::value>>
    : UniqueInner<Out, typename Rest::tail> {};

template <typename Out, typename Rest>
struct UniqueInner<Out, Rest,
                   std::enable_if_t<!Out::template contains<typename Rest::head>::value>>
    : UniqueInner<typename Out::template push_back<typename Rest::head>,
                  typename Rest::tail> {};

template <typename Out, typename Rest, typename Subtrahend>
struct SubtractInner<Out, Rest, Subtrahend,
                     std::enable_if_t<Rest::length == 0>> {
  using type = Out;
};

template <typename Out, typename Rest, typename Subtrahend>
struct SubtractInner<Out, Rest, Subtrahend,
                     std::enable_if_t<Subtrahend::template contains<
                         typename Rest::head>::value>>
    : SubtractInner<Out, typename Rest::tail, Subtrahend> {};

template <typename Out, typename Rest, typename Subtrahend>
struct SubtractInner<Out, Rest, Subtrahend,
                     std::enable_if_t<!Subtrahend::template contains<
                         typename Rest::head>::value>>
    : SubtractInner<typename Out::template push_back<typename Rest::head>,
                    typename Rest::tail, Subtrahend> {};

template <typename LeftEdge, typename RightEdge, typename EnableT = void>
struct EdgeMatcher {};

template <typename LeftEdge, typename RightEdge>
struct EdgeMatcher<LeftEdge, RightEdge,
                   std::enable_if_t<std::is_same<typename LeftEdge::tail::head,
                                                 typename RightEdge::head>::value>> {
  using type = TypeList<typename LeftEdge::head, typename RightEdge::tail::head>;
};

template <typename LeftEdges, typename RightEdges>
struct JoinPath {
  template <typename LeftEdge>
  struct JoinPathLeftHelper {
    template <typename RightEdge>
    struct JoinPathRightHelper : EdgeMatcher<LeftEdge, RightEdge> {};

    using type = typename RightEdges::template filtermap<JoinPathRightHelper>;
  };
  using type = typename LeftEdges::template flatmap<JoinPathLeftHelper>;
};

// Semi-naive
template <typename Out, typename WorkSet, typename Edges, typename EnableT = void>
struct TransitiveClosureInner;

template <typename Out, typename WorkSet, typename Edges>
struct TransitiveClosureInner<Out, WorkSet, Edges,
                              std::enable_if_t<WorkSet::length == 0>> {
  using type = Out;
};

template <typename Out, typename WorkSet, typename Edges>
struct TransitiveClosureInner<Out, WorkSet, Edges,
                              std::enable_if_t<WorkSet::length != 0>>
    : TransitiveClosureInner<typename Out::template append<WorkSet>,
                             typename JoinPath<WorkSet, Edges>::type::template subtract<
                                 typename Out::template append<WorkSet>>::template unique<>,
                             Edges> {};

template <typename Edge>
struct TransitiveClosureInitializer {
  using type = TypeList<TypeList<typename Edge::head, typename Edge::head>,
                        TypeList<typename Edge::tail::head, typename Edge::tail::head>>;
};

template <typename Edges>
using TransitiveClosure =
    typename TransitiveClosureInner<
        TypeList<>,
        typename Edges::template flatmap<TransitiveClosureInitializer>::template unique<>,
        Edges>::type;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_TEMPLATE_UTIL_HPP_
