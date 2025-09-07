#pragma once
#include "cand_syntax.hpp"

static const tk sentinel_end_token = e_tk::eof_;
class tk_iterator {
	tk_vector_cit beg_;
	tk_vector_cit end_;
	tk_vector_cit it_;
public:
	// Properties
	SL_CXA end() const { return end_; }
	SL_CXA begin() const { return beg_; }
	SL_CXA it() const { return it_; }
	SL_CX const tk& get() const {
		if (it_ >= end_) {
			return sentinel_end_token;
		}
		return *it_; 
	}
	SL_CX bool at_end() const { return it_ == end_; }
	SL_CX const tk& operator->() const { return get(); }
	
	// Iteration
	// <@method:advance> advances the cursor by n.
	SL_CX tk_iterator& advance(int n = 1) {
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
	SL_CX tk_iterator& advance(tk_vector_cit new_it) {
		if (new_it < beg_) {
			throw sl_out_of_range("tk_cursor passed advance_to outside of begin.");
		}
		else if (new_it > end_) {
			throw sl_out_of_range("tk_cursor passed advance_to outside of end.");
		}
		else
			it_ = new_it;
		return *this;
	}
	// <@method:next> returns cursor advanced by N. N may be negative.
	tk_iterator next(int n = 1)const {
		tk_iterator next_cursor = *this;
		next_cursor.advance(n);
		return next_cursor;
	}
	tk_iterator next(tk_vector_cit new_it) {
		if (new_it < beg_) {
			throw sl_out_of_range("tk_cursor passed advance_to outside of begin.");
		}
		else if (new_it > end_) {
			throw sl_out_of_range("tk_cursor passed advance_to outside of end.");
		}
		else {
			auto rt = *this;
			rt.it_ = new_it;
			return rt;
		}
	}
	// <@method:peek> returns the token at the cursor + n.
	SL_CX const tk& peek(int n = 0)const {
		return next(n).get();
	}
	// <@method:find_forward> True there is a match in the iterator's range. Starting from and including the current token.
	SL_CXA find_forward( tk_vector match) {
		auto end = std::next(it_, match.size());
		auto found = std::search(it_, end, match.begin(), match.end(), [](const tk& a, const tk& b) {
			return a.type() == b.type();
		});

		if (found != end) {
			return true;
		}
		return false;
	}
	// <@method:find_forward_exact> Returns true if the next n tokens match the match vector. Starting from and including the current token.
	SL_CX bool find_forward_exact(tk_vector_cit cursor, tk_vector match) {
		auto end = std::next(cursor, static_cast<std::ptrdiff_t>(match.size()));
		if (std::equal(match.begin(), match.end(), cursor, end)) {
			return true;
		}
		return false;
	}

	// Token Properties
	SL_CXINA type() const noexcept { return get().type(); }
	SL_CXINA node_type() const noexcept { return get().node_type(); }
	SL_CXINA size() const { return get().size(); }
	SL_CXINA line() const noexcept { return get().line(); }
	SL_CXINA col() const noexcept { return get().col(); }
	SL_CXIN const sl_u8string& literal() const { return get().literal(); }
	SL_CXINA priority() const { return get().priority(); }
	SL_CXINA assoc() const { return get().assoc(); }
	SL_CXINA operation() const { return get().operation(); }
	SL_CXINA is_keyword() const noexcept { return get().is_keyword(); }
	SL_CXINA is_opening_scope() const noexcept { return get().is_opening_scope(); }
	SL_CXINA is_closing_scope() const noexcept { return get().is_closing_scope(); }
	SL_CXINA is_closing_scope_of(e_tk open) const { return get().is_closing_scope_of(open); }

	SL_CXINA literal_str() const {
		return get().literal_str();
	}
	SL_CXINA type_is(e_tk type) const noexcept { return get().type_is(type); }
	SL_CXINA type_and_lit_is(e_tk kind, const sl_u8string& literal)const {
		return get().type_and_lit_is(kind, literal);
	}
	// <@method:type_and_lit_is> Returns true if the token at the cursor + offset is equal to the kind and literal.
	SL_CXINA type_and_lit_is(e_tk kind, const sl_u8string& literal, int offset)const {
		return peek(offset).type_and_lit_is(kind, literal);
	}
	SL_CXINA type_is(e_tk kind, int offset)const {
		return peek(offset).type_is(kind);
	}
public:
	tk_iterator() = default;
	tk_iterator(tk_vector_cit begin, tk_vector_cit end)
		: beg_(begin), end_(end), it_(begin) {}
	tk_iterator(tk_vector_cit begin, tk_vector_cit end, tk_vector_cit it)
		: beg_(begin), end_(end), it_(it) {}
	tk_iterator(const tk_iterator& other)
		: beg_(other.beg_), end_(other.end_), it_(other.it_) {}
	tk_iterator& operator=(const tk_iterator& other) {
		beg_ = other.beg_;
		end_ = other.end_;
		it_ = other.it_;
		return *this;
	}
};

class tk_scope {
	bool valid_{ false };
	tk_vector_cit begin_;
	tk_vector_cit end_;
	sl_string error_message;

public:
	const sl_string& error() const { return error_message; }
	bool valid() const { return valid_; }
	// <@method:contained_end> Returns the end of the scope, not including the close token.
	auto contained_end() const { return end_ - 1; }
	// <@method:contained_begin> Returns the beginning of the scope, not including the open token.
	auto contained_begin() const { return begin_ + 1; }
	// <@method:is_empty> Returns true if the scope is empty.
	auto is_empty() const { return contained_begin() == contained_end(); }
	// <@method:scope_end> Returns the end of the scope, including the close token.
	auto end() const { return end_; }
	// <@method:scope_begin> Returns the beginning of the scope, including the open token.
	auto begin() const { return begin_; }

