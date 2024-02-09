#pragma once
#include <vector>
#include <stdexcept>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <tuple>
#include <stack>

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
				return tk{ tk_enum::eof };
			}
			else if (it_ < beg_) {
				return tk{ tk_enum::eof };
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
		std::size_t line() const { return get().line(); }

		// <@method:column> returns the column of the token at the cursor.
		std::size_t column() const { return get().col(); }

		int importance() { 
			return std::get<static_cast<std::size_t>(TokenTraitIndex::importance_)>(token_traits[get().type()]); 
		}
		Associativity associativity() { 
			return std::get< static_cast<std::size_t>(TokenTraitIndex::associativity_)>(token_traits[get().type()]); 
		}
		Operation operation() { 
			return std::get< static_cast<std::size_t>(TokenTraitIndex::operation_)>(token_traits[get().type()]);
		}
		astnode to_statement() {
			return astnode{ std::get< static_cast<std::size_t>(TokenTraitIndex::produced_statement_)>(token_traits[get().type()]) ,it_,it_ + 1 };
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
				throw std::out_of_range("tk_cursor passed advance_to outside of begin.");
			}
			else if (new_cursor >= end_) {
				throw std::out_of_range("tk_cursor passed advance_to outside of end.");
			}
			else
				it_ = new_cursor;
			return *this;
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


		const auto& operator*() const { return it_; }

	};

	// <@class:parser_scope_result>
	// <@brief> A struct containing the result of a scope search. Use member methods to access the scope.
	// <@member:valid> If the scope is valid, invalid scopes indicate a mismatched parenthesis.
	struct parser_scope_result {
		bool valid{ false };
		tk_vector_cit begin_;
		tk_vector_cit end_;

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
				throw std::runtime_error("[PROGRAMMER LOGIC ERROR] find_scope: begin is not at an open token.");
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
			return find_exact(tk_enum::open_scope, tk_enum::close_scope, begin, end);
		}
		static parser_scope_result find_exact_frame(tk_vector_cit begin, tk_vector_cit end) {
			return find_exact(tk_enum::open_frame, tk_enum::close_frame, begin, end);
		}
		static parser_scope_result find_exact_list(tk_vector_cit begin, tk_vector_cit end) {
			return find_exact(tk_enum::open_list, tk_enum::close_list, begin, end);
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
		//tk_enum currrent_scope_type = tk_enum::none;
		std::stack<tk_enum> scope_type_history;
		auto last_open = begin;
		auto last_closed = begin;

		if (begin->type() != open) {
			throw std::runtime_error("find_statement: begin iterator not on an open token.");
		}
		if (std::next(begin)->type() == close)
			return parser_scope_result{ true, begin, begin + 2 }; // Empty statement

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto it = begin + 1; it < end; it++) {

			if (it->type() == tk_enum::open_scope) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope);
				//currrent_scope_type = tk_enum::open_scope;
			}
			else if (it->type() == tk_enum::close_scope) {
				if (scope_type_history.empty() || scope_type_history.top() != tk_enum::open_scope) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw std::runtime_error("find_statement: Found a close_scope before an open_scope.");
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (it->type() == tk_enum::open_frame) {
				frame_scope_depth++;
				//currrent_scope_type = tk_enum::open_frame;
				scope_type_history.push(tk_enum::open_frame);
			}
			else if (it->type() == tk_enum::close_frame) {
				if (scope_type_history.empty() || scope_type_history.top() != tk_enum::open_frame) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw std::runtime_error("find_statement: Found a close frame token before an open frame token.");
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (it->type() == tk_enum::open_list) {
				list_scope_depth++;
				//currrent_scope_type = tk_enum::open_list;
				scope_type_history.push(tk_enum::open_list);
			}
			else if (it->type() == tk_enum::close_list) {
				if (scope_type_history.empty() || scope_type_history.top() != tk_enum::open_list) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw std::runtime_error("find_statement: Found a close list token before an open list token.");
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
					throw std::runtime_error("find_statement: Found a repeated open token before a close token.");
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
	parser_scope_result find_open_statement(tk_enum open, tk_enum close, tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		//tk_enum currrent_scope_type = tk_enum::none;
		std::stack<tk_enum> scope_type_history;
		auto last_open = begin;
		auto last_closed = begin;

		if (begin->type() != open) {
			throw std::runtime_error("find_statement: begin iterator not on an open token.");
		}
		if (std::next(begin)->type() == close)
			return parser_scope_result{ true, begin, begin + 2 }; // Empty statement

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto it = begin + 1; it < end; it++) {

			if (it->type() == tk_enum::open_scope) {
				paren_scope_depth++;
				scope_type_history.push(tk_enum::open_scope);
				//currrent_scope_type = tk_enum::open_scope;
			}
			else if (it->type() == tk_enum::close_scope) {
				if (scope_type_history.top() != tk_enum::open_scope) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw std::runtime_error("find_statement: Found a close_scope before an open_scope.");
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (it->type() == tk_enum::open_frame) {
				frame_scope_depth++;
				//currrent_scope_type = tk_enum::open_frame;
				scope_type_history.push(tk_enum::open_frame);
			}
			else if (it->type() == tk_enum::close_frame) {
				if (scope_type_history.top() != tk_enum::open_frame) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw std::runtime_error("find_statement: Found a close frame token before an open frame token.");
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (it->type() == tk_enum::open_list) {
				list_scope_depth++;
				//currrent_scope_type = tk_enum::open_list;
				scope_type_history.push(tk_enum::open_list);
			}
			else if (it->type() == tk_enum::close_list) {

				if (scope_type_history.top() != tk_enum::open_list) {
					// Has to be a close or error
					if (it->type() == close) {
						last_closed = it;
						break;
					}
					else
						throw std::runtime_error("find_statement: Found a close list token before an open list token.");
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