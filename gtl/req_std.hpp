//
// Created by zbook15 on 9/6/2020.
//

#ifndef GTL_HEADGUARD_require_std_hpp
#define GTL_HEADGUARD_require_std_hpp

#include <concepts>
#include <ranges>
#include <utility>

#include "def_shortcuts.hpp"

namespace gtl::req {

    /* Core Language Concepts */
    __CASE<typename T, typename U>
    concept SameAs_ = std::same_as<T, U>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept DerivedFrom_ = std::derived_from<T, U>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept ConvertibleTo_ = std::convertible_to<T, U>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept CommonRefWith_ = std::common_reference_with<T, U>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept CommonWith_ = std::common_with<T, U>; /// Specifies that a type is the same as another type.

    __CASET
    concept Integral_ = std::integral<T>; /// Specifies that a type is the same as another type.

    __CASET
    concept SignedIntegral_ = std::signed_integral<T>; /// Specifies that a type is the same as another type.

    __CASET
    concept UnsignedIntegral_ = std::unsigned_integral<T>; /// Specifies that a type is the same as another type.

    __CASET
    concept FloatingPoint_ = std::floating_point<T>; /// Specifies that a type is the same as another type.

    __CASE<typename LhsT, typename RhsT>
    concept AssignableFrom_ = std::assignable_from<LhsT, RhsT>; /// Specifies that a type is the same as another type.

    __CASET
    concept Swappable_ = std::swappable<T>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept SwappableWith_ = std::swappable_with<T, U>; /// Specifies that a type is the same as another type.

    __CASET
    concept Destructible_ = std::destructible<T>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept ConstructibleFrom_ = std::constructible_from<T, U>; /// Specifies that a type is the same as another type.

    __CASET
    concept DefaultInitializable_ = std::default_initializable<T>; /// Specifies that a type is the same as another type.

    __CASET
    concept MoveConstructible_ = std::move_constructible<T>; /// Specifies that a type is the same as another type.

    // note: Types without a move constructor, but with a copy constructor that accepts const T& arguments,
    //       satisfy std::is_move_constructible.
    __CASET
    concept CopyAndMoveConstructible_ = std::move_constructible<T>; /// Specifies that a type is the same as another type.

    /* Comparison concepts */
    __CASET
    concept EqualityComparable_ = std::equality_comparable<T>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept EqualityComparableWith_ = std::equality_comparable_with<T, U>; /// Specifies that a type is the same as another type.

    __CASET
    concept TotallyOrdered_ = std::totally_ordered<T>; /// Specifies that a type is the same as another type.

    __CASE<typename T, typename U>
    concept TotallyOrderedWith_ = std::totally_ordered_with<T, U>; /// Specifies that a type is the same as another type.

    __CASET
    concept MoveAndSwappable_ = std::movable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept CopyMoveAndSwappable_ = std::copyable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept Semiregular_ = std::semiregular<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept Regular_ = std::regular<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept Invocable_ = std::invocable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept RegularInvocable_ = std::regular_invocable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept Predicate_ = std::predicate<T>; /// Specifies that an object of a type can be moved and swapped

    __CASE<typename R, typename T, typename U>
    concept EquivalenceRelation_ = std::equivalence_relation<R, T, U>; /// Specifies that an object of a type can be moved and swapped

    __CASE<typename R, typename T, typename U>
    concept StrictWeakOrder_ = std::strict_weak_order<R, T, U>; /// Specifies that an object of a type can be moved and swapped


    /// Range concepts

    __CASET
    concept Range_ = std::ranges::range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept BorrowedRange_ = std::ranges::borrowed_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept SizedRange_ = std::ranges::sized_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept RangeView_ = std::ranges::view<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept InputRange_ = std::ranges::input_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASE<typename T, typename U>
    concept OutputRange_ = std::ranges::output_range<T, U>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept ForwardRange_ = std::ranges::forward_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept BidirectionalRange_ = std::ranges::bidirectional_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept RandomAccessRange_ = std::ranges::random_access_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept ContiguousRange_ = std::ranges::contiguous_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept CommonRange_ = std::ranges::common_range<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept ViewableRange_ = std::ranges::viewable_range<T>; /// Specifies that an object of a type can be moved and swapped

    /* Iterator concepts */

    __CASET
    concept IndirectlyReadable_ = std::indirectly_readable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASE<typename T, typename U>
    concept IndirectlyWritable_ = std::indirectly_writable<T, U>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept WeaklyIncrementableIter_ = std::weakly_incrementable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept IncrementableIter_ = std::incrementable<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept InputOrOutputIter_ = std::input_or_output_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    __CASE<typename SentinelT, typename IterT>
    concept SentinelForIter_ = std::sentinel_for<SentinelT, IterT>; /// Specifies that an object of a type can be moved and swapped

