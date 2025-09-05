#pragma once
#include <iostream>
#include <array>
#include <type_traits>
#include <ranges>

#include "global_typedef.hpp"
#include "char_traits.hpp"
#include "import.hpp"
#include "token.hpp"


namespace cil {
	// Tokenizer is a class that takes a source file and produces a vector of tokens.
	// It is a stateful class that keeps track of the current position in the source file.
	// It is also responsible for error handling and reporting.

	// The Tokenizer class is implemented as a state machine.
	// The state machine is implemented as a series of lexers.
	// Each lexer is a function that takes a cursor and returns a pair of TokenKind and a new cursor.
	// The cursor is a pointer to the current character in the source file.
	// The TokenKind is the type of token that was lexed.
	// The new cursor is the position of the cursor after the lexer has consumed characters from the source file.
	// If the lexer does not match, it returns TokenKind::none and the cursor is not advanced.
	// If the lexer matches, it returns the TokenKind and the cursor is advanced to the end of the lexed token.
	// The Tokenizer class then pushes the lexed token to the output vector and advances the cursor to the new cursor.

	// The Tokenizer class is also responsible for error handling and reporting.
	// If none of the lexers match, the Tokenizer class reports an error and advances the cursor by one character.
	// The Tokenizer class also keeps track of the current line and character position in the source file.
	// The Tokenizer class also keeps track of the last newline character in the source file.
	// This is used to calculate the current character position in the source file.

	// The Tokenizer class is also responsible for input validation.
	// It checks for empty source files, null termination, valid characters, and file size limits.

	// The Tokenizer class is also responsible for handling the end of file.
	// It returns a TokenKind::eof token when it reaches the end of the source file.
	class Tokenizer {
	public:
		using char_iterator = rvector<char>::const_iterator;
		using token_iterator = rvector<Token>::const_iterator;
		using lex_result = std::pair<TokenKind, char_iterator>;
		using token_cref = std::reference_wrapper<const Token>;
		using token_view = rvector<token_cref>;
		using token_view_iterator = token_view::const_iterator;
	private:
		const rvector<char>& source_file;
	public:
		Tokenizer(const rvector<char>& source_file) : source_file(source_file) {}

		// Input Validation
		void validate();

		// Utilities

		char_iterator source_end();
		char_iterator source_begin();
		auto get(char_iterator cursor);
		auto peek(char_iterator cursor, int n);
		auto find_forward(char_iterator cursor, rstring string);
		bool push_and_advance(rvector<Token>& tokens, Tokenizer::char_iterator& cursor, Tokenizer::lex_result lex_result, int& current_line, int& current_char);

		lex_result lex_solidus(char_iterator cursor);
		lex_result lex_apostrophe(char_iterator cursor);
		lex_result lex_newline(char_iterator cursor);
		lex_result lex_whitespace(char_iterator cursor);
		lex_result lex_eof(char_iterator cursor);
		lex_result lex_number(char_iterator cursor);
		lex_result lex_alnumus(char_iterator cursor);
		lex_result lex_directive(char_iterator cursor);
		lex_result lex_operator(char_iterator cursor);
		lex_result lex_scopes(char_iterator cursor);
		lex_result lex_eos(char_iterator cursor);
		lex_result lex_comma(char_iterator cursor);
		lex_result lex_candi_special(char_iterator cursor);