	tk_scope() = default;
	tk_scope(bool valid, tk_vector_cit begin, tk_vector_cit end)
		: valid_(valid), begin_(begin), end_(end) {}


	// Methods for determining the start and end of a scope.
	static tk_scope find_paren(tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		sl_stack<e_tk> scope_type_history;
		tk_iterator last_open = { begin, end };
		tk_vector_cit last_closed = begin;
		tk_vector_cit error_last_closed = begin;

		if (last_open.at_end()) { // Open is at end therefore cannot be closed.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_paren: Open token is at end of token vector.";
			return failure;
		}

		if (!last_open.type_is(e_tk::open_paren_)) { // No open list token to start with.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_paren: Open token is not an open scope token.";
			return failure;
		}

		if (last_open.next().at_end()) { // End right after open, cannot be closed.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_paren: Open token is at end of token vector.";
			return failure;
		}

		if (last_open.next().type_is(e_tk::close_paren_)) { // Empty list
			return tk_scope{ true, begin, last_open.next(2).it() };
		}

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
			if (tk_it->type_is(e_tk::open_paren_)) {
				paren_scope_depth++;
				scope_type_history.push(e_tk::open_paren_);
			}
			else if (tk_it->type_is(e_tk::close_paren_)) {
				if (scope_type_history.empty()) {
					// This is the end of the scope
					last_closed = tk_it;
					break;
				}

				if (scope_type_history.top() != e_tk::open_paren_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_paren: Close token ')' without open token '('.";
					return failure;
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (tk_it->type_is(e_tk::open_bracket_)) {

				frame_scope_depth++;
				scope_type_history.push(e_tk::open_bracket_);
			}
			else if (tk_it->type_is(e_tk::close_bracket_)) {
				if (scope_type_history.empty()) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_paren: Close token ']' without open token '['.";
					return failure;
				}

				if (scope_type_history.top() != e_tk::open_bracket_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_paren: Close token ']' without open token '['.";
					return failure;
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (tk_it->type_is(e_tk::open_brace_)) {
				list_scope_depth++;
				scope_type_history.push(e_tk::open_brace_);
			}
			else if (tk_it->type_is(e_tk::close_brace_)) {
				if (scope_type_history.empty()) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_paren: Close token '}' without open token '{'.";
					return failure;
				}

				if (scope_type_history.top() != e_tk::open_brace_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_paren: Close token '}' without open token '{'.";
					return failure;
				}
				scope_type_history.pop();
				list_scope_depth--;
			} // end if
			error_last_closed = tk_it;
		}// end for

		if (paren_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_paren: Parentheses scope contains mismatched parentheses.";
			return failure;
		}
		if (frame_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_paren: Parentheses scope contains mismatched frames.";
			return failure;
		}
		if (list_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_paren: Parentheses scope contains mismatched list scopes.";
			return failure;
		}
		if (last_closed == begin) {
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_paren: Parentheses scope at line:" + std::to_string(last_open.line())
				+ " column:" + std::to_string(last_open.col()) + " was never closed.\n"
				+ "Expected a close token ')' at line:" + std::to_string(error_last_closed->line()) + " column:" + std::to_string(error_last_closed->col());
			return failure;
		}

		return tk_scope{ true, begin, last_closed + 1 };
	} // end find_paren
	static tk_scope find_paren(tk_iterator crsr) {
		return find_paren(crsr.it(), crsr.end());
	}
	static tk_scope find_brace(tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		sl_stack<e_tk> scope_type_history;
		tk_iterator last_open = { begin, end };
		tk_vector_cit last_closed = begin;
		tk_vector_cit error_last_closed = begin;
		if (last_open.at_end()) { // Open is at end therefore cannot be closed.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_brace: Open token is at end of token vector.";
			return failure;
		}

		if (!last_open.type_is(e_tk::open_brace_)) { // No open list token to start with.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_brace: Open token is not an open list token.";
			return failure;
		}

		if (last_open.next().at_end()) { // End right after open, cannot be closed.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_brace: Open token is at end of token vector.";
			return failure;
		}

		if (last_open.next().type_is(e_tk::close_brace_)) { // Empty list
			return tk_scope{ true, begin, last_open.next(2).it() };
		}

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto tk_it = (begin + 1); tk_it < end; tk_it++) {
			if (tk_it->type_is(e_tk::open_paren_)) {
				paren_scope_depth++;
				scope_type_history.push(e_tk::open_paren_);
			}
			else if (tk_it->type_is(e_tk::close_paren_))
			{
				if (scope_type_history.empty()) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_brace: Close token ')' without open token '('.";
					return failure;
				}

				if (scope_type_history.top() != e_tk::open_paren_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_brace: Close token ')' without open token '('.";
					return failure;
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (tk_it->type_is(e_tk::open_bracket_))
			{
				frame_scope_depth++;
				scope_type_history.push(e_tk::open_bracket_);
			}
			else if (tk_it->type_is(e_tk::close_bracket_))
			{
				if (scope_type_history.empty()) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_brace: Close token ']' without open token '['.";
					return failure;
				}

				if (scope_type_history.top() != e_tk::open_bracket_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_brace: Close token ']' without open token '['.";
					return failure;
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (tk_it->type_is(e_tk::open_brace_))
			{
				list_scope_depth++;
				scope_type_history.push(e_tk::open_brace_);
			}
			else if (tk_it->type_is(e_tk::close_brace_)) {
				if (scope_type_history.empty()) {
					last_closed = tk_it;
					break; 					// This is the end of the scope
				}

				if (scope_type_history.top() != e_tk::open_brace_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_brace: Close token '}' without open token '{'.";
					return failure;
				}
				scope_type_history.pop();
				list_scope_depth--;
			}

			error_last_closed = tk_it;
		}

		if (paren_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_brace: List scope contains mismatched parentheses.";
			return failure;
		}
		if (frame_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_brace: List scope contains mismatched frames.";
			return failure;
		}
		if (list_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_brace: List scope contains mismatched list scopes.";
			return failure;
		}
		if (last_closed == begin) {
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_brace: List scope at line:" + std::to_string(last_open.line())
				+ " column:" + std::to_string(last_open.col()) + " was never closed.\n"
				+ "Expected a close token ']' at line:" + std::to_string(error_last_closed->line()) + " column:" + std::to_string(error_last_closed->col());
			return failure;
		}

		return tk_scope{ true, begin, last_closed + 1 };
	}
	static tk_scope find_brace(tk_iterator crsr) {
		return find_brace(crsr.it(), crsr.end());
	}
	static tk_scope find_bracket(tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		sl_stack<e_tk> scope_type_history;
		tk_iterator last_open = { begin, end };
		tk_vector_cit last_closed = begin;
		tk_vector_cit error_last_closed = begin;

		if (last_open.at_end()) { // Open is at end therefore cannot be closed.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_bracket: Open token is at end of token vector.";
		}

		if (!last_open.type_is(e_tk::open_bracket_)) { // No open list token to start with.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_bracket: Open token is not an open frame token.";
			return failure;
		}

		if (last_open.next().at_end()) { // End right after open, cannot be closed.
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_bracket: Open token is at end of token vector.";
			return failure;
		}

		if (last_open.next().type_is(e_tk::close_bracket_)) { // Empty list
			return tk_scope{ true, begin, last_open.next(2).it() };
		}

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
				// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto tk_it = (begin + 1); tk_it != end; tk_it++) {
			if (tk_it->type_is(e_tk::open_bracket_)) {
				frame_scope_depth++;
				scope_type_history.push(e_tk::open_bracket_);
			}
			else if (tk_it->type_is(e_tk::close_bracket_)) {
				if (scope_type_history.empty()) {
					// This is the end of the scope
					last_closed = tk_it;
					break;
				}

				if (scope_type_history.top() != e_tk::open_bracket_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_bracket: Close token ']' without open token '['.";
					return failure;
				}
				scope_type_history.pop();
				frame_scope_depth--;
			}
			else if (tk_it->type_is(e_tk::open_paren_)) {
				paren_scope_depth++;
				scope_type_history.push(e_tk::open_paren_);
			}
			else if (tk_it->type_is(e_tk::close_paren_)) {
				if (scope_type_history.empty()) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_bracket: Close token ')' without open token '('.";
					return failure;
				}

				if (scope_type_history.top() != e_tk::open_paren_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_bracket: Close token ')' without open token '('.";
					return failure;
				}
				scope_type_history.pop();
				paren_scope_depth--;
			}
			else if (tk_it->type_is(e_tk::open_brace_)) {
				list_scope_depth++;
				scope_type_history.push(e_tk::open_brace_);
			}
			else if (tk_it->type_is(e_tk::close_brace_)) {
				if (scope_type_history.empty()) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_bracket: Close token '}' without open token '{'.";
					return failure;
				}

				if (scope_type_history.top() != e_tk::open_brace_) {
					auto failure = tk_scope{ false, begin, end };
					failure.error_message = "find_bracket: Close token '}' without open token '{'.";
					return failure;
				}
				scope_type_history.pop();
				list_scope_depth--;
			} // end switch

			error_last_closed = tk_it;
		}

