#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
#include "cand_syntax.hpp"
#include "compiler_error.hpp"

class tokenizer {
	// Constants
	SL_CXS char8_t EOF_CHAR = grammar::characters::EOFILE::u8;
public:
	using lex_result = sl_partial_expected<tk, sl_char8_vector_cit>;
	using tokenizer_result = sl_expected<tk_vector>;
	private:
	SL_CXIN lex_result make_result(e_tk type, sl_char8_vector_cit beg_it, sl_char8_vector_cit end_it);
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
	SL_CX lex_result lex_quotation(sl_char8_vector_cit it);
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
	SL_CX lex_result lex_period(sl_char8_vector_cit it);

	constexpr tokenizer_result tokenize();
public:
	explicit tokenizer(sl_char8_vector_cit beg, sl_char8_vector_cit end) : beg_(beg), end_(end) {}
	tokenizer_result operator()(){
		// Check for empty input
		if (beg_ == end_) {
			return tokenizer_result::make_failure("Empty input");
		}
		return tokenize();
	}
};

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
		if (token.is_keyword()){
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
		tk result_token = lex_result.expected();
		sl_char8_vector_cit result_end = lex_result.always();
			
		if (result_token.type() == e_tk::none_) { // No match, try next lexer
			return sl_expected<bool>::make_success(false);
		}
		//else if (result_token.type() == e_tk::invalid_) {
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
			current_col = static_cast<sl_size>(std::distance(last_newline, result_end)); // Will always be a positive number

			// SPECIAL CASE: Keyword syntax switch
			auto switch_result = swap_keyword_syntax(result_token,current_line,current_col);
			if(!switch_result.valid())
				return sl_expected<bool>::make_failure(switch_result.error_message());

			// Set the line and col of the resulting token and emplace it into the output vector
			result_token.set_line(current_line);
			result_token.set_col(current_col);
			output_tokens.push_back(result_token);
			it = result_end; // Advance the iterator to the end of lexing. Note lex end and token end may differ.
			return sl_expected<bool>::make_success(true);
		}
	};

	// Attempt to lex a token using one of the lexers until one succeeds. If none succeed, report error.
	// Order of lexers is important. For example, the identifier lexer will match keywords, so it must come after the keyword lexer.
	while (it != end_) {
		bool match = false;
		for (auto lexer : { &tokenizer::lex_solidus,&tokenizer::lex_quotation,&tokenizer::lex_newline,
				&tokenizer::lex_whitespace,&tokenizer::lex_eof,& tokenizer::lex_directive,&tokenizer::lex_number,&tokenizer::lex_alnumus,
				&tokenizer::lex_operator,&tokenizer::lex_scopes, &tokenizer::lex_eos,
				&tokenizer::lex_comma, &tokenizer::lex_period }) {
			auto lex_result = perform_lex(lexer);
			if (!lex_result.valid()) { // Error inside one of the lexers
				return tokenizer_result::make_failure(
					compiler_error::tokenizer::lexer_syntax_error(current_line, current_col, get(it),lex_result.error_message()));
			}
			else if (lex_result.expected()) {
				// Note: The iterator 'it' is advanced in perform_lex lambda.
				match = true;
				break; // Exit for-loop
			}
		}

		if (!match) { // None of the lexers matched, report an error
			return tokenizer_result::make_failure(
				compiler_error::tokenizer::invalid_char(current_line, current_col, get(it)));
		}
	}

	// Remove redundant tokens after lexing
	// Note: "i" is used instead of "it" to avoid ambiguity with the iterator above.
	tk_vector sanitized = [&]() SL_CX{
		tk_vector new_output;
		for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
			const sl_ilist<e_tk> REDUNDANT_TOKEN_KINDS{
				e_tk::whitespace_,
				e_tk::line_comment_,
				e_tk::block_comment_,
				e_tk::newline_
			};

			if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
				[i](e_tk match) { return match == i->type(); })) {
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
SL_CXIN tokenizer::lex_result tokenizer::make_result(e_tk type, sl_char8_vector_cit beg_it, sl_char8_vector_cit end_it) {
	return lex_result::make_success(end_it, tk(type, beg_it, end_it));
}

SL_CXIN tokenizer::lex_result tokenizer::make_none_result(sl_char8_vector_cit beg_it) {
	return lex_result::make_success(beg_it,tk(e_tk::none_, beg_it, beg_it));
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
	if (std::distance(it, end_) < static_cast<std::ptrdiff_t>(characters.size())) return false; // Out of bounds cant match
	auto end = std::next(it, static_cast<std::ptrdiff_t>(characters.size()));
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
	using namespace grammar::characters;
	auto begin = it;
	if (get(it) == DIV::u8) {
		if (peek(it, 1) == DIV::u8 && peek(it, 2) != DIV::u8) {			// Line comment two solidus '//' closed by '\n'
			while (!char_traits::is_newline(get(it)) && get(it) != EOF_CHAR) {
				advance(it);
			}
			return make_result(e_tk::line_comment_,begin,it);
		}
		else if (peek(it, 1) == DIV::u8 && peek(it, 2) == DIV::u8) {	// Block comment three solidus '///' closed by '///'
			advance(it, 3);
			while (!find_forward(it, grammar::BLOCK_COMMENT::u8)) {
				advance(it);
			}
			advance(it, 3);
			/* Cursor is at end of block,
				1. move to next character, (1)
				2. advance past block comment closing(3)
				3. then go 1 past to the end.(1)
				Total: 5
			*/
			return  make_result(e_tk::block_comment_, begin, it);
		}
		else {
			advance(it);
			// if the next character is a '=' then we have a division assignment operator
			if (get(it) == EQ::u8) {
				advance(it);
				return  make_result(e_tk::division_assignment_, begin, it);
			}
			// otherwise we have a division operator
			else
				return  make_result(e_tk::division_, begin, it);
		}
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_quotation(sl_char8_vector_cit it) {
	using namespace grammar::characters;
	auto begin = it;
	if (get(it) == APOSTROPHE::u8) {
		advance(it);

		while (!(get(it) == APOSTROPHE::u8 && peek(it, -1) != BACKLASH::u8)) {
			advance(it);
		}
		advance(it);

		// Check for byte literal
		if (get(it) == u8'c') {
			advance(it);
			return  make_result(e_tk::byte_literal_, begin, it);
		}
		else
			return make_result(e_tk::string_literal_, begin, it);
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
		return make_result(e_tk::newline_, begin, it);
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
		return make_result(e_tk::whitespace_, begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_eof(sl_char8_vector_cit it) {
	auto begin = it;
	if (get(it) == EOF_CHAR) {
		advance(it);
		return make_result(e_tk::eof_, begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_number(sl_char8_vector_cit it) {
	using namespace grammar;
	auto begin = it;
	if (char_traits::is_numeric(get(it))) {
		//Special case for 1b and 0b
		if(get(it) == '1' && peek(it,1) == 'b'){
			advance(it,2);
			return make_result(e_tk::bit_literal_, begin, it);
		}
		else if(get(it) == '0' && peek(it,1) == 'b'){
			advance(it,2);
			return make_result(e_tk::bit_literal_, begin, it);
		}

		while (char_traits::is_numeric(get(it))) {
			advance(it);
		}

		// Special case for unsigned literal (overflow is handled by the parser)
		if (get(it) == 'u') {
			advance(it);
			return make_result(e_tk::unsigned_literal_, begin, it);
		}

		// Special case for byte literal(overflow is handled by the parser)
		if (get(it) == 'c') {
			advance(it);
			return make_result(e_tk::byte_literal_, begin, it);
		}

		// If number is followed by elipsis. Return the number.
		if(find_forward(it,scopes::ELLIPSIS::u8)){
			return make_result(e_tk::number_literal_, begin, it);
		}

		// Else process a floating literal.
		if (get(it) == characters::PERIOD::u8) {
			advance(it);
			while (char_traits::is_numeric(get(it))) {
				advance(it);
			}
			return make_result(e_tk::real_literal_, begin, it);
		}

		return make_result(e_tk::number_literal_, begin, it);
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
		return make_result(e_tk::alnumus_, begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_directive(sl_char8_vector_cit it) {
	using namespace grammar;
	auto beg = it;
	if (get(it) == characters::HASH::u8) {
		advance(it);
		while (char_traits::is_alus(get(it))) {
			advance(it);
		} // keyword is from keyword_begin to the next non-alpha
		// From beg to it must be one of the candi directive keywords, otherwise error.
		if (find_forward(beg, directives::ENTER::u8))
			return make_result(e_tk::enter_, beg, it);
		else if (find_forward(beg, directives::START::u8))
			return make_result(e_tk::start_, beg, it);
		else if (find_forward(beg, directives::INCLUDE::u8))
			return make_result(e_tk::include_, beg, it);
		else if (find_forward(beg, directives::MACRO::u8))
			return make_result(e_tk::macro_, beg, it);
		else if (find_forward(beg, directives::ENDMACRO::u8))
			return make_result(e_tk::endmacro_, beg, it);
		else if (find_forward(beg, directives::USE::u8))
			return make_result(e_tk::use_, beg, it);
		else if (find_forward(beg, directives::CLASS::u8))
			return make_result(e_tk::class_, beg, it);
		else if (find_forward(beg, directives::OBJ::u8))
			return make_result(e_tk::obj_, beg, it);
		else if (find_forward(beg, directives::PRIVATE::u8))
			return make_result(e_tk::private_, beg, it);
		else if (find_forward(beg, directives::PUBLIC::u8))
			return make_result(e_tk::public_, beg, it);
		else if (find_forward(beg, directives::CONST::u8))
			return make_result(e_tk::const_, beg, it);
		else if (find_forward(beg, directives::STATIC::u8))
			return make_result(e_tk::static_, beg, it);
		else if (find_forward(beg, directives::IF::u8))
			return make_result(e_tk::if_, beg, it);
		else if (find_forward(beg, directives::ELSE::u8))
			return make_result(e_tk::else_, beg, it);
		else if (find_forward(beg, directives::ELIF::u8))
			return make_result(e_tk::elif_, beg, it);
		else if (find_forward(beg, directives::WHILE::u8))
			return make_result(e_tk::while_, beg, it);
		else if (find_forward(beg, directives::FOR::u8))
			return make_result(e_tk::for_, beg, it);
		else if (find_forward(beg, directives::SWITCH::u8))
			return make_result(e_tk::switch_, beg, it);
		else if (find_forward(beg, directives::BREAK::u8))
			return make_result(e_tk::break_, beg, it);
		else if (find_forward(beg, directives::CONTINUE::u8))
			return make_result(e_tk::continue_, beg, it);
		else if (find_forward(beg, directives::RETURN::u8))
			return make_result(e_tk::return_, beg, it);
		else if (find_forward(beg, directives::PRINT::u8))
			return make_result(e_tk::print_, beg, it);
		else if (find_forward(beg, directives::TYPE::u8))
			return make_result(e_tk::type_, beg, it);
		else if (find_forward(beg, directives::VALUE::u8))
			return make_result(e_tk::value_, beg, it);
		else if (find_forward(beg, directives::IDENTITY::u8))
			return make_result(e_tk::identity_, beg, it);
		else if (find_forward(beg, directives::NONE::u8))
			return make_result(e_tk::none_literal_, beg, it);
		else if (find_forward(beg, directives::INT::u8))
			return make_result(e_tk::int_, beg, it);
		else if (find_forward(beg, directives::UINT::u8))
			return make_result(e_tk::uint_, beg, it);
		else if (find_forward(beg, directives::REAL::u8))
			return make_result(e_tk::real_, beg, it);
		else if (find_forward(beg, directives::BYTE::u8))
			return make_result(e_tk::byte_, beg, it); 
		else if (find_forward(beg, directives::BIT::u8))
			return make_result(e_tk::bit_, beg, it);
		else if (find_forward(beg, directives::STR::u8))
			return make_result(e_tk::str_, beg, it);
		else if (find_forward(beg, directives::ARRAY::u8))
			return make_result(e_tk::array_, beg, it);
		else if (find_forward(beg, directives::POINTER::u8))
			return make_result(e_tk::pointer_, beg, it);
		else if (find_forward(beg, directives::MEMORY::u8))
			return make_result(e_tk::memory_, beg, it);
		else if (find_forward(beg, directives::FUNCTION::u8))
			return make_result(e_tk::function_, beg, it);
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


		if (find_forward(beg, keywords::ENTER::u8))
			return make_result(e_tk::enter_, beg, it);
		else if (find_forward(beg, keywords::START::u8))
			return make_result(e_tk::start_, beg, it);
		else if (find_forward(beg, keywords::INCLUDE::u8))
			return make_result(e_tk::include_, beg, it);
		else if (find_forward(beg, keywords::MACRO::u8))
			return make_result(e_tk::macro_, beg, it);
		else if (find_forward(beg, keywords::ENDMACRO::u8))
			return make_result(e_tk::endmacro_, beg, it);
		else if (find_forward(beg, keywords::USE::u8))
			return make_result(e_tk::use_, beg, it);
		else if (find_forward(beg, keywords::CLASS::u8))
			return make_result(e_tk::class_, beg, it);
		else if (find_forward(beg, keywords::OBJ::u8))
			return make_result(e_tk::obj_, beg, it);
		else if (find_forward(beg, keywords::PRIVATE::u8))
			return make_result(e_tk::private_, beg, it);
		else if (find_forward(beg, keywords::PUBLIC::u8))
			return make_result(e_tk::public_, beg, it);
		else if (find_forward(beg, keywords::CONST::u8))
			return make_result(e_tk::const_, beg, it);
		else if (find_forward(beg, keywords::STATIC::u8))
			return make_result(e_tk::static_, beg, it);
		else if (find_forward(beg, keywords::IF::u8))
			return make_result(e_tk::if_, beg, it);
		else if (find_forward(beg, keywords::ELSE::u8))
			return make_result(e_tk::else_, beg, it);
		else if (find_forward(beg, keywords::ELIF::u8))
			return make_result(e_tk::elif_, beg, it);
		else if (find_forward(beg, keywords::WHILE::u8))
			return make_result(e_tk::while_, beg, it);
		else if (find_forward(beg, keywords::FOR::u8))
			return make_result(e_tk::for_, beg, it);
		else if (find_forward(beg, keywords::SWITCH::u8))
			return make_result(e_tk::switch_, beg, it);
		else if (find_forward(beg, keywords::BREAK::u8))
			return make_result(e_tk::break_, beg, it);
		else if (find_forward(beg, keywords::CONTINUE::u8))
			return make_result(e_tk::continue_, beg, it);
		else if (find_forward(beg, keywords::RETURN::u8))
			return make_result(e_tk::return_, beg, it);
		else if (find_forward(beg, keywords::PRINT::u8))
			return make_result(e_tk::print_, beg, it);
		else if (find_forward(beg, keywords::TYPE::u8))
			return make_result(e_tk::type_, beg, it);
		else if (find_forward(beg, keywords::VALUE::u8))
			return make_result(e_tk::value_, beg, it);
		else if (find_forward(beg, keywords::IDENTITY::u8))
			return make_result(e_tk::identity_, beg, it);
		else if (find_forward(beg, keywords::NONE::u8))
			return make_result(e_tk::none_literal_, beg, it);
		else if (find_forward(beg, keywords::INT::u8))
			return make_result(e_tk::int_, beg, it);
		else if (find_forward(beg, keywords::UINT::u8))
			return make_result(e_tk::uint_, beg, it);
		else if (find_forward(beg, keywords::REAL::u8))
			return make_result(e_tk::real_, beg, it);
		else if (find_forward(beg, keywords::BYTE::u8))
			return make_result(e_tk::byte_, beg, it); 
		else if (find_forward(beg, keywords::BIT::u8))
			return make_result(e_tk::bit_, beg, it);
		else if (find_forward(beg, keywords::STR::u8))
			return make_result(e_tk::str_, beg, it);
		else if (find_forward(beg, keywords::ARRAY::u8))
			return make_result(e_tk::array_, beg, it);
		else if (find_forward(beg, keywords::POINTER::u8))
			return make_result(e_tk::pointer_, beg, it);
		else if (find_forward(beg, keywords::MEMORY::u8))
			return make_result(e_tk::memory_, beg, it);
		else if (find_forward(beg, keywords::FUNCTION::u8))
			return make_result(e_tk::function_, beg, it);
		else {
			return make_none_result(beg);
		}
	}
	else {
		return make_none_result(beg);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_operator(sl_char8_vector_cit it) {
	using namespace grammar::characters;
	auto begin = it;
	if (get(it) == EQ::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::equal_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::simple_assignment_, begin, it);
		}
	}
	else if (get(it) == ADD::u8) {
		if (peek(it, 1) == ADD::u8) {
			advance(it,2);
			return make_result(e_tk::increment_, begin, it);
		}
		else if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::addition_assignment_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::addition_, begin, it);
		}
	}
	else if (get(it) == SUB::u8) {
		if (peek(it, 1) == SUB::u8) {
			advance(it,2);
			return make_result(e_tk::decrement_, begin, it);
		}
		else if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::subtraction_assignment_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::subtraction_, begin, it);
		}
	}
	else if (get(it) == MUL::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::multiplication_assignment_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::multiplication_, begin, it);
		}
	}
	else if (get(it) == DIV::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::division_assignment_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::division_, begin, it);
		}
	}
	else if (get(it) == MOD::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::remainder_assignment_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::remainder_, begin, it);
		}
	}
	else if (get(it) == AND::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::bitwise_and_assignment_, begin, it);
		}
		else if (peek(it, 1) == AND::u8) {
			advance(it,2);
			return make_result(e_tk::logical_and_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::bitwise_and_, begin, it);
		}
	}
	else if (get(it) == OR::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::bitwise_or_assignment_, begin, it);
		}
		else if (peek(it, 1) == OR::u8) {
			advance(it,2);
			return make_result(e_tk::logical_or_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::bitwise_or_, begin, it);
		}
	}
	else if (get(it) == XOR::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::bitwise_xor_assignment_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::bitwise_xor_, begin, it);
		}
	}
	else if (get(it) == LSH::u8) {
		if (peek(it, 1) == LSH::u8) {
			if (peek(it, 2) == EQ::u8) {
				advance(it,3);
				return make_result(e_tk::left_shift_assignment_, begin, it);
			}
			else {
				advance(it,2);
				return make_result(e_tk::bitwise_left_shift_, begin, it);
			}
		}
		else if (peek(it, 1) == EQ::u8) {
			if (peek(it, 2) == RSH::u8) {
				advance(it,3);
				return make_result(e_tk::three_way_comparison_, begin, it);
			}
			advance(it,2);
			return make_result(e_tk::less_than_or_equal_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::less_than_, begin, it);
		}
	}
	else if (get(it) == RSH::u8) {
		if (peek(it, 1) == RSH::u8) {
			if (peek(it, 2) == EQ::u8) {
				advance(it,3);
				return make_result(e_tk::right_shift_assignment_, begin, it);
			}
			else {
				advance(it,2);
				return make_result(e_tk::bitwise_right_shift_, begin, it);
			}
		}
		else if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::greater_than_or_equal_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::greater_than_, begin, it);
		}
	}
	else if (get(it) == NOT::u8) {
		if (peek(it, 1) == EQ::u8) {
			advance(it,2);
			return make_result(e_tk::not_equal_, begin, it);
		}
		else {
			advance(it);
			return make_result(e_tk::negation_, begin, it);
		}
	}
	else if (get(it) == TILDE::u8) {
		advance(it);
		return make_result(e_tk::bitwise_not_, begin, it);
	}
	else if (get(it) == COMMERICIAL_AT::u8) {
		advance(it);
		return make_result(e_tk::commerical_at_, begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_scopes(sl_char8_vector_cit it) {
	using namespace grammar::characters;
	auto begin = it;
	if (get(it) == LPAREN::u8) {
		advance(it);
		return make_result(e_tk::open_paren_, begin, it);
	}
	else if (get(it) == RPAREN::u8) {
		advance(it);
		return make_result(e_tk::close_paren_, begin, it);
	}
	else if (get(it) == LBRACE::u8) {
		advance(it);
		return make_result(e_tk::open_brace_, begin, it);
	}
	else if (get(it) == RBRACE::u8) {
		advance(it);
		return make_result(e_tk::close_brace_, begin, it);
	}
	else if (get(it) == LBRACKET::u8) {
		advance(it);
		return make_result(e_tk::open_bracket_, begin, it);
	}
	else if (get(it) == RBRACKET::u8) {
		advance(it);
		return make_result(e_tk::close_bracket_, begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_eos(sl_char8_vector_cit it) {
	auto begin = it;
	if (get(it) == grammar::characters::SEMICOLON::u8) {
		advance(it);
		return make_result(e_tk::semicolon_,begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_comma(sl_char8_vector_cit it) {
	auto begin = it;
	if (get(it) == grammar::characters::COMMA::u8) {
		advance(it);
		return make_result(e_tk::comma_,begin, it);
	}
	else {
		return make_none_result(begin);
	}
}

SL_CX tokenizer::lex_result tokenizer::lex_period(sl_char8_vector_cit it) {
	auto begin = it;
	if (find_forward(it, grammar::scopes::ELLIPSIS::u8)) {
		advance(it,3);
		return make_result(e_tk::ellipsis_,begin, it);
	}
	else if (get(it) == grammar::characters::PERIOD::u8) {
		advance(it);
		return make_result(e_tk::period_, begin, it);
	}
	else {
		return make_none_result(begin);
	}
}