		// Main methods
		rvector<Token> tokenize() {
			rvector<char>::const_iterator cursor = source_begin();
			rvector<char>::const_iterator end = source_end();

			rvector<Token> output_tokens;

			int current_line = 1;
			int current_char = 1;

			auto try_lex = [&](auto lex_function) -> bool {
				auto lex_result = (this->*lex_function)(cursor);
				if (push_and_advance(output_tokens, cursor, lex_result, current_line, current_char)) {
					return true;
				}
				return false;
				};

			auto try_lexers = [&]() -> bool {
				return try_lex(&Tokenizer::lex_solidus) ||
					try_lex(&Tokenizer::lex_apostrophe) ||
					try_lex(&Tokenizer::lex_newline) ||
					try_lex(&Tokenizer::lex_whitespace) ||
					try_lex(&Tokenizer::lex_eof) ||
					try_lex(&Tokenizer::lex_number) ||
					try_lex(&Tokenizer::lex_alnumus) ||
					try_lex(&Tokenizer::lex_directive) ||
					try_lex(&Tokenizer::lex_operator) ||
					try_lex(&Tokenizer::lex_scopes) ||
					try_lex(&Tokenizer::lex_eos) ||
					try_lex(&Tokenizer::lex_comma) ||
					try_lex(&Tokenizer::lex_candi_special);
				};

			while (cursor != end) {
				if (!try_lexers()) {
					// None of the lexers matched, report an error
					std::cerr << "Error at line " << current_line << ", character " << current_char << ": Unrecognized character '" << *cursor << "'\n";
					// Move to the next character and update position
					cursor++;
					current_char++;
				}
			}

			return output_tokens;
		}
	};

	class TokenSanitizer {
	public:
		using token_view = Tokenizer::token_view;
		using token_view_iterator = Tokenizer::token_view_iterator;
	private:
		static constexpr rilist<TokenKind> REDUNDANT_TOKEN_KINDS{
			TokenKind::whitespace,
			TokenKind::line_comment,
			TokenKind::block_comment,
			TokenKind::newline
		};

	public:
		// Returns a non-modifiable vector of const references to the subrange of tokens which are not redundant.
		static const token_view sanitize(const rvector<Token>& tokens) {
			std::vector<std::reference_wrapper<const Token>> sanitized;
			for (auto it = tokens.cbegin(); it != tokens.cend(); ++it) {

				if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
					[it](TokenKind match) { return match == it->type(); })) {
					continue;
				};

				sanitized.push_back(*it);
			}

