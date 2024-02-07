#pragma once
#include "char_traits.hpp"
#include "token.hpp"
#include <algorithm>
#include <sstream> // For error reporting
#include <iostream>
#include <fstream>

namespace caoco {
	using char_t = char_traits::char_t;
	template<typename T>
	using vector = std::vector<T>;
	using char_vector = vector<char_t>;
	using string_t = std::basic_string<char_t>;

	// Converts a string_t utf 8 string to a std::string char string
	constexpr inline std::string to_std_string(const string_t& str) {
		return std::string(str.begin(), str.end());
	}

	// Converts a string_t utf 8 string to a vector of chars
	constexpr inline char_vector to_char_vector(const string_t& str) {
		return char_vector(str.begin(), str.end());
	}

	// Converts a regular string literal to a utf 8 char vector
	constexpr inline char_vector to_char_vector(const char* str) {
		return char_vector(str, str + std::char_traits<char>::length(str));
	}

	// Loads a file into a vector of chars
	char_vector load_source_file(std::string name) {
		std::vector<char> bytes;                            
		std::ifstream file(name, std::ios::binary);         
		file.seekg(0, std::ios::end);                       
		std::streamsize size = file.tellg();                
		file.seekg(0, std::ios::beg);                       
		bytes.resize(size);                                 
		file.read(bytes.data(), size);                      
		file.close();                                       

		// if the last char is not a null terminator, add one
		if (bytes.back() != '\0')
			bytes.push_back('\0');

		// Convert the vector of chars to a vector of char_t
		char_vector chars;
		chars.reserve(bytes.size());
		for (auto& c : bytes) {
			chars.push_back(static_cast<char_t>(c));
		}
		return chars;
	}

	class tokenizer {
		// Constants
		static constexpr char_t EOF_CHAR = '\0';

		// Lexing result
		class lex_result {
			Tk token_;
			char_vector::const_iterator end_it_;
			std::string error_;
			public:
				constexpr lex_result(Tk token, char_vector::const_iterator end_it, std::string error = "") : token_(token), end_it_(end_it) ,error_(error){}
				constexpr const Tk & token() const { return token_; }
				constexpr const std::string & error() const { return error_; }
				constexpr char_vector::const_iterator end_it() const { return end_it_; }
		};
		constexpr inline lex_result make_result(Tk::eType type, char_vector::const_iterator beg_it,char_vector::const_iterator end_it) {
			return lex_result(Tk(type, beg_it,end_it),end_it);
		}
		constexpr inline lex_result make_none_result(char_vector::const_iterator beg_it) {
			return lex_result(Tk(Tk::eType::none, beg_it, beg_it), beg_it);
		}
		constexpr inline lex_result make_invalid_result(char_vector::const_iterator beg_it, const std::string& error) {
			return lex_result(Tk(Tk::eType::invalid, beg_it, beg_it), beg_it, error);
		}

		struct LexError : public std::logic_error {
			auto make_error(size_t line, size_t col,std::string error) {
				return "Error at line: " + std::to_string(line) + ", column: " + std::to_string(col) + " Details: " + error + "\n";
				
			}
			LexError(size_t line, size_t col, std::string error) : std::logic_error(make_error(line,col,error)) {}
		};

		// Members
		char_vector::const_iterator beg_;
		char_vector::const_iterator end_;
		
		// Lexer's Utility functions
		// Returns the character at the iterator, or EOF_CHAR if the iterator is anything but a valid iterator
		constexpr char_t get(char_vector::const_iterator it) {
			if (it >= end_) return EOF_CHAR;
			if(it < beg_) return EOF_CHAR;
			return *it;
		}

		// Returns character at the iterator + n, or EOF_CHAR if the iterator is anything but a valid iterator
		constexpr char_t peek(char_vector::const_iterator it,int n){
			if(std::distance(it,end_) < n) return EOF_CHAR; // Out of bounds cant peek
			return get(it + n);
		}

		// Searches forward for a complete match of characters. Starting from it, inclusive. Returns true if found, false otherwise.
		constexpr bool find_forward(char_vector::const_iterator it, string_t characters){
			if(std::distance(it,end_) < characters.size()) return false; // Out of bounds cant match
			//if(it+characters.size() >= end_) return false; // Out of bounds cant match
			auto end = std::next(it, characters.size());
			if(std::equal(it, end, characters.begin(), characters.end())) 
				return true;
			return false;
		}

		// Typedef for std::advance. Advances the iterator by n. No checks performed.
		constexpr char_vector::const_iterator& advance(char_vector::const_iterator& it, int n = 1) {
			std::advance(it, n);
			return it;
		}