		if (paren_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_bracket: Frame scope contains mismatched parentheses.";
			return failure;
		}
		if (frame_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_bracket: Frame scope contains mismatched frames.";
			return failure;
		}
		if (list_scope_depth != 0) {
			auto failure = tk_scope{ false, begin, last_closed + 1 };
			failure.error_message = "find_bracket: Frame scope contains mismatched list scopes.";
			return failure;
		}
		if (last_closed == begin) {
			auto failure = tk_scope{ false, begin, end };
			failure.error_message = "find_bracket: Frame scope at line:" + std::to_string(last_open.line())
				+ " column:" + std::to_string(last_open.col()) + " was never closed.\n"
				+ "Expected a close token ']' at line:" + std::to_string(error_last_closed->line()) + " column:" + std::to_string(error_last_closed->col());
			return failure;
		}

		return tk_scope{ true, begin, last_closed + 1 };
	}
	static tk_scope find_bracket(tk_iterator crsr) {
		return find_bracket(crsr.it(), crsr.end());
	}

	// Method for extracting a seperated parentheses scope. (<separator>)
	static sl_vector<tk_scope> find_seperated_paren(tk_vector_cit begin, tk_vector_cit end, e_tk separator) {
		sl_vector< tk_scope> scopes;
		if (begin->type() != e_tk::open_paren_) {
			scopes.push_back(tk_scope{ false, begin, end });
			return scopes;
		}
		sl_stack<e_tk> scope_type_history;
		tk_vector_cit last_closed = begin;
		begin++; // Skip the open list token
		for (auto i = begin; i < end;) {
			if (i->type_is(separator) && scope_type_history.empty()) {
				scopes.push_back(tk_scope{ true, last_closed, i + 1 });
				last_closed = i;
			}
			else if (i->is_opening_scope()) {
				scope_type_history.push(i->type());
			}
			else if (i->is_closing_scope() && !scope_type_history.empty()) {
				if (tk_type_is_closing_scope_of(scope_type_history.top(), i->type())) {
					scope_type_history.pop();
				}
				else {
					scopes.push_back(tk_scope{ false, i, end }); // Wrong closing scope
					return scopes;
				}
			}
			else if (i->type_is(e_tk::close_paren_) && scope_type_history.empty()) {
				// end of list
				scopes.push_back(tk_scope{ true, last_closed, i + 1 });
				return scopes;
			}
			else if (i->type_is(e_tk::eof_)) {
				scopes.push_back(tk_scope{ false, i, end }); // End of file
				return scopes;
			}
			sl::advance(i, 1);
		}
	}
	static sl_vector<tk_scope> find_seperated_paren(tk_iterator crsr, e_tk separator) {
		return find_seperated_paren(crsr.it(), crsr.end(), separator);
	}
	static sl_vector<tk_scope> find_seperated_paren(tk_scope ls, e_tk separator) {
		return find_seperated_paren(ls.begin(), ls.end(), separator);
	}

