#pragma once
#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <tuple>
#include <stack>
#include <ranges>
#include <string>
#include "char_traits.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "ast_node.hpp"
#include "syntax_traits.hpp"

namespace caoco {

	enum class mask_policy {
		mandatory,
		optional
	};

	template<tk_enum TOKEN_TYPE, mask_policy POLICY = mask_policy::mandatory>
	struct tk_mask {
		using type_t = std::integral_constant<tk_enum, TOKEN_TYPE>;
		using policy_t = std::integral_constant<mask_policy, POLICY>;
		static consteval tk_enum type() {
			return TOKEN_TYPE;
		}
		static consteval mask_policy policy() {
			return POLICY;
		}
	};

	template<size_t I = 0, typename ...MaskT>
	void constexpr scan_pack_impl(std::tuple<MaskT...> tup, tk_vector_cit it, tk_vector_cit end, bool& is_found) {
		// If we have iterated through all elements
		if
			constexpr (I == sizeof...(MaskT))
		{
			// Last case, if nothing is left to
			// iterate, then exit the function
			return;
		}
		else {
			if (it == end) {
				is_found = false;
				return;
			}
			is_found = false;

			// Check if the current element is mandatory or optional
			if ((std::tuple_element<I, std::tuple<MaskT...>>::type::policy() == mask_policy::mandatory)) {
				// Acces the tuple and go to next element
				if (std::tuple_element_t<I, std::tuple<MaskT...>>::type() == it->type()) {
					is_found = true;
				}
				else {
					is_found = false;
					return;
				}
			}
			else if ((std::tuple_element<I, std::tuple<MaskT...>>::type::policy() == mask_policy::optional)) {
				// Acces the tuple and go to next element
				if (std::tuple_element_t<I, std::tuple<MaskT...>>::type() == it->type()) {
					is_found = true;
				}
				else {
					scan_pack_impl<I + 1>(tup, it, end, is_found);
					return;
				}
			}

			// Going for next element.
			scan_pack_impl<I + 1>(tup, it + 1, end, is_found);
		}

	}

	template<typename... MaskTs>
	constexpr bool scan_tokens(tk_vector_cit it, tk_vector_cit end) {
		bool is_found = false;
		scan_pack_impl(std::tuple<MaskTs...>(), std::move(it), std::move(end), is_found);
		return is_found;
	}

	// Scans for a combination of tokens starting from the beg iterator.
	// Tokens may be specified to be optional or mandatory.
	// In case of optional tokens, if the token is not found, 
	// the following required token will be searched for from that point.
	template<typename MaskTupleT>
	constexpr bool scan_tokens_pack(tk_vector_cit it, tk_vector_cit end) {
		bool is_found = false;
		scan_pack_impl(MaskTupleT(), std::move(it), std::move(end), is_found);
		return is_found;
	}


	// <@class:tk_cursor> An iterator-like struct for tokens. Requires an end and begin iterator. 
	struct tk_cursor {
		tk_vector_cit it_;
		tk_vector_cit beg_;
		tk_vector_cit end_;

		tk_cursor(tk_vector_cit begin, tk_vector_cit end) : it_(begin), beg_(begin), end_(end) {}

		auto end() const { return end_; }
		auto begin() const { return beg_; }
		bool empty() const { return beg_ == end_; }

		// <@method:next> returns the current token on the cursor, if at end or before begin, returns eof token.
		const tk& get() const {
			if (it_ >= end_) {
				return tk{ tk_enum::eof_ };
			}
			else if (it_ < beg_) {
				return tk{ tk_enum::eof_ };
			}
			else
				return *it_;
		}

		// <@method:lit> returns the literal of the token at the cursor.
		sl_u8string lit() const { return get().literal(); }

		// <@method:type> returns the kind of the token at the cursor.
		tk_enum type() const { return get().type(); }

		bool type_is(tk_enum tkt)  const { return get().type_is(tkt); }
		// <@method:line> returns the line of the token at the cursor.
		sl_size line() const { return get().line(); }

		// <@method:column> returns the column of the token at the cursor.
		sl_size column() const { return get().col(); }