		// Lexers
		constexpr lex_result lex_solidus(char_vector::const_iterator it);
		constexpr lex_result lex_apostrophe(char_vector::const_iterator it);
		constexpr lex_result lex_newline(char_vector::const_iterator it);
		constexpr lex_result lex_whitespace(char_vector::const_iterator it);
		constexpr lex_result lex_eof(char_vector::const_iterator it);
		constexpr lex_result lex_number(char_vector::const_iterator it);
		constexpr lex_result lex_alnumus(char_vector::const_iterator it);
		constexpr lex_result lex_directive(char_vector::const_iterator it);
		constexpr lex_result lex_operator(char_vector::const_iterator it);
		constexpr lex_result lex_scopes(char_vector::const_iterator it);
		constexpr lex_result lex_eos(char_vector::const_iterator it);
		constexpr lex_result lex_comma(char_vector::const_iterator it);
		constexpr lex_result lex_candi_special(char_vector::const_iterator it);
		constexpr lex_result lex_period(char_vector::const_iterator it);

		vector<Tk> tokenize() {
			char_vector::const_iterator it = beg_;
			vector<Tk> output_tokens;
			size_t current_line = 1;
			size_t current_col = 1;

			// Lambda for executing a lexer and updating the iterator.
			auto perform_lex = [&](auto lexer) constexpr-> bool {
				lex_result lex_result = (this->*lexer)(it);
				const Tk & result_token = lex_result.token();
				if (lex_result.token().type() == Tk::eType::none) { // No match, try next lexer
					return false;
				}
				else if (lex_result.token().type() == Tk::eType::invalid) {
					throw LexError(current_line,current_col,lex_result.error());
				}
				else { // Lexing was successful
					// Update position based on the number of characters consumed
					current_line += std::count(it, lex_result.end_it(), '\n');

					// Find the last newline before the current character
					char_vector::const_iterator last_newline = std::find(std::reverse_iterator(lex_result.end_it()), std::reverse_iterator(it), '\n').base();
					// If there is no newline before the current character, use the start of the string
					if (last_newline == end_) {
						last_newline = beg_;
					}

					// Calculate the character index within the line
					current_col = std::distance(last_newline, lex_result.end_it());

					// Set the line and col of the resulting token and emplace it into the output vector
					output_tokens.push_back(Tk(result_token.type(), result_token.beg(), result_token.end(),current_line,current_col));
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
						&tokenizer::lex_directive,& tokenizer::lex_candi_special,&tokenizer::lex_operator,&tokenizer::lex_scopes, &tokenizer::lex_eos,
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
					throw LexError(current_line, current_col, char_error.str());
				}
			}

			// Remove redundant tokens after lexing
			// Note: "i" is used instead of "it" to avoid ambiguity with the iterator above.
			vector<Tk> sanitized = [&]() constexpr {
				vector<Tk> new_output;
				for (auto i = output_tokens.cbegin(); i != output_tokens.cend(); ++i) {
					const std::initializer_list<Tk::eType> REDUNDANT_TOKEN_KINDS{
						Tk::eType::whitespace,
						Tk::eType::line_comment,
						Tk::eType::block_comment,
						Tk::eType::newline
					};

					if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
						[i](Tk::eType match) { return match == i->type(); })) {
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
	public:
		tokenizer() = delete;
		constexpr explicit tokenizer(char_vector::const_iterator beg, char_vector::const_iterator end) {
			beg_ = beg;
			end_ = end;
		};
		vector<Tk> operator()(){
			// Check for empty input
			if (beg_ == end_) {
				return vector<Tk>();
			}
			return tokenize();
		}
	};

	// Lexers
	// <@brief> Lexes a solidus '/' character.
	//			Returns: line comments, block comments, division, division assignment
	constexpr tokenizer::lex_result tokenizer::lex_solidus(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == '/') {
			if (peek(it, 1) == '/' && peek(it, 2) != '/') {			// Line comment two solidus '//' closed by '\n'
				while (!char_traits::is_newline(get(it)) && get(it) != '\0') {
					advance(it);
				}
				return make_result(Tk::eType::line_comment,begin,it);
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
				return  make_result(Tk::eType::block_comment, begin, it);
			}
			else {
				advance(it);
				// if the next character is a '=' then we have a division assignment operator
				if (get(it) == '=') {
					advance(it);
					return  make_result(Tk::eType::division_assignment, begin, it);
				}
				// otherwise we have a division operator
				else
					return  make_result(Tk::eType::division, begin, it);
			}
		}
		else {
			return make_none_result(begin);
		}
	}

	// <@brief> Lexes an apostrophe ''' character. Allows for escaped characters. Does not evaluate the escape sequences.
	//			Returns: string literals
	constexpr tokenizer::lex_result tokenizer::lex_apostrophe(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == '\'') {
			advance(it);
			while (!(get(it) == '\'' && peek(it, -1) != '\\')) {
				advance(it);
			}
			advance(it);
			return  make_result(Tk::eType::string_literal, begin, it);
		}
		else {
			return  make_none_result(begin);
		}
	}