	// Method for extracting a seperated list scope. {<separator>}
	static sl_vector<tk_scope> find_seperated_brace(tk_vector_cit begin, tk_vector_cit end, e_tk separator) {
		sl_vector< tk_scope> scopes;
		if (begin->type() != e_tk::open_brace_) {
			scopes.push_back(tk_scope{ false, begin, end });
			return scopes;
		}
		sl_stack<e_tk> scope_type_history;
		tk_vector_cit last_closed = begin;
		begin++; // Skip the open list token
		for (auto i = begin; i < end;) {
			if (i->type_is(separator) && scope_type_history.empty()) {
				scopes.push_back(tk_scope{ true, last_closed, i + 1 });
				last_closed = i;
			}
			else if (i->is_opening_scope()) {
				scope_type_history.push(i->type());
			}
			else if (i->is_closing_scope() && !scope_type_history.empty()) {
				if (tk_type_is_closing_scope_of(scope_type_history.top(), i->type())) {
					scope_type_history.pop();
				}
				else {
					scopes.push_back(tk_scope{ false, i, end }); // Wrong closing scope
					return scopes;
				}
			}
			else if (i->type_is(e_tk::close_brace_) && scope_type_history.empty()) {
				// end of list
				scopes.push_back(tk_scope{ true, last_closed, i + 1 });
				return scopes;
			}
			else if (i->type_is(e_tk::eof_)) {
				scopes.push_back(tk_scope{ false, i, end }); // End of file
				return scopes;
			}
			sl::advance(i, 1);
		}
	}
	static sl_vector<tk_scope> find_seperated_brace(tk_iterator crsr, e_tk separator) {
		return find_seperated_brace(crsr.it(), crsr.end(), separator);
	}
	static sl_vector<tk_scope> find_seperated_brace(tk_scope ls, e_tk separator) {
		return find_seperated_brace(ls.begin(), ls.end(), separator);
	}