		int priority() { 
			return std::get<static_cast<sl_size>(syntax::e_tk_trait::priority_)>(syntax::token_traits[get().type()]);
		}
		syntax::e_assoc associativity() {
			return std::get<static_cast<sl_size>(syntax::e_tk_trait::associativity_)>(syntax::token_traits[get().type()]);
		}
		syntax::e_operation operation() { 
			return std::get<static_cast<sl_size>(syntax::e_tk_trait::operation_)>(syntax::token_traits[get().type()]);
		}
		astnode to_statement() {
			return astnode{ std::get<static_cast<sl_size>(syntax::e_tk_trait::produced_statement_)>(syntax::token_traits[get().type()]) ,it_,it_ + 1 };
		}


		tk_vector_cit get_it() const { return it_; }

		// <@method:advance> advances the cursor by n.
		tk_cursor& advance(int n = 1) {
			if (n != 0) {
				if (it_ + n >= end_) {
					it_ = end_;
				}
				else if (it_ + n < beg_) {
					it_ = beg_;
				}
				else {
					it_ += n;
				}
			}
			return *this;
		}

		// <@method:advance_to> advances the cursor to the new_cursor. Checks that cursor is within beg and end.
		tk_cursor& advance_to(tk_vector_cit new_cursor) {
			if (new_cursor < beg_) {
				throw sl_out_of_range("tk_cursor passed advance_to outside of begin.");
			}
			else if (new_cursor > end_) {
				throw sl_out_of_range("tk_cursor passed advance_to outside of end.");
			}
			else
				it_ = new_cursor;
			return *this;
		}

		// <@method:advance_to> advances the cursor to the new_cursor. Checks that cursor is within beg and end.
		tk_cursor jump_to(tk_vector_cit new_cursor) {
			if (new_cursor < beg_) {
				throw sl_out_of_range("tk_cursor passed advance_to outside of begin.");
			}
			else if (new_cursor > end_) {
				throw sl_out_of_range("tk_cursor passed advance_to outside of end.");
			}
			else {
				auto rt = *this;
				rt.it_ = new_cursor;
				return rt;
			}
		}

		// <@method:next> returns cursor advanced by N. N may be negative.
		tk_cursor next(int n = 1)const {
			tk_cursor next_cursor = *this;
			next_cursor.advance(n);
			return next_cursor;
		}

		// <@method:peek> returns the token at the cursor + n.
		const tk& peek(int n = 0)const {
			return next(n).get();
		}

		// <@method:is_equal> Returns true if the token at the cursor + offset is equal to the kind and literal.
		auto is_equal(tk_enum kind, sl_u8string literal, int offset)const {
			return peek(offset).type() == kind && peek(offset).literal() == literal;
		}

		bool at_end() const { return it_ == end_; }


		const auto& operator*() const { return it_; }
		

		// utils
		// True if it is one of the keywords which represent a type: aint_ auint_ areal_ abyte_ abit_ astr_
		bool is_keyword_type() const {
			return get().type_is(tk_enum::aint_) 
				|| get().type_is(tk_enum::auint_) 
				|| get().type_is(tk_enum::areal_) 
				|| get().type_is(tk_enum::abyte_) 
				|| get().type_is(tk_enum::abit_) 
				|| get().type_is(tk_enum::astr_);
		
		}

	};

	// <@class:parser_scope_result>
	// <@brief> A struct containing the result of a scope search. Use member methods to access the scope.
	// <@member:valid> If the scope is valid, invalid scopes indicate a mismatched parenthesis.
	struct parser_scope_result {
		bool valid{ false };
		tk_vector_cit begin_;
		tk_vector_cit end_;
		sl_string error_message;

		// <@method:contained_end> Returns the end of the scope, not including the close token.
		auto contained_end() const { return end_ - 1; }
		// <@method:contained_begin> Returns the beginning of the scope, not including the open token.
		auto contained_begin() const { return begin_ + 1; }
		// <@method:is_empty> Returns true if the scope is empty.
		auto is_empty() const { return contained_begin() == contained_end(); }
		// <@method:scope_end> Returns the end of the scope, including the close token.
		auto scope_end() const { return end_; }
		// <@method:scope_begin> Returns the beginning of the scope, including the open token.
		auto scope_begin() const { return begin_; }


