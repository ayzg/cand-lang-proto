#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
#include "cand_constants.hpp"
#include "token.hpp"
#include "cand_errors.hpp"
namespace caoco {
	class tokenizer {
		// Constants
		SL_CXS char8_t EOF_CHAR = ca_constant::u8::EOF_CHAR;
	public:
		using lex_result = sl_partial_expected<tk, sl_char8_vector_cit>;
		using tokenizer_result = sl_expected<tk_vector>;
		// Internal classes
		//class lex_result;
		//struct lex_error;
		private:
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

		constexpr tokenizer_result tokenize();
	public:
		tokenizer() = delete;
		SL_CX explicit tokenizer(sl_char8_vector_cit beg, sl_char8_vector_cit end) {
			beg_ = beg;
			end_ = end;
		};
		tokenizer_result operator()(){
			// Check for empty input
			if (beg_ == end_) {
				return tokenizer_result::make_failure("Empty input");
			}
			return tokenize();
		}
	};

	//// Internal classes
	//class tokenizer::lex_result {
	//	tk token_;
	//	sl_char8_vector_cit end_it_;
	//	sl_string error_;
	//public:
	//	SL_CX lex_result(tk token, sl_char8_vector_cit end_it, sl_string error = "")
	//		: token_(token), end_it_(end_it), error_(error) {}
	//	SL_CX const tk& token() const { return token_; }
	//	SL_CX const sl_string& error() const { return error_; }
	//	SL_CX sl_char8_vector_cit end_it() const { return end_it_; }
	//};

	//struct tokenizer::lex_error : public std::logic_error {
	//	auto make_error(sl_size line, sl_size col, sl_string error) {
	//		return "Error at line: " + std::to_string(line)
	//			+ ", column: " + std::to_string(col)
	//			+ " Details: " + error + "\n";

	//	}
	//	lex_error(sl_size line, sl_size col, sl_string error)
	//		: std::logic_error(make_error(line, col, error)) {}
	//};