	// Method for extracting a seperated frame scope. [<separator>]
	static sl_vector<tk_scope> find_seperated_bracket(tk_vector_cit begin, tk_vector_cit end, e_tk separator) {
		sl_vector< tk_scope> scopes;
		if (begin->type() != e_tk::open_bracket_) {
			scopes.push_back(tk_scope{ false, begin, end });
			return scopes;
		}
		sl_stack<e_tk> scope_type_history;
		tk_vector_cit last_closed = begin;
		begin++; // Skip the open list token
		for (auto i = begin; i < end;) {
			if (i->type_is(separator) && scope_type_history.empty()) {
				scopes.push_back(tk_scope{ true, last_closed, i + 1 });
				last_closed = i;
			}
			else if (i->is_opening_scope()) {
				scope_type_history.push(i->type());
			}
			else if (i->is_closing_scope() && !scope_type_history.empty()) {
				if (tk_type_is_closing_scope_of(scope_type_history.top(),i->type())) {
					scope_type_history.pop();
				}
				else {
					scopes.push_back(tk_scope{ false, i, end }); // Wrong closing scope
					return scopes;
				}
			}
			else if (i->type_is(e_tk::close_bracket_) && scope_type_history.empty()) {
				// end of list
				scopes.push_back(tk_scope{ true, last_closed, i + 1 });
				return scopes;
			}
			else if (i->type_is(e_tk::eof_)) {
				scopes.push_back(tk_scope{ false, i, end }); // End of file
				return scopes;
			}
			sl::advance(i, 1);
		}
	}
	static sl_vector<tk_scope> find_seperated_bracket(tk_iterator crsr, e_tk separator) {
		return find_seperated_bracket(crsr.it(), crsr.end(), separator);
	}
	static sl_vector<tk_scope> find_seperated_bracket(const tk_scope & ls, e_tk separator) {
		return find_seperated_bracket(ls.begin(), ls.end(), separator);
	}