		parser_scope_result() = default;
		parser_scope_result(bool valid, tk_vector_cit begin, tk_vector_cit end)
			: valid(valid), begin_(begin), end_(end) {}

		static parser_scope_result find_exact(tk_enum open, tk_enum close, tk_vector_cit begin, tk_vector_cit end) {
			auto scope_depth = 0;
			auto last_open = begin;
			auto last_closed = begin;

			if (begin->type() != open) {
				throw sl_runtime_error("[PROGRAMMER LOGIC ERROR] find_scope: begin is not at an open token.");
			}

			// find the last matching close token, if there is no matching close token, return false
			for (auto i = begin; i < end; i++) {
				if (i->type() == open) {
					scope_depth++;
					last_open = i;
				}
				else if (i->type() == close) {
					scope_depth--;
					last_closed = i;
				}

				if (scope_depth == 0) {
					break;
				}
			}

			// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
			if (scope_depth != 0) {
				return parser_scope_result{ false, begin, last_closed + 1 }; // No matching close token
			}
			else {
				return parser_scope_result{ true, begin, last_closed + 1 };
			}

		}
		static parser_scope_result find_exact_paren(tk_vector_cit begin, tk_vector_cit end) {
			return find_exact(tk_enum::open_scope_, tk_enum::close_scope_, begin, end);
		}
		static parser_scope_result find_exact_frame(tk_vector_cit begin, tk_vector_cit end) {
			return find_exact(tk_enum::open_frame_, tk_enum::close_frame_, begin, end);
		}
		static parser_scope_result find_exact_list(tk_vector_cit begin, tk_vector_cit end) {
			return find_exact(tk_enum::open_list_, tk_enum::close_list_, begin, end);
		}
	};

	// Method for determining the start and end of a scope. 
	// The exact scope is defined by the first open parenthesis and the last close parenthesis.
	parser_scope_result find_scope(tk_vector_cit begin, tk_vector_cit end) {
		return parser_scope_result::find_exact_paren(begin, end);
	} // end find_scope
	parser_scope_result find_list(tk_vector_cit begin, tk_vector_cit end) {
		return parser_scope_result::find_exact_list(begin, end);
	} // end find_scope
	parser_scope_result find_frame(tk_vector_cit cursor, tk_vector_cit end) {
		return parser_scope_result::find_exact_frame(cursor, end);
	} // end find_scope