    __CASE<typename SentinelT, typename IterT>
    concept SizedSentinelForIter_ = std::sized_sentinel_for<SentinelT, IterT>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept InputIter_ = std::input_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept OutputIter_ = std::input_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept ForwardIter_ = std::forward_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept BidirectionalIter_ = std::bidirectional_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept RandomAccessIter_ = std::random_access_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    __CASET
    concept ContiguousIter_ = std::contiguous_iterator<T>; /// Specifies that an object of a type can be moved and swapped

    /* Indirect callable concepts */


    /* <@concept:IndirectlyUnaryInvocable_>
     * <@brief:Specifies that a callable type can be invoked with the result of dereferencing
     * an indirectly_readable type.>
     * */
    __CASE<typename T, typename U>
    concept IndirectlyUnaryInvocable_ = std::indirectly_unary_invocable<T, U>;


    /* <@concept:IndirectlyRegularUnaryInvocable_>
     * <@brief:Specifies that a callable type can be invoked with the result of dereferencing
     * an indirectly_readable regular type.>
     * */
    __CASE<typename T, typename U>
    concept IndirectlyRegularUnaryInvocable_ = std::indirectly_regular_unary_invocable<T, U>;


    /* <@concept:IndirectUnaryPredicate_>
     * <@brief:specifies that a callable type, when invoked with the result of dereferencing an
     * indirectly_readable type, satisfies predicate.>
     * */
    __CASE<typename T, typename U>
    concept IndirectUnaryPredicate_ = std::indirect_unary_predicate<T, U>;


    /* <@concept:IndirectBinaryPredicate_>
     * <@brief:	Specifies that a callable type, when invoked with the result of dereferencing two
     * indirectly_readable types, satisfies predicate.>
     * */
    __CASE<typename T, typename U, typename V>
    concept IndirectBinaryPredicate_ = std::indirect_binary_predicate<T, U, V>;


    /* <@concept:IndirectEquivalenceRelation_>
     * <@brief:	Specifies that a callable type, when invoked with the result of dereferencing two
     * indirectly_readable types, satisfies equivalence_relation.>
     * */
    __CASE<typename T, typename U, typename V = U>
    concept IndirectEquivalenceRelation_ = std::indirect_equivalence_relation<T, U, V>;


    /* <@concept:IndirectStrictWeakOrder_>
     * <@brief:	specifies that a callable type, when invoked with the result of dereferencing two
     * indirectly_readable types, satisfies strict_weak_order.>
     * */
    __CASE<typename T, typename U, typename V = U>
    concept IndirectStrictWeakOrder_ = std::indirect_strict_weak_order<T, U, V>;


    /* specifies that values may be moved from an indirectly_readable type to an indirectly_writable type */
    __CASE<typename T, typename U> concept IndirectlyMovable_ = std::indirectly_movable<T, U>;

    /* specifies that values may be moved from an indirectly_readable type to an indirectly_writable
     * type and that the move may be performed via an intermediate object */
    __CASE<typename T, typename U> concept IndirectlyMovableStorage_ = std::indirectly_movable_storable<T, U>;

    /* specifies that values may be copied from an indirectly_readable type to an indirectly_writable type */
    __CASE<typename T, typename U> concept IndirectlyCopyable_ = std::indirectly_copyable<T, U>;

    /* specifies that values may be copied from an indirectly_readable type to an indirectly_writable
     * type and that the copy may be performed via an intermediate object */
    __CASE<typename T, typename U> concept IndirectlyCopyableStorage_ = std::indirectly_copyable_storable<T, U>;

    /* specifies that the values referenced by two indirectly_readable types can be swapped */
    __CASE<typename T, typename U> concept IndirectlySwappable_ = std::indirectly_swappable<T, U>;

    /* specifies that the values referenced by two indirectly_readable types can be compared */
    __CASE<typename T, typename U, typename V, typename X = std::identity, typename Y = std::identity>
    concept IndirectlyComparable_ = std::indirectly_comparable<T, U, V, X, Y>;

    /* <@concept: Mergeable_>
    * <@brief: Specifies the common requirements of algorithms that reorder elements in place.>
    * */
    __CASET concept Permutable_ = std::permutable<T>;

    /* <@concept: Mergeable_>
     * <@brief: Specifies the requirements for algorithms that merge two input ranges
     * into a single output range according to the strict weak ordering imposed by X.>
     * */
    __CASE<typename T, typename U, typename V, typename X = std::ranges::less, typename Y = std::identity, typename Z = std::identity
                                                                                                                        >
    concept Mergeable_ = std::mergeable<T, U, V, X, Y, Z>;

    /* <@concept: Mergeable_>
     * <@brief: Specifies the requirements for algorithms that permute a range into
     * an ordered range according to R..>
     * */
    __CASE<typename T, typename U = std::ranges::less, typename V = std::identity
                                                                    >
    concept Sortable_ = std::sortable<T, U, V>;

} // end gtl::req

#endif // GTL_HEADGUARD_require_gtl_hpp