			return sanitized;
		}
	};

	// Input Validation
	void Tokenizer::validate() {
		// 1. Check for Empty Source File
		if (source_file.empty()) {
			std::cerr << "Error: Empty source file\n";
			// Handle error or exit the program
		}

		// 2. Check for Null Termination
		if (!source_file.empty() && source_file.back() != '\0') {
			std::cerr << "Error: Source file must be null-terminated\n";
			// Handle error or exit the program
		}

		//// 3. Check for Valid Characters
		//for (char c : source) {
		//	if (!isValidCharacter(c)) {
		//		std::cerr << "Error: Invalid character '" << c << "' in source file\n";
		//		// Handle error or exit the program
		//	}
		//}

		// 4. Check for File Size Limits
		constexpr std::size_t maxFileSize = 1000000; // 1 million character limit.
		if (source_file.size() > maxFileSize) {
			std::cerr << "Error: Source file size exceeds the allowed limit\n";
			// Handle error or exit the program
		}
	}

	// Utilities
	Tokenizer::char_iterator Tokenizer::source_end() {
		return source_file.cend();
	}

	Tokenizer::char_iterator Tokenizer::source_begin() {
		return source_file.cbegin();
	}

	auto Tokenizer::get(Tokenizer::char_iterator cursor) {
		if (cursor >= source_end()) {
			return '\0';
		}
		return *cursor;
	}

	auto Tokenizer::peek(Tokenizer::char_iterator cursor, int n) {
		auto peeked = std::next(cursor, n);
		if (peeked >= source_end() || peeked < source_begin()) {
			return '\0';
		}
		return *peeked;
	}

	auto Tokenizer::find_forward(Tokenizer::char_iterator cursor, rstring string) {
		auto end = std::next(cursor, string.size());
		if (std::equal(string.begin(), string.end(), cursor, end)) {
			return true;
		}
		return false;
	}

	bool Tokenizer::push_and_advance(rvector<Token>& tokens, 
		Tokenizer::char_iterator& cursor, 
		Tokenizer::lex_result lex_result,
		int& current_line, 
		int& current_char) {
		if (lex_result.first != TokenKind::none) {
			// Update position based on the number of characters consumed
			current_line += std::count(cursor, lex_result.second, '\n');

			// Find the last newline before the current character
			auto last_newline = std::find(std::reverse_iterator(lex_result.second), std::reverse_iterator(cursor), '\n').base();
			// If there is no newline before the current character, use the start of the string
			if (last_newline == source_end()) {
				last_newline = tokens.begin()->begin;

			}

			// Calculate the character index within the line
			current_char = std::distance(last_newline, lex_result.second);

			tokens.push_back(Token{ lex_result.first, cursor, lex_result.second ,current_char,current_line});
			cursor = lex_result.second;
			return true;
		}
		else {
			return false;
		}
	}

	// Lexers
	Tokenizer::lex_result Tokenizer::lex_solidus(char_iterator cursor) {
		if (get(cursor) == '/') {
			if (peek(cursor, 1) == '/' && peek(cursor, 2) != '/') {			// Line comment two solidus '//' closed by '\n'
				while (!CharTraits::is_newline(get(cursor)) && get(cursor) != '\0') {
					cursor++;
				}
				return std::make_pair(TokenKind::line_comment, cursor);
			}
			else if (peek(cursor, 1) == '/' && peek(cursor, 2) == '/') {	// Block comment three solidus '///' closed by '///'
				cursor += 3;
				//while (get(cursor) != '/' && peek(cursor, 1) != '/' && peek(cursor, 2) != '/') {
				while (!find_forward(cursor, "///")) {
					cursor++;

				}
				cursor += 3;
				/* Cursor is at end of block,
					1. move to next character, (1)
					2. advance past block comment closing(3)
					3. then go 1 past to the end.(1)
					Total: 5
				*/
				return std::make_pair(TokenKind::block_comment, cursor);
			}
			else {
				cursor++;
				// if the next character is a '=' then we have a division assignment operator
				if (get(cursor) == '=') {
					cursor++;
					return std::make_pair(TokenKind::division_assignment, cursor);
				}
				// otherwise we have a division operator
				else
					return std::make_pair(TokenKind::division, cursor);
			}
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_apostrophe(char_iterator cursor) {
		if (get(cursor) == '\'') {
			cursor++;
			while (!(get(cursor) == '\'' && peek(cursor, -1) != '\\')) {
				cursor++;
			}
			cursor++;
			return std::make_pair(TokenKind::string_literal, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_newline(char_iterator cursor) {
		if (CharTraits::is_newline(get(cursor))) {
			while (CharTraits::is_newline(get(cursor))) {
				cursor++;
			}
			return std::make_pair(TokenKind::newline, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_whitespace(char_iterator cursor) {
		if (CharTraits::is_whitespace(get(cursor))) {
			while (CharTraits::is_whitespace(get(cursor))) {
				cursor++;
			}
			return std::make_pair(TokenKind::whitespace, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_eof(char_iterator cursor) {
		if (get(cursor) == '\0') {
			cursor++;
			return std::make_pair(TokenKind::eof, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_number(char_iterator cursor) {
		if (CharTraits::is_numeric(get(cursor))) {
			while (CharTraits::is_numeric(get(cursor))) {
				cursor++;
			}

			if (get(cursor) == '.') {
				cursor++;
				while (CharTraits::is_numeric(get(cursor))) {
					cursor++;
				}
				return std::make_pair(TokenKind::real_literal, cursor);
			}

			return std::make_pair(TokenKind::number_literal, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_alnumus(char_iterator cursor) {
		if (CharTraits::is_alpha(get(cursor))) {
			while (CharTraits::is_alnumus(get(cursor))) {
				cursor++;
			}
			return std::make_pair(TokenKind::alnumus, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_directive(char_iterator cursor) {
		if (get(cursor) == '#') {
			cursor++;
			while (CharTraits::is_alpha(get(cursor))) {
				cursor++;
			}
			return std::make_pair(TokenKind::directive, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_operator(char_iterator cursor) {
		if (get(cursor) == '=') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::equal, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::simple_assignment, cursor);
			}
		}
		else if (get(cursor) == '+') {
			if (peek(cursor, 1) == '+') {
				cursor += 2;
				return std::make_pair(TokenKind::increment, cursor);
			}
			else if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::addition_assignment, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::addition, cursor);
			}
		}
		else if (get(cursor) == '-') {
			if (peek(cursor, 1) == '-') {
				cursor += 2;
				return std::make_pair(TokenKind::decrement, cursor);
			}
			else if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::subtraction_assignment, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::subtraction, cursor);
			}
		}
		else if (get(cursor) == '*') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::multiplication_assignment, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::multiplication, cursor);
			}
		}
		else if (get(cursor) == '/') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::division_assignment, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::division, cursor);
			}
		}
		else if (get(cursor) == '%') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::remainder_assignment, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::remainder, cursor);
			}
		}
		else if (get(cursor) == '&') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::bitwise_and_assignment, cursor);
			}
			else if (peek(cursor, 1) == '&') {
				cursor += 2;
				return std::make_pair(TokenKind::logical_AND, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::bitwise_AND, cursor);
			}
		}
		else if (get(cursor) == '|') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::bitwise_or_assignment, cursor);
			}
			else if (peek(cursor, 1) == '|') {
				cursor += 2;
				return std::make_pair(TokenKind::logical_OR, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::bitwise_OR, cursor);
			}
		}
		else if (get(cursor) == '^') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::bitwise_xor_assignment, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::bitwise_XOR, cursor);
			}
		}
		else if (get(cursor) == '<') {
			if (peek(cursor, 1) == '<') {
				if (peek(cursor, 2) == '=') {
					cursor += 3;
					return std::make_pair(TokenKind::left_shift_assignment, cursor);
				}
				else {
					cursor += 2;
					return std::make_pair(TokenKind::bitwise_left_shift, cursor);
				}
			}
			else if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::less_than_or_equal, cursor);
			}
			else if (peek(cursor, 1) == '>') {
				cursor += 2;
				return std::make_pair(TokenKind::three_way_comparison, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::less_than, cursor);
			}
		}
		else if (get(cursor) == '>') {
			if (peek(cursor, 1) == '>') {
				if (peek(cursor, 2) == '=') {
					cursor += 3;
					return std::make_pair(TokenKind::right_shift_assignment, cursor);
				}
				else {
					cursor += 2;
					return std::make_pair(TokenKind::bitwise_right_shift, cursor);
				}
			}
			else if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::greater_than_or_equal, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::greater_than, cursor);
			}
		}
		else if (get(cursor) == '!') {
			if (peek(cursor, 1) == '=') {
				cursor += 2;
				return std::make_pair(TokenKind::not_equal, cursor);
			}
			else {
				cursor++;
				return std::make_pair(TokenKind::negation, cursor);
			}
		}
		else if (get(cursor) == '~') {
			cursor++;
			return std::make_pair(TokenKind::bitwise_NOT, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_scopes(char_iterator cursor) {
		if (get(cursor) == '(') {
			cursor++;
			return std::make_pair(TokenKind::open_scope, cursor);
		}
		else if (get(cursor) == ')') {
			cursor++;
			return std::make_pair(TokenKind::close_scope, cursor);
		}
		else if (get(cursor) == '{') {
			cursor++;
			return std::make_pair(TokenKind::open_list, cursor);
		}
		else if (get(cursor) == '}') {
			cursor++;
			return std::make_pair(TokenKind::close_list, cursor);
		}
		else if (get(cursor) == '[') {
			cursor++;
			return std::make_pair(TokenKind::open_frame, cursor);
		}
		else if (get(cursor) == ']') {
			cursor++;
			return std::make_pair(TokenKind::close_frame, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_eos(char_iterator cursor) {
		if (get(cursor) == ';') {
			cursor++;
			return std::make_pair(TokenKind::eos, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_comma(char_iterator cursor) {
		if (get(cursor) == ',') {
			cursor++;
			return std::make_pair(TokenKind::comma, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

	Tokenizer::lex_result Tokenizer::lex_candi_special(char_iterator cursor) {
		if (get(cursor) == '$') {
			cursor++;
			while (CharTraits::is_alpha(get(cursor))) {
				cursor++;
			}
			return std::make_pair(TokenKind::candi, cursor);
		}
		else {
			return std::make_pair(TokenKind::none, cursor);
		}
	}

}