	// Main tokenizer method
	constexpr tokenizer::tokenizer_result tokenizer::tokenize() {
		enum keyword_syntax_switch{
			keyword_syntax_switch_none,
			keyword_syntax_switch_directive,
			keyword_syntax_switch_keyword
		}keyword_syntax_switch;
		bool keyword_syntax_switched = false;
		auto swap_keyword_syntax = 
			[&keyword_syntax_switch,&keyword_syntax_switched](tk token,sl_size line,sl_size col)->sl_boolerror {
			if (token.is_directive()){
				if (keyword_syntax_switched) {
					if(token.literal()[0] == u8'#'){
						if (keyword_syntax_switch == keyword_syntax_switch_keyword) {
							return "Directive in Keyword File. Mixing keyword and directive keyword syntax in a single file is forbbiden.";
						}
					}
					else {
						if (keyword_syntax_switch == keyword_syntax_switch_directive) {
							return "Keyword in Directive File. Mixing keyword and directive keyword syntax in a single file is forbbiden.";
						}
					}
				}
				else {
					if (token.literal()[0] == u8'#') {
						keyword_syntax_switch = keyword_syntax_switch_directive;
						keyword_syntax_switched = true;
					}
					else {
						keyword_syntax_switch = keyword_syntax_switch_keyword;
						keyword_syntax_switched = true;
					}
				}
			}
			return true;
		};
		
		sl_char8_vector_cit it = beg_;
		tk_vector output_tokens;
		sl_size current_line = 1;
		sl_size current_col = 1;

		// Lambda for executing a lexer and updating the iterator.
		auto perform_lex = [&](auto lexer) SL_CX-> sl_expected<bool> {
			lex_result lex_result = (this->*lexer)(it);
			if(!lex_result.valid()){
			    return  sl_expected<bool>::make_failure(lex_result.error_message());
			}
			const tk& result_token = lex_result.expected();
			sl_char8_vector_cit result_end = lex_result.always();
			
			if (result_token.type() == tk_enum::none_) { // No match, try next lexer
				return sl_expected<bool>::make_success(false);
			}
			//else if (result_token.type() == tk_enum::invalid_) {
			//	throw lex_error(current_line,current_col,lex_result.error());
			//}
			else { // Lexing was successful
				// Update position based on the number of characters consumed
				current_line += std::count(it, result_end, '\n');

				// Find the last newline before the current character
				sl_char8_vector_cit last_newline = std::find(sl_reverse_iterator(result_end), sl_reverse_iterator(it), '\n').base();
				// If there is no newline before the current character, use the start of the string
				if (last_newline == end_) {
					last_newline = beg_;
				}

				// Calculate the character index within the line
				current_col = std::distance(last_newline, result_end);

				// SPECIAL CASE: Keyword syntax switch
				auto switch_result = swap_keyword_syntax(result_token,current_line,current_col);
				if(!switch_result.valid())
					return sl_expected<bool>::make_failure(switch_result.error_message());

				// Set the line and col of the resulting token and emplace it into the output vector
				output_tokens.push_back(tk(result_token.type(), result_token.beg(), result_token.end(),current_line,current_col));
				it = result_end; // Advance the iterator to the end of lexing. Note lex end and token end may differ.
				return sl_expected<bool>::make_success(true);
			}
		};

		// Attempt to lex a token using one of the lexers until one succeeds. If none succeed, report error.
		// Order of lexers is important. For example, the identifier lexer will match keywords, so it must come after the keyword lexer.
		while (it != end_) {
			bool match = false;
			for (auto lexer : { &tokenizer::lex_solidus,&tokenizer::lex_apostrophe,&tokenizer::lex_newline,
					&tokenizer::lex_whitespace,&tokenizer::lex_eof,& tokenizer::lex_directive,&tokenizer::lex_number,&tokenizer::lex_alnumus,
					&tokenizer::lex_candi_special,&tokenizer::lex_operator,&tokenizer::lex_scopes, &tokenizer::lex_eos,
					&tokenizer::lex_comma, &tokenizer::lex_period }) {
				auto lex_result = perform_lex(lexer);
				if (!lex_result.valid()) { // Error inside one of the lexers
					return tokenizer_result::make_failure(
						ca_error::tokenizer::lexer_syntax_error(current_line, current_col, get(it),lex_result.error_message()));
				}
				else if (lex_result.expected()) {
					// Note: The iterator 'it' is advanced in perform_lex lambda.
					match = true;
					break; // Exit for-loop
				}
			}

			if (!match) { // None of the lexers matched, report an error
				return tokenizer_result::make_failure(
					ca_error::tokenizer::invalid_char(current_line, current_col, get(it)));
			}
		}

		// Remove redundant tokens after lexing
		// Note: "i" is used instead of "it" to avoid ambiguity with the iterator above.
		tk_vector sanitized = [&]() SL_CX{
			tk_vector new_output;
			for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
				const sl_ilist<tk_enum> REDUNDANT_TOKEN_KINDS{
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

		return tokenizer_result::make_success(sanitized);
	} // end tokenize

	// Lexer's Utility methods
	SL_CXIN tokenizer::lex_result tokenizer::make_result(tk_enum type, sl_char8_vector_cit beg_it, sl_char8_vector_cit end_it) {
		return lex_result::make_success(end_it, tk(type, beg_it, end_it));
	}

	SL_CXIN tokenizer::lex_result tokenizer::make_none_result(sl_char8_vector_cit beg_it) {
		return lex_result::make_success(beg_it,tk(tk_enum::none_, beg_it, beg_it));
	}

	SL_CXIN tokenizer::lex_result tokenizer::make_invalid_result(sl_char8_vector_cit beg_it, const sl_string& error) {
		return lex_result::make_failure(beg_it, error);
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
		using namespace ca_constant;
		auto begin = it;
		if (get(it) == u8::chars::DIV) {
			if (peek(it, 1) == u8::chars::DIV && peek(it, 2) != u8::chars::DIV) {			// Line comment two solidus '//' closed by '\n'
				while (!char_traits::is_newline(get(it)) && get(it) != u8::EOF_CHAR) {
					advance(it);
				}
				return make_result(tk_enum::line_comment_,begin,it);
			}
			else if (peek(it, 1) == u8::chars::DIV && peek(it, 2) == u8::chars::DIV) {	// Block comment three solidus '///' closed by '///'
				advance(it, 3);
				while (!find_forward(it, u8::other::BLOCK_COMMENT)) {
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
				if (get(it) == u8::chars::EQ) {
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
		using namespace ca_constant;
		auto beg = it;
		if (get(it) == u8::chars::HASH) {
			advance(it);
			while (char_traits::is_alus(get(it))) {
				advance(it);
			} // keyword is from keyword_begin to the next non-alpha
			// From beg to it must be one of the candi directive keywords, otherwise error.
			if (find_forward(beg, u8::directives::ENTER))
				return make_result(tk_enum::enter_, beg, it);
			else if (find_forward(beg, u8::directives::START))
				return make_result(tk_enum::start_, beg, it);
			else if (find_forward(beg, u8::directives::INCLUDE))
				return make_result(tk_enum::include_, beg, it);
			else if (find_forward(beg, u8::directives::MACRO))
				return make_result(tk_enum::macro_, beg, it);
			else if (find_forward(beg, u8::directives::TYPE))
				return make_result(tk_enum::type_, beg, it);
			else if (find_forward(beg, u8::directives::VAR))
				return make_result(tk_enum::var_, beg, it);
			else if (find_forward(beg, u8::directives::CLASS))
				return make_result(tk_enum::class_, beg, it);
			else if (find_forward(beg, u8::directives::OBJECT))
				return make_result(tk_enum::obj_, beg, it);
			else if (find_forward(beg, u8::directives::PRIVATE))
				return make_result(tk_enum::private_, beg, it);
			else if (find_forward(beg, u8::directives::PUBLIC))
				return make_result(tk_enum::public_, beg, it);
			else if (find_forward(beg, u8::directives::FUNC))
				return make_result(tk_enum::func_, beg, it);
			else if (find_forward(beg, u8::directives::CONST))
				return make_result(tk_enum::const_, beg, it);
			else if (find_forward(beg, u8::directives::STATIC))
				return make_result(tk_enum::static_, beg, it);
			else if (find_forward(beg, u8::directives::IF))
				return make_result(tk_enum::if_, beg, it);
			else if (find_forward(beg, u8::directives::ELSE))
				return make_result(tk_enum::else_, beg, it);
			else if (find_forward(beg, u8::directives::ELIF))
				return make_result(tk_enum::elif_, beg, it);
			else if (find_forward(beg, u8::directives::WHILE))
				return make_result(tk_enum::while_, beg, it);
			else if (find_forward(beg, u8::directives::FOR))
				return make_result(tk_enum::for_, beg, it);
			else if (find_forward(beg, u8::directives::ON))
				return make_result(tk_enum::on_, beg, it);
			else if (find_forward(beg, u8::directives::BREAK))
				return make_result(tk_enum::break_, beg, it);
			else if (find_forward(beg, u8::directives::CONTINUE))
				return make_result(tk_enum::continue_, beg, it);
			else if (find_forward(beg, u8::directives::RETURN))
				return make_result(tk_enum::return_, beg, it);
			else if (find_forward(beg, u8::directives::PRINT))
				return make_result(tk_enum::print_, beg, it);
			else if (find_forward(beg, u8::directives::NONE))
				return make_result(tk_enum::none_literal_, beg, it);
			else if (find_forward(beg, u8::directives::INT))
				return make_result(tk_enum::aint_, beg, it);
			else if (find_forward(beg, u8::directives::UINT))
				return make_result(tk_enum::auint_, beg, it);
			else if (find_forward(beg, u8::directives::REAL))
				return make_result(tk_enum::areal_, beg, it);
			else if (find_forward(beg, u8::directives::BYTE))
				return make_result(tk_enum::aoctet_, beg, it); // TODO: make byte
			else if (find_forward(beg, u8::directives::BIT))
				return make_result(tk_enum::abit_, beg, it);
			else if (find_forward(beg, u8::directives::STR))
				return make_result(tk_enum::astr_, beg, it);
			else {
				return make_invalid_result(beg, "Invalid keyword:" + sl_string(beg, it));
			}
		}
		else if (char_traits::is_alpha(get(it))) {
			advance(it);
			//auto beg = it;
			while (char_traits::is_alus(get(it))) {
				advance(it);
			} 


			if (find_forward(beg, u8::keywords::ENTER))
				return make_result(tk_enum::enter_, beg, it);
			else if (find_forward(beg, u8::keywords::START))
				return make_result(tk_enum::start_, beg, it);
			else if (find_forward(beg, u8::keywords::INCLUDE))
				return make_result(tk_enum::include_, beg, it);
			else if (find_forward(beg, u8::keywords::MACRO))
				return make_result(tk_enum::macro_, beg, it);
			else if (find_forward(beg, u8::keywords::TYPE))
				return make_result(tk_enum::type_, beg, it);
			else if (find_forward(beg, u8::keywords::VAR))
				return make_result(tk_enum::var_, beg, it);
			else if (find_forward(beg, u8::keywords::CLASS))
				return make_result(tk_enum::class_, beg, it);
			else if (find_forward(beg, u8::keywords::OBJECT))
				return make_result(tk_enum::obj_, beg, it);
			else if (find_forward(beg, u8::keywords::PRIVATE))
				return make_result(tk_enum::private_, beg, it);
			else if (find_forward(beg, u8::keywords::PUBLIC))
				return make_result(tk_enum::public_, beg, it);
			else if (find_forward(beg, u8::keywords::FUNC))
				return make_result(tk_enum::func_, beg, it);
			else if (find_forward(beg, u8::keywords::CONST))
				return make_result(tk_enum::const_, beg, it);
			else if (find_forward(beg, u8::keywords::STATIC))
				return make_result(tk_enum::static_, beg, it);
			else if (find_forward(beg, u8::keywords::IF))
				return make_result(tk_enum::if_, beg, it);
			else if (find_forward(beg, u8::keywords::ELSE))
				return make_result(tk_enum::else_, beg, it);
			else if (find_forward(beg, u8::keywords::ELIF))
				return make_result(tk_enum::elif_, beg, it);
			else if (find_forward(beg, u8::keywords::WHILE))
				return make_result(tk_enum::while_, beg, it);
			else if (find_forward(beg, u8::keywords::FOR))
				return make_result(tk_enum::for_, beg, it);
			else if (find_forward(beg, u8::keywords::ON))
				return make_result(tk_enum::on_, beg, it);
			else if (find_forward(beg, u8::keywords::BREAK))
				return make_result(tk_enum::break_, beg, it);
			else if (find_forward(beg, u8::keywords::CONTINUE))
				return make_result(tk_enum::continue_, beg, it);
			else if (find_forward(beg, u8::keywords::RETURN))
				return make_result(tk_enum::return_, beg, it);
			else if (find_forward(beg, u8::keywords::PRINT))
				return make_result(tk_enum::print_, beg, it);
			else if (find_forward(beg, u8::keywords::NONE))
				return make_result(tk_enum::none_literal_, beg, it);
			else if (find_forward(beg, u8::keywords::INT))
				return make_result(tk_enum::aint_, beg, it);
			else if (find_forward(beg, u8::keywords::UINT))
				return make_result(tk_enum::auint_, beg, it);
			else if (find_forward(beg, u8::keywords::REAL))
				return make_result(tk_enum::areal_, beg, it);
			else if (find_forward(beg, u8::keywords::BYTE))
				return make_result(tk_enum::aoctet_, beg, it); // TODO: make byte
			else if (find_forward(beg, u8::keywords::BIT))
				return make_result(tk_enum::abit_, beg, it);
			else if (find_forward(beg, u8::keywords::STR))
				return make_result(tk_enum::astr_, beg, it);
			else {
				return make_none_result(beg);
			}
		}
		else {
			return make_none_result(beg);
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