	parser_scope_result find_statement(tk_enum open, tk_enum close, tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		//tk_enum currrent_scope_type = tk_enum::none_;
		std::stack<tk_enum> scope_type_history;
		auto last_open = begin;
		auto last_closed = begin;

		if (begin->type() != open) {
			throw sl_runtime_error("find_statement: begin iterator not on an open token.");
		}
		if (std::next(begin)->type() == close)
			return parser_scope_result{ true, begin, begin + 2 }; // Empty statement

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto it = begin + 1; it < end; it++) {

			if (it->type() == tk_enum::open_scope_) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope_);
				//currrent_scope_type = tk_enum::open_scope_;
			}
			else if (it->type() == tk_enum::close_scope_) {
				if (scope_type_history.empty() || scope_type_history.top() != tk_enum::open_scope_) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw sl_runtime_error("find_statement: Found a close_scope before an open_scope.");
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (it->type() == tk_enum::open_frame_) {
				frame_scope_depth++;
				//currrent_scope_type = tk_enum::open_frame_;
				scope_type_history.push(tk_enum::open_frame_);
			}
			else if (it->type() == tk_enum::close_frame_) {
				if (scope_type_history.empty() || scope_type_history.top() != tk_enum::open_frame_) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw sl_runtime_error("find_statement: Found a close frame token before an open frame token.");
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (it->type() == tk_enum::open_list_) {
				list_scope_depth++;
				//currrent_scope_type = tk_enum::open_list_;
				scope_type_history.push(tk_enum::open_list_);
			}
			else if (it->type() == tk_enum::close_list_) {
				if (scope_type_history.empty() || scope_type_history.top() != tk_enum::open_list_) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw sl_runtime_error("find_statement: Found a close list token before an open list token.");
				}
				scope_type_history.pop();
				list_scope_depth--;
			}

			if (paren_scope_depth == 0 && frame_scope_depth == 0 && list_scope_depth == 0) {
				if (it->type() == close) {
					last_closed = it;
					break;
				}
				else if (it->type() == open)
					throw sl_runtime_error("find_statement: Found a repeated open token before a close token.");
			}
		}

		//if(last_closed == end)
		//	return parser_scope_result{ false, begin, last_closed + 1 }; // No matching close token

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (paren_scope_depth != 0) {
			return parser_scope_result{ false, begin, last_closed + 1 }; // No matching close token for parenthesis
		}
		else if (frame_scope_depth != 0) {
			return parser_scope_result{ false, begin, last_closed + 1 }; // No matching close token for frame
		}
		else if (list_scope_depth != 0) {
			return parser_scope_result{ false, begin, last_closed + 1 }; // No matching close token for list
		}
		else {
			return parser_scope_result{ true, begin, last_closed + 1 };
		}

	} // end find_scope

	// Methods for determining the start and end of a scope.
	parser_scope_result find_paren_scope(tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		sl_stack<tk_enum> scope_type_history;
		tk_cursor last_open = { begin, end };
		tk_vector_cit last_closed = begin;
		tk_vector_cit error_last_closed = begin;

		if (last_open.at_end()) { // Open is at end therefore cannot be closed.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_paren_scope: Open token is at end of token vector.";
			return failure;
		}

		if (!last_open.type_is(tk_enum::open_scope_)) { // No open list token to start with.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_paren_scope: Open token is not an open scope token.";
			return failure;
		}

		if (last_open.next().at_end()) { // End right after open, cannot be closed.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_paren_scope: Open token is at end of token vector.";
			return failure;
		}

		if (last_open.next().type_is(tk_enum::close_scope_)) { // Empty list
			return parser_scope_result{ true, begin, last_open.next(2).get_it() };
		}
	
		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
			if (tk_it->type_is(tk_enum::open_scope_)) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope_);
			}
			else if (tk_it->type_is(tk_enum::close_scope_)) {
				if (scope_type_history.empty()) {
					// This is the end of the scope
					last_closed = tk_it;
					break;
				}

				if (scope_type_history.top() != tk_enum::open_scope_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_paren_scope: Close token ')' without open token '('.";
					return failure;
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (tk_it->type_is(tk_enum::open_frame_)) {

				frame_scope_depth++;
				scope_type_history.push(tk_enum::open_frame_);
			}
			else if (tk_it->type_is(tk_enum::close_frame_)) {
				if (scope_type_history.empty()) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_paren_scope: Close token ']' without open token '['.";
					return failure;
				}

				if (scope_type_history.top() != tk_enum::open_frame_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_paren_scope: Close token ']' without open token '['.";
					return failure;
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (tk_it->type_is(tk_enum::open_list_)) {
				list_scope_depth++;
				scope_type_history.push(tk_enum::open_list_);
			}
			else if (tk_it->type_is(tk_enum::close_list_)) {
					if (scope_type_history.empty()) {
						auto failure = parser_scope_result{ false, begin, end };
						failure.error_message = "find_paren_scope: Close token '}' without open token '{'.";
						return failure;
					}

					if (scope_type_history.top() != tk_enum::open_list_) {
						auto failure = parser_scope_result{ false, begin, end };
						failure.error_message = "find_paren_scope: Close token '}' without open token '{'.";
						return failure;
					}
					scope_type_history.pop();
					list_scope_depth--;
			} // end if
			error_last_closed = tk_it;
		}// end for

		if (paren_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_paren_scope: Parentheses scope contains mismatched parentheses.";
			return failure;
		}
		if (frame_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_paren_scope: Parentheses scope contains mismatched frames.";
			return failure;
		}
		if (list_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_paren_scope: Parentheses scope contains mismatched list scopes.";
			return failure;
		}
		if (last_closed == begin) {
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_paren_scope: Parentheses scope at line:" + std::to_string(last_open.line())
				+ " column:" + std::to_string(last_open.column()) + " was never closed.\n"
				+ "Expected a close token ')' at line:" + std::to_string(error_last_closed->line()) + " column:" + std::to_string(error_last_closed->col());
			return failure;
		}

		return parser_scope_result{ true, begin, last_closed + 1 };
	} // end find_paren_scope
	parser_scope_result find_paren_scope(tk_cursor crsr) {
		return find_paren_scope(crsr.get_it(), crsr.end());
	}

	parser_scope_result find_frame_scope(tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		sl_stack<tk_enum> scope_type_history;
		tk_cursor last_open = { begin, end };
		tk_vector_cit last_closed = begin;
		tk_vector_cit error_last_closed = begin;

		if (last_open.at_end()) { // Open is at end therefore cannot be closed.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_frame_scope: Open token is at end of token vector.";
		}

		if (!last_open.type_is(tk_enum::open_frame_)) { // No open list token to start with.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_frame_scope: Open token is not an open frame token.";
			return failure;
		}

		if (last_open.next().at_end()) { // End right after open, cannot be closed.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_frame_scope: Open token is at end of token vector.";
			return failure;
		}

		if (last_open.next().type_is(tk_enum::close_frame_)) { // Empty list
			return parser_scope_result{ true, begin, last_open.next(2).get_it() };
		}

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
				// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
			if (tk_it->type_is(tk_enum::open_frame_)) {
				frame_scope_depth++;
				scope_type_history.push(tk_enum::open_frame_);
			}
			else if (tk_it->type_is(tk_enum::close_frame_)) {
				if (scope_type_history.empty()) {
					// This is the end of the scope
					last_closed = tk_it;
					break;
				}

				if (scope_type_history.top() != tk_enum::open_frame_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_frame_scope: Close token ']' without open token '['.";
					return failure;
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (tk_it->type_is(tk_enum::open_scope_)) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope_);
			}
			else if (tk_it->type_is(tk_enum::close_scope_)) {
				if (scope_type_history.empty()) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_frame_scope: Close token ')' without open token '('.";
					return failure;
				}

				if (scope_type_history.top() != tk_enum::open_scope_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_frame_scope: Close token ')' without open token '('.";
					return failure;
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (tk_it->type_is(tk_enum::open_list_)) {
				list_scope_depth++;
				scope_type_history.push(tk_enum::open_list_);
			}
			else if (tk_it->type_is(tk_enum::close_list_)) {
				if (scope_type_history.empty()) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_frame_scope: Close token '}' without open token '{'.";
					return failure;
				}

				if (scope_type_history.top() != tk_enum::open_list_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_frame_scope: Close token '}' without open token '{'.";
					return failure;
				}
				scope_type_history.pop();
				list_scope_depth--;
			} // end switch

			error_last_closed = tk_it;
		}

		if (paren_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_frame_scope: Frame scope contains mismatched parentheses.";
			return failure;
		}
		if (frame_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_frame_scope: Frame scope contains mismatched frames.";
			return failure;
		}
		if (list_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_frame_scope: Frame scope contains mismatched list scopes.";
			return failure;
		}
		if (last_closed == begin) {
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_frame_scope: Frame scope at line:" + std::to_string(last_open.line())
				+ " column:" + std::to_string(last_open.column()) + " was never closed.\n"
				+ "Expected a close token ']' at line:" + std::to_string(error_last_closed->line()) + " column:" + std::to_string(error_last_closed->col());
			return failure;
		}

		return parser_scope_result{ true, begin, last_closed + 1 };
	}
	parser_scope_result find_frame_scope(tk_cursor crsr) {
		return find_frame_scope(crsr.get_it(), crsr.end());
	}

	parser_scope_result find_list_scope(tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		sl_stack<tk_enum> scope_type_history;
		tk_cursor last_open = { begin, end };
		tk_vector_cit last_closed = begin;
		tk_vector_cit error_last_closed = begin;
		if (last_open.at_end()) { // Open is at end therefore cannot be closed.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_list_scope: Open token is at end of token vector.";
			return failure;
		}

		if (!last_open.type_is(tk_enum::open_list_)) { // No open list token to start with.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_list_scope: Open token is not an open list token.";
			return failure;
		}

		if (last_open.next().at_end()) { // End right after open, cannot be closed.
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_list_scope: Open token is at end of token vector.";
			return failure;
		}

		if (last_open.next().type_is(tk_enum::close_list_)) { // Empty list
			return parser_scope_result{ true, begin, last_open.next(2).get_it() };
		}

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto tk_it = (begin + 1); tk_it < end; tk_it++) {
			if (tk_it->type_is(tk_enum::open_scope_)) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope_);
			}
			else if (tk_it->type_is(tk_enum::close_scope_))
			{
				if (scope_type_history.empty()) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_list_scope: Close token ')' without open token '('.";
					return failure;
				}

				if (scope_type_history.top() != tk_enum::open_scope_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_list_scope: Close token ')' without open token '('.";
					return failure;
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (tk_it->type_is(tk_enum::open_frame_))
			{
				frame_scope_depth++;
				scope_type_history.push(tk_enum::open_frame_);
			}
			else if (tk_it->type_is(tk_enum::close_frame_))
			{
				if (scope_type_history.empty()) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_list_scope: Close token ']' without open token '['.";
					return failure;
				}

				if (scope_type_history.top() != tk_enum::open_frame_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_list_scope: Close token ']' without open token '['.";
					return failure;
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (tk_it->type_is(tk_enum::open_list_))
			{
				list_scope_depth++;
				scope_type_history.push(tk_enum::open_list_);
			}
			else if (tk_it->type_is(tk_enum::close_list_)) {
				if (scope_type_history.empty()) {
					last_closed = tk_it;
					break; 					// This is the end of the scope
				}

				if (scope_type_history.top() != tk_enum::open_list_) {
					auto failure = parser_scope_result{ false, begin, end };
					failure.error_message = "find_list_scope: Close token '}' without open token '{'.";
					return failure;
				}
				scope_type_history.pop();
				list_scope_depth--;
			}

			error_last_closed = tk_it;
		}

		if (paren_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_list_scope: List scope contains mismatched parentheses.";
			return failure;
		}
		if (frame_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_list_scope: List scope contains mismatched frames.";
			return failure;
		}
		if (list_scope_depth != 0) {
			auto failure = parser_scope_result{ false, begin, last_closed + 1 };
			failure.error_message = "find_list_scope: List scope contains mismatched list scopes.";
			return failure;
		}
		if (last_closed == begin) {
			auto failure = parser_scope_result{ false, begin, end };
			failure.error_message = "find_list_scope: List scope at line:" + std::to_string(last_open.line())
				+ " column:" + std::to_string(last_open.column()) + " was never closed.\n"
				+ "Expected a close token ']' at line:" + std::to_string(error_last_closed->line()) + " column:" + std::to_string(error_last_closed->col());
			return failure;
		}

		return parser_scope_result{ true, begin, last_closed + 1 };
	}
	parser_scope_result find_list_scope(tk_cursor crsr) {
		return find_list_scope(crsr.get_it(), crsr.end());
	}

	// Method for extracting a seperated parentheses scope. (<separator>)
	sl_vector<parser_scope_result> find_seperated_paren_scopes(tk_vector_cit begin, tk_vector_cit end, tk_enum separator) {
		sl_vector< parser_scope_result> scopes;
		if (begin->type() != tk_enum::open_scope_) {
			scopes.push_back(parser_scope_result{ false, begin, end });
			return scopes;
		}
		sl_stack<tk_enum> scope_type_history;
		tk_vector_cit last_closed = begin;
		begin++; // Skip the open list token
		for (auto i = begin; i < end;) {
			if (i->type_is(separator) && scope_type_history.empty()) {
				scopes.push_back(parser_scope_result{ true, last_closed, i + 1 });
				last_closed = i;
			}
			else if (syntax::is_opening_scope(*i)) {
				scope_type_history.push(i->type());
			}
			else if (syntax::is_closing_scope(*i) && !scope_type_history.empty()) {
				if (syntax::is_closing_scope_of(scope_type_history.top(), i->type())) {
					scope_type_history.pop();
				}
				else {
					scopes.push_back(parser_scope_result{ false, i, end }); // Wrong closing scope
					return scopes;
				}
			}
			else if (i->type_is(tk_enum::close_scope_) && scope_type_history.empty()) {
				// end of list
				scopes.push_back(parser_scope_result{ true, last_closed, i + 1 });
				return scopes;
			}
			else if (i->type_is(tk_enum::eof_)) {
				scopes.push_back(parser_scope_result{ false, i, end }); // End of file
				return scopes;
			}
			sl::advance(i, 1);
		}
	}
	sl_vector<parser_scope_result> find_seperated_paren_scopes(parser_scope_result ls, tk_enum separator) {
		return find_seperated_paren_scopes(ls.scope_begin(), ls.scope_end(), separator);
	}

	// Method for extracting a seperated list scope. {<separator>}
	sl_vector<parser_scope_result> find_seperated_list_scopes(tk_vector_cit begin, tk_vector_cit end, tk_enum separator) {
		sl_vector< parser_scope_result> scopes;
		if (begin->type() != tk_enum::open_list_) {
			scopes.push_back(parser_scope_result{ false, begin, end });
			return scopes;
		}
		sl_stack<tk_enum> scope_type_history;
		tk_vector_cit last_closed = begin;
		begin++; // Skip the open list token
		for (auto i = begin; i < end;) {
			if (i->type_is(separator) && scope_type_history.empty()) {
				scopes.push_back(parser_scope_result{ true, last_closed, i+1 });
				last_closed = i;
			}
			else if(syntax::is_opening_scope(*i)){
				scope_type_history.push(i->type());
			}
			else if(syntax::is_closing_scope(*i) && !scope_type_history.empty()) {
				if (syntax::is_closing_scope_of(scope_type_history.top(), i->type())) {
					scope_type_history.pop();
				}
				else {
					scopes.push_back(parser_scope_result{ false, i, end }); // Wrong closing scope
					return scopes;
				}
			}
			else if(i->type_is(tk_enum::close_list_) && scope_type_history.empty()) {
				// end of list
				scopes.push_back(parser_scope_result{ true, last_closed, i+1});
				return scopes;
			}
			else if(i->type_is(tk_enum::eof_)) {
				scopes.push_back(parser_scope_result{ false, i, end }); // End of file
				return scopes;
			}
			sl::advance(i,1);
		}
	}
	sl_vector<parser_scope_result> find_seperated_list_scopes(parser_scope_result ls, tk_enum separator) {
		return find_seperated_list_scopes(ls.scope_begin(), ls.scope_end(), separator);
	}

	// Method for extracting a seperated frame scope. [<separator>]
	sl_vector<parser_scope_result> find_seperated_frame_scopes(tk_vector_cit begin, tk_vector_cit end, tk_enum separator) {
		sl_vector< parser_scope_result> scopes;
		if (begin->type() != tk_enum::open_frame_) {
			scopes.push_back(parser_scope_result{ false, begin, end });
			return scopes;
		}
		sl_stack<tk_enum> scope_type_history;
		tk_vector_cit last_closed = begin;
		begin++; // Skip the open list token
		for (auto i = begin; i < end;) {
			if (i->type_is(separator) && scope_type_history.empty()) {
				scopes.push_back(parser_scope_result{ true, last_closed, i + 1 });
				last_closed = i;
			}
			else if (syntax::is_opening_scope(*i)) {
				scope_type_history.push(i->type());
			}
			else if (syntax::is_closing_scope(*i) && !scope_type_history.empty()) {
				if (syntax::is_closing_scope_of(scope_type_history.top(), i->type())) {
					scope_type_history.pop();
				}
				else {
					scopes.push_back(parser_scope_result{ false, i, end }); // Wrong closing scope
					return scopes;
				}
			}
			else if (i->type_is(tk_enum::close_frame_) && scope_type_history.empty()) {
				// end of list
				scopes.push_back(parser_scope_result{ true, last_closed, i + 1 });
				return scopes;
			}
			else if (i->type_is(tk_enum::eof_)) {
				scopes.push_back(parser_scope_result{ false, i, end }); // End of file
				return scopes;
			}
			sl::advance(i, 1);
		}
	}
	sl_vector<parser_scope_result> find_seperated_frame_scopes(parser_scope_result ls, tk_enum separator) {
		return find_seperated_frame_scopes(ls.scope_begin(), ls.scope_end(), separator);
	}




	parser_scope_result find_open_statement(tk_enum open, tk_enum close, tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		//tk_enum currrent_scope_type = tk_enum::none_;
		std::stack<tk_enum> scope_type_history;
		auto last_open = begin;
		auto last_closed = begin;

		if(begin+1 == end)
			return parser_scope_result{ false, begin, end + 1 }; // Empty statement

		if (begin->type() != open) {
			throw sl_runtime_error("find_statement: begin iterator not on an open token.");
		}

		if (std::next(begin)->type() == close)
			return parser_scope_result{ true, begin, begin + 2 }; // Empty statement

		// SPECIAL CASE: if the open token is a list,paren or frame-> set scope_type_history and scope depth of the list,paren or frame.
		if (begin->type() == tk_enum::open_scope_) {
			paren_scope_depth++;
			scope_type_history.push(tk_enum::open_scope_);
		}
		else if (begin->type() == tk_enum::open_frame_) {
			frame_scope_depth++;
			scope_type_history.push(tk_enum::open_frame_);
		}
		else if (begin->type() == tk_enum::open_list_) {
			list_scope_depth++;
			scope_type_history.push(tk_enum::open_list_);
		}


		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto it = begin + 1; it < end; it++) {

			if (it->type() == tk_enum::open_scope_) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope_);
				//currrent_scope_type = tk_enum::open_scope_;
			}
			else if (it->type() == tk_enum::close_scope_) {
				if (scope_type_history.empty()) {
					return parser_scope_result{ false, begin, end }; // Close token without open token
				}

				if (scope_type_history.top() != tk_enum::open_scope_) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw sl_runtime_error("find_statement: Found a close_scope before an open_scope.");
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (it->type() == tk_enum::open_frame_) {
				frame_scope_depth++;
				//currrent_scope_type = tk_enum::open_frame_;
				scope_type_history.push(tk_enum::open_frame_);
			}
			else if (it->type() == tk_enum::close_frame_) {
				if (scope_type_history.top() != tk_enum::open_frame_) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw sl_runtime_error("find_statement: Found a close frame token before an open frame token.");
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (it->type() == tk_enum::open_list_) {
				list_scope_depth++;
				//currrent_scope_type = tk_enum::open_list_;
				scope_type_history.push(tk_enum::open_list_);
			}
			else if (it->type() == tk_enum::close_list_) {

				if (scope_type_history.top() != tk_enum::open_list_) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw sl_runtime_error("find_statement: Found a close list token before an open list token.");
				}
				scope_type_history.pop();
				list_scope_depth--;
			}

			if (paren_scope_depth == 0 && frame_scope_depth == 0 && list_scope_depth == 0) {
				if (it->type() == close) {
					last_closed = it;
					break;
				}
			}
		}

		//if(last_closed == end)
		//	return parser_scope_result{ false, begin, last_closed + 1 }; // No matching close token

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (paren_scope_depth != 0) {
			return parser_scope_result{ false, begin, last_closed + 1 }; 
		}
		else if (frame_scope_depth != 0) {
			return parser_scope_result{ false, begin, last_closed + 1 }; 
		}
		else if (list_scope_depth != 0) {
			return parser_scope_result{ false, begin, last_closed + 1 }; 
		}
		else {
			if(last_closed == begin) // Expression was never closed
				return parser_scope_result{ false, begin, last_closed + 1 }; 
			else
				return parser_scope_result{ true, begin, last_closed + 1 };
		}

	} // end find_scope

		// <@method:find_forward> Returns the cursor if the next n tokens match the match vector.
	bool find_forward(tk_vector_cit cursor, tk_vector match) {
		auto end = std::next(cursor, match.size());
		auto found = std::search(cursor, end, match.begin(), match.end(), [](const tk& a, const tk& b) {
			return a.type() == b.type();
		});

		if (found != end) {
			return true;
		}
		return false;
	}
	// <@method:find_forward_exact> Returns true if the next n tokens match the match vector.
	bool find_forward_exact(tk_vector_cit cursor, tk_vector match) {
		auto end = std::next(cursor, match.size());
		if (std::equal(match.begin(), match.end(), cursor, end)) {
			return true;
		}
		return false;
	}

};

//
//my[int] 1;
//oh (what) ret what;
//oh_my = oh(my);
//req(oh_my);
//