	// <@brief> Lexes a newline character. Repetitive newlines are lexed as a single newline.
	//			Returns: newline
	constexpr tokenizer::lex_result tokenizer::lex_newline(char_vector::const_iterator it) {
		auto begin = it;
		if (char_traits::is_newline(get(it))) {
			while (char_traits::is_newline(get(it))) {
				advance(it);
			}
			return make_result(Tk::eType::newline, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	// <@brief> Lexes a whitespace character.
	//			Returns: whitespace
	constexpr tokenizer::lex_result tokenizer::lex_whitespace(char_vector::const_iterator it) {
		auto begin = it;
		if (char_traits::is_whitespace(get(it))) {
			while (char_traits::is_whitespace(get(it))) {
				advance(it);
			}
			return make_result(Tk::eType::whitespace, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_eof(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == '\0') {
			advance(it);
			return make_result(Tk::eType::eof, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_number(char_vector::const_iterator it) {
		auto begin = it;
		if (char_traits::is_numeric(get(it))) {
			while (char_traits::is_numeric(get(it))) {
				advance(it);
			}

			// If number is followed by elipsis. Return the number.
			if(find_forward(it,u8"...")){
				return make_result(Tk::eType::number_literal, begin, it);
			}

			// Else process a floating literal.
			if (get(it) == '.') {
				advance(it);
				while (char_traits::is_numeric(get(it))) {
					advance(it);
				}
				return make_result(Tk::eType::real_literal, begin, it);
			}

			return make_result(Tk::eType::number_literal, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_alnumus(char_vector::const_iterator it) {
		auto begin = it;
		if (char_traits::is_alpha(get(it))) {
			while (char_traits::is_alnumus(get(it))) {
				advance(it);
			}
			return make_result(Tk::eType::alnumus, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_directive(char_vector::const_iterator it) {
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
			// 		func_[#func], none_literal_[#none]
			if (find_forward(keyword_begin, u8"enter")) {
				return make_result(Tk::eType::enter_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"start")) {
				return make_result(Tk::eType::start_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"type")) {
				return make_result(Tk::eType::type_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"var")) {
				return make_result(Tk::eType::var_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"class")) {
				return make_result(Tk::eType::class_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"public")) {
				return make_result(Tk::eType::public_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"const")) {
				return make_result(Tk::eType::const_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"static")) {
				return make_result(Tk::eType::static_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"ref")) {
				return make_result(Tk::eType::ref_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"if")) {
				return make_result(Tk::eType::if_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"else")) {
				return make_result(Tk::eType::else_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"elif")) {
				return make_result(Tk::eType::elif_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"while")) {
				return make_result(Tk::eType::while_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"for")) {
				return make_result(Tk::eType::for_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"switch")) {
				return make_result(Tk::eType::switch_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"case")) {
				return make_result(Tk::eType::case_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"default")) {
				return make_result(Tk::eType::default_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"break")) {
				return make_result(Tk::eType::break_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"continue")) {
				return make_result(Tk::eType::continue_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"return")) {
				return make_result(Tk::eType::return_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"into")) {
				return make_result(Tk::eType::into_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"print")) {
				return make_result(Tk::eType::print_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"func")) {
				return make_result(Tk::eType::func_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"none")) {
				return make_result(Tk::eType::none_literal_, begin, it);
			}
			else {
				return make_invalid_result(begin, "Invalid directive keyword:" + std::string(begin,it));
			}
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_operator(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == '=') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::equal, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::simple_assignment, begin, it);
			}
		}
		else if (get(it) == '+') {
			if (peek(it, 1) == '+') {
				advance(it,2);
				return make_result(Tk::eType::increment, begin, it);
			}
			else if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::addition_assignment, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::addition, begin, it);
			}
		}
		else if (get(it) == '-') {
			if (peek(it, 1) == '-') {
				advance(it,2);
				return make_result(Tk::eType::decrement, begin, it);
			}
			else if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::subtraction_assignment, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::subtraction, begin, it);
			}
		}
		else if (get(it) == '*') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::multiplication_assignment, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::multiplication, begin, it);
			}
		}
		else if (get(it) == '/') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::division_assignment, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::division, begin, it);
			}
		}
		else if (get(it) == '%') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::remainder_assignment, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::remainder, begin, it);
			}
		}
		else if (get(it) == '&') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::bitwise_and_assignment, begin, it);
			}
			else if (peek(it, 1) == '&') {
				advance(it,2);
				return make_result(Tk::eType::logical_AND, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::bitwise_AND, begin, it);
			}
		}
		else if (get(it) == '|') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::bitwise_or_assignment, begin, it);
			}
			else if (peek(it, 1) == '|') {
				advance(it,2);
				return make_result(Tk::eType::logical_OR, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::bitwise_OR, begin, it);
			}
		}
		else if (get(it) == '^') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::bitwise_xor_assignment, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::bitwise_XOR, begin, it);
			}
		}
		else if (get(it) == '<') {
			if (peek(it, 1) == '<') {
				if (peek(it, 2) == '=') {
					advance(it,3);
					return make_result(Tk::eType::left_shift_assignment, begin, it);
				}
				else {
					advance(it,2);
					return make_result(Tk::eType::bitwise_left_shift, begin, it);
				}
			}
			else if (peek(it, 1) == '=') {
				if (peek(it, 2) == '>') {
					advance(it,3);
					return make_result(Tk::eType::three_way_comparison, begin, it);
				}
				advance(it,2);
				return make_result(Tk::eType::less_than_or_equal, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::less_than, begin, it);
			}
		}
		else if (get(it) == '>') {
			if (peek(it, 1) == '>') {
				if (peek(it, 2) == '=') {
					advance(it,3);
					return make_result(Tk::eType::right_shift_assignment, begin, it);
				}
				else {
					advance(it,2);
					return make_result(Tk::eType::bitwise_right_shift, begin, it);
				}
			}
			else if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::greater_than_or_equal, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::greater_than, begin, it);
			}
		}
		else if (get(it) == '!') {
			if (peek(it, 1) == '=') {
				advance(it,2);
				return make_result(Tk::eType::not_equal, begin, it);
			}
			else {
				advance(it);
				return make_result(Tk::eType::negation, begin, it);
			}
		}
		else if (get(it) == '~') {
			advance(it);
			return make_result(Tk::eType::bitwise_NOT, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_scopes(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == '(') {
			advance(it);
			return make_result(Tk::eType::open_scope, begin, it);
		}
		else if (get(it) == ')') {
			advance(it);
			return make_result(Tk::eType::close_scope, begin, it);
		}
		else if (get(it) == '{') {
			advance(it);
			return make_result(Tk::eType::open_list, begin, it);
		}
		else if (get(it) == '}') {
			advance(it);
			return make_result(Tk::eType::close_list, begin, it);
		}
		else if (get(it) == '[') {
			advance(it);
			return make_result(Tk::eType::open_frame, begin, it);
		}
		else if (get(it) == ']') {
			advance(it);
			return make_result(Tk::eType::close_frame, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_eos(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == ';') {
			advance(it);
			return make_result(Tk::eType::eos,begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_comma(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == ',') {
			advance(it);
			return make_result(Tk::eType::comma,begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_candi_special(char_vector::const_iterator it) {
		auto begin = it;
		if (get(it) == '&') {
			advance(it);
			auto keyword_begin = it;
			while (char_traits::is_alpha(get(it))) {
				advance(it);
			} // Keyword is from keyword_begin to the next non-alpha

			// From begin+1 to it must be one of the candi special keywords, otherwise error.
			if (find_forward(keyword_begin, u8"type")) {
				return make_result(Tk::eType::atype_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"identity")) {
				return make_result(Tk::eType::aidentity_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"value")) {
				return make_result(Tk::eType::avalue_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"int")) {
				return make_result(Tk::eType::aint_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"uint")) {
				return make_result(Tk::eType::auint_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"real")) {
				return make_result(Tk::eType::areal_, begin, it);
			}
			//else if (find_forward(keyword_begin, u8"ureal")) {
			//	return make_result(Tk::eType::aureal_, begin, it);
			//}
			else if (find_forward(keyword_begin, u8"octet")) {
				return make_result(Tk::eType::aoctet_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"bit")) {
				return make_result(Tk::eType::abit_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"array")) {
				return make_result(Tk::eType::aarray_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"pointer")) {
				return make_result(Tk::eType::apointer_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"memory")) {
				return make_result(Tk::eType::amemory_, begin, it);
			}
			else if (find_forward(keyword_begin, u8"function")) {
				return make_result(Tk::eType::afunction_, begin, it);
			}
			else {
				return make_none_result(begin);
			}
		}
		else {
			return make_none_result(begin);
		}
	}

	constexpr tokenizer::lex_result tokenizer::lex_period(char_vector::const_iterator it) {
		auto begin = it;
		if (find_forward(it, u8"...")) {
			advance(it,3);
			return make_result(Tk::eType::ellipsis,begin, it);
		}
		else if (get(it) == '.') {
			advance(it);
			return make_result(Tk::eType::period, begin, it);
		}
		else {
			return make_none_result(begin);
		}
	}

}