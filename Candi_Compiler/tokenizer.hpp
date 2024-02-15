#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
#include "token.hpp"

namespace caoco {
	class tokenizer {
		// Constants
		SL_CXS char8_t EOF_CHAR = '\0';

		// Internal classes
		class lex_result;
		struct lex_error;

		SL_CXIN lex_result make_result(tk_enum type, sl_char8_vector_cit beg_it, sl_char8_vector_cit end_it);
		SL_CXIN lex_result make_none_result(sl_char8_vector_cit beg_it);
		SL_CXIN lex_result make_invalid_result(sl_char8_vector_cit beg_it, const sl_string& error);

		// Members
		sl_char8_vector_cit beg_;
		sl_char8_vector_cit end_;
		
		// Lexer's Utility functions
		SL_CX char8_t get(sl_char8_vector_cit it);
		SL_CX char8_t peek(sl_char8_vector_cit it, int n);
		SL_CX bool find_forward(sl_char8_vector_cit it, sl_u8string characters);
		SL_CX sl_char8_vector_cit& advance(sl_char8_vector_cit& it, int n = 1);

		// Lexers
		SL_CX lex_result lex_solidus(sl_char8_vector_cit it);
		SL_CX lex_result lex_apostrophe(sl_char8_vector_cit it);
		SL_CX lex_result lex_newline(sl_char8_vector_cit it);
		SL_CX lex_result lex_whitespace(sl_char8_vector_cit it);
		SL_CX lex_result lex_eof(sl_char8_vector_cit it);
		SL_CX lex_result lex_number(sl_char8_vector_cit it);
		SL_CX lex_result lex_alnumus(sl_char8_vector_cit it);
		SL_CX lex_result lex_directive(sl_char8_vector_cit it);
		SL_CX lex_result lex_operator(sl_char8_vector_cit it);
		SL_CX lex_result lex_scopes(sl_char8_vector_cit it);
		SL_CX lex_result lex_eos(sl_char8_vector_cit it);
		SL_CX lex_result lex_comma(sl_char8_vector_cit it);
		SL_CX lex_result lex_candi_special(sl_char8_vector_cit it);
		SL_CX lex_result lex_period(sl_char8_vector_cit it);

		tk_vector tokenize();
	public:
		tokenizer() = delete;
		SL_CX explicit tokenizer(sl_char8_vector_cit beg, sl_char8_vector_cit end) {
			beg_ = beg;
			end_ = end;
		};
		tk_vector operator()(){
			// Check for empty input
			if (beg_ == end_) {
				return tk_vector();
			}
			return tokenize();
		}
	};

	// Internal classes
	class tokenizer::lex_result {
		tk token_;
		sl_char8_vector_cit end_it_;
		sl_string error_;
	public:
		SL_CX lex_result(tk token, sl_char8_vector_cit end_it, sl_string error = "")
			: token_(token), end_it_(end_it), error_(error) {}
		SL_CX const tk& token() const { return token_; }
		SL_CX const sl_string& error() const { return error_; }
		SL_CX sl_char8_vector_cit end_it() const { return end_it_; }
	};

	struct tokenizer::lex_error : public std::logic_error {
		auto make_error(sl_size line, sl_size col, sl_string error) {
			return "Error at line: " + std::to_string(line)
				+ ", column: " + std::to_string(col)
				+ " Details: " + error + "\n";

		}
		lex_error(sl_size line, sl_size col, sl_string error)
			: std::logic_error(make_error(line, col, error)) {}
	};