	// Open token may NOT be repeated.
	static tk_scope find_statement(e_tk open, e_tk close, tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		std::stack<e_tk> scope_type_history;
		auto last_open = begin;
		auto last_closed = begin;

		if (begin->type() != open) {
			throw sl_runtime_error("find_statement: begin iterator not on an open token.");
		}
		if (std::next(begin)->type() == close)
			return tk_scope{ true, begin, begin + 2 }; // Empty statement

		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto it = begin + 1; it < end; it++) {

			if (it->type() == e_tk::open_paren_) {
				paren_scope_depth++;
				scope_type_history.push(e_tk::open_paren_);
				//currrent_scope_type = e_tk::open_paren_;
			}
			else if (it->type() == e_tk::close_paren_) {
				if (scope_type_history.empty() || scope_type_history.top() != e_tk::open_paren_) {
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
			else if (it->type() == e_tk::open_bracket_) {
				frame_scope_depth++;
				//currrent_scope_type = e_tk::open_bracket_;
				scope_type_history.push(e_tk::open_bracket_);
			}
			else if (it->type() == e_tk::close_bracket_) {
				if (scope_type_history.empty() || scope_type_history.top() != e_tk::open_bracket_) {
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
			else if (it->type() == e_tk::open_brace_) {
				list_scope_depth++;
				//currrent_scope_type = e_tk::open_brace_;
				scope_type_history.push(e_tk::open_brace_);
			}
			else if (it->type() == e_tk::close_brace_) {
				if (scope_type_history.empty() || scope_type_history.top() != e_tk::open_brace_) {
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
		//	return tk_scope{ false, begin, last_closed + 1 }; // No matching close token

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (paren_scope_depth != 0) {
			return tk_scope{ false, begin, last_closed + 1 }; // No matching close token for parenthesis
		}
		else if (frame_scope_depth != 0) {
			return tk_scope{ false, begin, last_closed + 1 }; // No matching close token for frame
		}
		else if (list_scope_depth != 0) {
			return tk_scope{ false, begin, last_closed + 1 }; // No matching close token for list
		}
		else {
			return tk_scope{ true, begin, last_closed + 1 };
		}

	} // end find_scope
	
	// Open token may be repeated.
	static tk_scope find_open_statement(e_tk open, e_tk close, tk_vector_cit begin, tk_vector_cit end) {
		auto paren_scope_depth = 0;
		auto frame_scope_depth = 0;
		auto list_scope_depth = 0;
		//e_tk currrent_scope_type = e_tk::none_;
		std::stack<e_tk> scope_type_history;
		auto last_open = begin;
		auto last_closed = begin;

		if (begin + 1 == end)
			return tk_scope{ false, begin, end + 1 }; // Empty statement

		if (begin->type() != open) {
			throw sl_runtime_error("find_statement: begin iterator not on an open token.");
		}

		if (std::next(begin)->type() == close)
			return tk_scope{ true, begin, begin + 2 }; // Empty statement

		// SPECIAL CASE: if the open token is a list,paren or frame-> set scope_type_history and scope depth of the list,paren or frame.
		if (begin->type() == e_tk::open_paren_) {
			paren_scope_depth++;
			scope_type_history.push(e_tk::open_paren_);
		}
		else if (begin->type() == e_tk::open_bracket_) {
			frame_scope_depth++;
			scope_type_history.push(e_tk::open_bracket_);
		}
		else if (begin->type() == e_tk::open_brace_) {
			list_scope_depth++;
			scope_type_history.push(e_tk::open_brace_);
		}


		// find the last matching close token that is not within a () [] or {} scope, if there is no matching close token, return false
		for (auto it = begin + 1; it < end; it++) {

			if (it->type() == e_tk::open_paren_) {
				paren_scope_depth++;
				scope_type_history.push(e_tk::open_paren_);
				//currrent_scope_type = e_tk::open_paren_;
			}
			else if (it->type() == e_tk::close_paren_) {
				if (scope_type_history.empty()) {
					return tk_scope{ false, begin, end }; // Close token without open token
				}

				if (scope_type_history.top() != e_tk::open_paren_) {
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
			else if (it->type() == e_tk::open_bracket_) {
				frame_scope_depth++;
				//currrent_scope_type = e_tk::open_bracket_;
				scope_type_history.push(e_tk::open_bracket_);
			}
			else if (it->type() == e_tk::close_bracket_) {
				if (scope_type_history.top() != e_tk::open_bracket_) {
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
			else if (it->type() == e_tk::open_brace_) {
				list_scope_depth++;
				//currrent_scope_type = e_tk::open_brace_;
				scope_type_history.push(e_tk::open_brace_);
			}
			else if (it->type() == e_tk::close_brace_) {

				if (scope_type_history.top() != e_tk::open_brace_) {
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
		//	return tk_scope{ false, begin, last_closed + 1 }; // No matching close token

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (paren_scope_depth != 0) {
			return tk_scope{ false, begin, last_closed + 1 };
		}
		else if (frame_scope_depth != 0) {
			return tk_scope{ false, begin, last_closed + 1 };
		}
		else if (list_scope_depth != 0) {
			return tk_scope{ false, begin, last_closed + 1 };
		}
		else {
			if (last_closed == begin) // Expression was never closed
				return tk_scope{ false, begin, last_closed + 1 };
			else
				return tk_scope{ true, begin, last_closed + 1 };
		}

	} // end find_scope

	// Starts with the begin token which may be repeated, ends with a semicolon_ ';'
	static tk_scope find_program_statement(tk_vector_cit begin, tk_vector_cit end) {
		return tk_scope::find_open_statement(begin->type(), e_tk::semicolon_, begin, end);
	} // end find_scope
};


// Currently Unused?
// <@method:scan_tokens> Token Mask Scanner 
// Scans for a combination of tokens starting from the beg iterator(inclusive).
// Tokens may be specified to be optional or mandatory.
// In case of optional tokens, if the token is not found, 
// the following required token will be searched for from that point.
// 
// scan_tokens takes the masks as variadic template arguments.
// scan_tokens_pack takes the masks as a tuple type.
// 
// Sample Use: int[1..10]
// <aint_ranged> 
//		::= <aint> <open_frame> <unary_minus?> <number_literal>> <ellipsis> <unary_minus?> <number_literal> <close_frame>
// <aint>
//		::= <aint>		
// 
//	using constrained_int_type_mask = std::tuple<
//		tk_mask<e_tk::int_>, tk_mask<e_tk::open_bracket_>, tk_mask<e_tk::subtraction_, mask_policy::optional>,
//		tk_mask<e_tk::number_literal_>, tk_mask<e_tk::ellipsis_>, tk_mask<e_tk::subtraction_, mask_policy::optional>,
//		tk_mask<e_tk::number_literal_>, tk_mask<e_tk::close_bracket_>>;
enum class mask_policy {
	mandatory,
	optional
};

template<e_tk TOKEN_TYPE, mask_policy POLICY = mask_policy::mandatory>
struct tk_mask {
	using type_t = std::integral_constant<e_tk, TOKEN_TYPE>;
	using policy_t = std::integral_constant<mask_policy, POLICY>;
	static consteval e_tk type() {
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

template<typename MaskTupleT>
constexpr bool scan_tokens_pack(tk_vector_cit it, tk_vector_cit end) {
	bool is_found = false;
	scan_pack_impl(MaskTupleT(), std::move(it), std::move(end), is_found);
	return is_found;
}