	// Main tokenizer method
	tk_vector tokenizer::tokenize() {
		sl_char8_vector_cit it = beg_;
		tk_vector output_tokens;
		sl_size current_line = 1;
		sl_size current_col = 1;

		// Lambda for executing a lexer and updating the iterator.
		auto perform_lex = [&](auto lexer) SL_CX-> bool{
			lex_result lex_result = (this->*lexer)(it);
			const tk& result_token = lex_result.token();
			if (lex_result.token().type() == tk_enum::none_) { // No match, try next lexer
				return false;
			}
			else if (lex_result.token().type() == tk_enum::invalid_) {
				throw lex_error(current_line,current_col,lex_result.error());
			}
			else { // Lexing was successful
				// Update position based on the number of characters consumed
				current_line += std::count(it, lex_result.end_it(), '\n');

				// Find the last newline before the current character
				sl_char8_vector_cit last_newline = std::find(sl_reverse_iterator(lex_result.end_it()), sl_reverse_iterator(it), '\n').base();
				// If there is no newline before the current character, use the start of the string
				if (last_newline == end_) {
					last_newline = beg_;
				}

				// Calculate the character index within the line
				current_col = std::distance(last_newline, lex_result.end_it());

				// Set the line and col of the resulting token and emplace it into the output vector
				output_tokens.push_back(tk(result_token.type(), result_token.beg(), result_token.end(),current_line,current_col));
				it = lex_result.end_it(); // Advance the iterator to the end of lexing. Note lex end and token end may differ.
				return true;
			}
		};

		// Attempt to lex a token using one of the lexers until one succeeds. If none succeed, report error.
		// Order of lexers is important. For example, the identifier lexer will match keywords, so it must come after the keyword lexer.
		while (it != end_) {
			bool match = false;
			for (auto lexer : { &tokenizer::lex_solidus,&tokenizer::lex_apostrophe,&tokenizer::lex_newline,
					&tokenizer::lex_whitespace,&tokenizer::lex_eof, &tokenizer::lex_number,&tokenizer::lex_alnumus,
					&tokenizer::lex_directive,&tokenizer::lex_candi_special,&tokenizer::lex_operator,&tokenizer::lex_scopes, &tokenizer::lex_eos,
					&tokenizer::lex_comma, &tokenizer::lex_period }) {
				if (perform_lex(lexer)) {
					// Note: The iterator 'it' is advanced in perform_lex lambda.
					match = true;
					break; // Exit for-loop
				}
			}

			if (!match) {// None of the lexers matched, report an error
				std::stringstream char_error;
				char_error << " Unrecognized character : '" << static_cast<char>(get(it)) << "'\n";
				// Stop lexing and throw. 
				throw lex_error(current_line, current_col, char_error.str());
			}
		}

		// Remove redundant tokens after lexing
		// Note: "i" is used instead of "it" to avoid ambiguity with the iterator above.
		tk_vector sanitized = [&]() SL_CX{
			tk_vector new_output;
			for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
				const std::initializer_list<tk_enum> REDUNDANT_TOKEN_KINDS{
					tk_enum::whitespace_,
					tk_enum::line_comment_,
					tk_enum::block_comment_,
					tk_enum::newline_
				};

				if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
					[i](tk_enum match) { return match == i->type(); })) {
					continue;
				}
				else { // Push back non-redundant tokens
					new_output.push_back(*i);
				}
			}
			return new_output;
		}(); // Note: The lambda is immediately called.

		return sanitized;
	} // end tokenize

	// Lexer's Utility methods
	SL_CXIN tokenizer::lex_result tokenizer::make_result(tk_enum type, sl_char8_vector_cit beg_it, sl_char8_vector_cit end_it) {
		return lex_result(tk(type, beg_it, end_it), end_it);
	}

	SL_CXIN tokenizer::lex_result tokenizer::make_none_result(sl_char8_vector_cit beg_it) {
		return lex_result(tk(tk_enum::none_, beg_it, beg_it), beg_it);
	}

	SL_CXIN tokenizer::lex_result tokenizer::make_invalid_result(sl_char8_vector_cit beg_it, const sl_string& error) {
		return lex_result(tk(tk_enum::invalid_, beg_it, beg_it), beg_it, error);
	}
	
	SL_CX char8_t tokenizer::get(sl_char8_vector_cit it) {
		// EOF_CHAR if it is anything but a valid iterator
		if (it >= end_) return EOF_CHAR;
		if (it < beg_) return EOF_CHAR;
		return *it;
	}

	SL_CX char8_t tokenizer::peek(sl_char8_vector_cit it, int n) {
		if (std::distance(it, end_) < n) return EOF_CHAR; // Out of bounds cant peek
		return get(it + n);
	}
 
	SL_CX bool tokenizer::find_forward(sl_char8_vector_cit it, sl_u8string characters) {
		// Searches forward for a complete match of characters. Starting from it, inclusive.
		if (std::distance(it, end_) < characters.size()) return false; // Out of bounds cant match
		auto end = std::next(it, characters.size());
		if (std::equal(it, end, characters.begin(), characters.end()))
			return true;
		return false;
	}

	SL_CX sl_char8_vector_cit& tokenizer::advance(sl_char8_vector_cit& it, int n) {
		// No checks performed. Use with caution.
		std::advance(it, n);
		return it;
	}

	// Lexers
	SL_CX tokenizer::lex_result tokenizer::lex_solidus(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '/') {
			if (peek(it, 1) == '/' && peek(it, 2) != '/') {			// Line comment two solidus '//' closed by '\n'
				while (!char_traits::is_newline(get(it)) && get(it) != '\0') {
					advance(it);
				}
				return make_result(tk_enum::line_comment_,begin,it);
			}
			else if (peek(it, 1) == '/' && peek(it, 2) == '/') {	// Block comment three solidus '///' closed by '///'
				advance(it, 3);
				while (!find_forward(it, u8"///")) {
					advance(it);
				}
				advance(it, 3);
				/* Cursor is at end of block,
					1. move to next character, (1)
					2. advance past block comment closing(3)
					3. then go 1 past to the end.(1)
					Total: 5
				*/
				return  make_result(tk_enum::block_comment_, begin, it);
			}
			else {
				advance(it);
				// if the next character is a '=' then we have a division assignment operator
				if (get(it) == '=') {
					advance(it);
					return  make_result(tk_enum::division_assignment_, begin, it);
				}
				// otherwise we have a division operator
				else
					return  make_result(tk_enum::division_, begin, it);
			}
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_apostrophe(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '\'') {
			advance(it);
			while (!(get(it) == '\'' && peek(it, -1) != '\\')) {
				advance(it);
			}
			advance(it);

			// Check for octet literal
			if (get(it) == 'c') {
				advance(it);
				return  make_result(tk_enum::octet_literal_, begin, it);
			}
			else
				return make_result(tk_enum::string_literal_, begin, it);
		}
		else {
			return  make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_newline(sl_char8_vector_cit it) {
		auto begin = it;
		if (char_traits::is_newline(get(it))) {
			while (char_traits::is_newline(get(it))) {
				advance(it);
			}
			return make_result(tk_enum::newline_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_whitespace(sl_char8_vector_cit it) {
		auto begin = it;
		if (char_traits::is_whitespace(get(it))) {
			while (char_traits::is_whitespace(get(it))) {
				advance(it);
			}
			return make_result(tk_enum::whitespace_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_eof(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '\0') {
			advance(it);
			return make_result(tk_enum::eof_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_number(sl_char8_vector_cit it) {
		auto begin = it;
		if (char_traits::is_numeric(get(it))) {
			//Special case for 1b and 0b
			if(get(it) == '1' && peek(it,1) == 'b'){
				advance(it,2);
				return make_result(tk_enum::bit_literal_, begin, it);
			}
			else if(get(it) == '0' && peek(it,1) == 'b'){
				advance(it,2);
				return make_result(tk_enum::bit_literal_, begin, it);
			}

			while (char_traits::is_numeric(get(it))) {
				advance(it);
			}

			// Special case for unsigned literal (overflow is handled by the parser)
			if (get(it) == 'u') {
				advance(it);
				return make_result(tk_enum::unsigned_literal_, begin, it);
			}

			// Special case for octet literal(overflow is handled by the parser)
			if (get(it) == 'c') {
				advance(it);
				return make_result(tk_enum::octet_literal_, begin, it);
			}

			// If number is followed by elipsis. Return the number.
			if(find_forward(it,u8"...")){
				return make_result(tk_enum::number_literal_, begin, it);
			}

			// Else process a floating literal.
			if (get(it) == '.') {
				advance(it);
				while (char_traits::is_numeric(get(it))) {
					advance(it);
				}
				return make_result(tk_enum::real_literal_, begin, it);
			}

			return make_result(tk_enum::number_literal_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_alnumus(sl_char8_vector_cit it) {
		auto begin = it;
		if (char_traits::is_alpha(get(it))) {
			while (char_traits::is_alnumus(get(it))) {
				advance(it);
			}
			return make_result(tk_enum::alnumus_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_directive(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '#') {
			advance(it);
			auto keyword_begin = it;
			while (char_traits::is_alpha(get(it))) {
				advance(it);
			} // keyword is from keyword_begin to the next non-alpha

			// From keyword_begin to it must be one of the candi directive keywords, otherwise error.
			// Directive Tokens: 
			//		enter_[#enter] | start_[#start] | type_[#type] | var_[#var] | class_[#class] |
			//		public_[#public] | const_[#const] | static_[#static] | ref_[#ref] |
			//		if_[#if] | else_[#else] | elif_[#elif] | while_[#while] | for_[#for] |
			//		switch_[#switch] | case_[#case] | default_[#default] | break_[#break] |
			//		continue_[#continue] | ret_[#return] | into_[#into] | print_[#print]
			// 		func_[#func], none_literal_[#none], include_[#include]
			if (find_forward(keyword_begin, u8"enter")) {
				return make_result(tk_enum::enter_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"start")) {
				return make_result(tk_enum::start_, begin, it);
			}
			else if(find_forward(keyword_begin, u8"include")){
				return make_result(tk_enum::include_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"type")) {
				return make_result(tk_enum::type_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"var")) {
				return make_result(tk_enum::var_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"class")) {
				return make_result(tk_enum::class_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"public")) {
				return make_result(tk_enum::public_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"const")) {
				return make_result(tk_enum::const_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"static")) {
				return make_result(tk_enum::static_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"ref")) {
				return make_result(tk_enum::ref_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"if")) {
				return make_result(tk_enum::if_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"else")) {
				return make_result(tk_enum::else_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"elif")) {
				return make_result(tk_enum::elif_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"while")) {
				return make_result(tk_enum::while_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"for")) {
				return make_result(tk_enum::for_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"switch")) {
				return make_result(tk_enum::switch_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"case")) {
				return make_result(tk_enum::case_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"default")) {
				return make_result(tk_enum::default_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"break")) {
				return make_result(tk_enum::break_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"continue")) {
				return make_result(tk_enum::continue_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"return")) {
				return make_result(tk_enum::return_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"into")) {
				return make_result(tk_enum::into_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"print")) {
				return make_result(tk_enum::print_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"func")) {
				return make_result(tk_enum::func_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"none")) {
				return make_result(tk_enum::none_literal_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"on")) {
				return make_result(tk_enum::on_, begin, it);
			}
			else {
				return make_invalid_result(begin, "Invalid directive keyword:" + sl_string(begin,it));
			}
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_operator(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '=') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::equal_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::simple_assignment_, begin, it);
			}
		}
		else if (get(it) == '+') {
			if (peek(it, 1) == '+') {
				advance(it,2);
				return make_result(tk_enum::increment_, begin, it);
			}
			else if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::addition_assignment_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::addition_, begin, it);
			}
		}
		else if (get(it) == '-') {
			if (peek(it, 1) == '-') {
				advance(it,2);
				return make_result(tk_enum::decrement_, begin, it);
			}
			else if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::subtraction_assignment_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::subtraction_, begin, it);
			}
		}
		else if (get(it) == '*') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::multiplication_assignment_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::multiplication_, begin, it);
			}
		}
		else if (get(it) == '/') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::division_assignment_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::division_, begin, it);
			}
		}
		else if (get(it) == '%') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::remainder_assignment_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::remainder_, begin, it);
			}
		}
		else if (get(it) == '&') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::bitwise_and_assignment_, begin, it);
			}
			else if (peek(it, 1) == '&') {
				advance(it,2);
				return make_result(tk_enum::logical_AND_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::bitwise_AND_, begin, it);
			}
		}
		else if (get(it) == '|') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::bitwise_or_assignment_, begin, it);
			}
			else if (peek(it, 1) == '|') {
				advance(it,2);
				return make_result(tk_enum::logical_OR_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::bitwise_OR_, begin, it);
			}
		}
		else if (get(it) == '^') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::bitwise_xor_assignment_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::bitwise_XOR_, begin, it);
			}
		}
		else if (get(it) == '<') {
			if (peek(it, 1) == '<') {
				if (peek(it, 2) == '=') {
					advance(it,3);
					return make_result(tk_enum::left_shift_assignment_, begin, it);
				}
				else {
					advance(it,2);
					return make_result(tk_enum::bitwise_left_shift_, begin, it);
				}
			}
			else if (peek(it, 1) == '=') {
				if (peek(it, 2) == '>') {
					advance(it,3);
					return make_result(tk_enum::three_way_comparison_, begin, it);
				}
				advance(it,2);
				return make_result(tk_enum::less_than_or_equal_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::less_than_, begin, it);
			}
		}
		else if (get(it) == '>') {
			if (peek(it, 1) == '>') {
				if (peek(it, 2) == '=') {
					advance(it,3);
					return make_result(tk_enum::right_shift_assignment_, begin, it);
				}
				else {
					advance(it,2);
					return make_result(tk_enum::bitwise_right_shift_, begin, it);
				}
			}
			else if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::greater_than_or_equal_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::greater_than_, begin, it);
			}
		}
		else if (get(it) == '!') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(tk_enum::not_equal_, begin, it);
			}
			else {
				advance(it);
				return make_result(tk_enum::negation_, begin, it);
			}
		}
		else if (get(it) == '~') {
			advance(it);
			return make_result(tk_enum::bitwise_NOT_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_scopes(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '(') {
			advance(it);
			return make_result(tk_enum::open_scope_, begin, it);
		}
		else if (get(it) == ')') {
			advance(it);
			return make_result(tk_enum::close_scope_, begin, it);
		}
		else if (get(it) == '{') {
			advance(it);
			return make_result(tk_enum::open_list_, begin, it);
		}
		else if (get(it) == '}') {
			advance(it);
			return make_result(tk_enum::close_list_, begin, it);
		}
		else if (get(it) == '[') {
			advance(it);
			return make_result(tk_enum::open_frame_, begin, it);
		}
		else if (get(it) == ']') {
			advance(it);
			return make_result(tk_enum::close_frame_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_eos(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == ';') {
			advance(it);
			return make_result(tk_enum::eos_,begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_comma(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == ',') {
			advance(it);
			return make_result(tk_enum::comma_,begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_candi_special(sl_char8_vector_cit it) {
		auto begin = it;
		if (get(it) == '&') {
			advance(it);
			auto keyword_begin = it;
			while (char_traits::is_alpha(get(it))) {
				advance(it);
			} // Keyword is from keyword_begin to the next non-alpha

			// From begin+1 to it must be one of the candi special keywords, otherwise error.
			if (find_forward(keyword_begin, u8"type")) {
				return make_result(tk_enum::atype_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"identity")) {
				return make_result(tk_enum::aidentity_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"value")) {
				return make_result(tk_enum::avalue_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"int")) {
				return make_result(tk_enum::aint_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"uint")) {
				return make_result(tk_enum::auint_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"real")) {
				return make_result(tk_enum::areal_, begin, it);
			}
			//else if (find_forward(keyword_begin, u8"ureal")) {
			//	return make_result(tk_enum::aureal_, begin, it);
			//}
			else if (find_forward(keyword_begin, u8"octet")) {
				return make_result(tk_enum::aoctet_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"bit")) {
				return make_result(tk_enum::abit_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"array")) {
				return make_result(tk_enum::aarray_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"pointer")) {
				return make_result(tk_enum::apointer_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"memory")) {
				return make_result(tk_enum::amemory_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"function")) {
				return make_result(tk_enum::afunction_, begin, it);
			}
			else {
				return make_none_result(begin);
			}
		}
		else {
			return make_none_result(begin);
		}
	}

	SL_CX tokenizer::lex_result tokenizer::lex_period(sl_char8_vector_cit it) {
		auto begin = it;
		if (find_forward(it, u8"...")) {
			advance(it,3);
			return make_result(tk_enum::ellipsis_,begin, it);
		}
		else if (get(it) == '.') {
			advance(it);
			return make_result(tk_enum::period_, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

}