#pragma once
#include <sstream>
#include "global_dependencies.hpp"
#include "cand_syntax.hpp"

namespace compiler_error {
	namespace tokenizer {

		auto invalid_char = [](sl_size line, sl_size col, char8_t c, sl_string error = "") {
			std::stringstream ss;
			ss << "\n[User Syntax Error]: ";
			ss << "\nInvalid character at line: " << line << " column: " << col << " \nerror detail: " << error;
			ss << "\n Offending character: " << static_cast<char>(c);
			return ss.str();
		};

		auto lexer_syntax_error = [](sl_size line, sl_size col, char8_t c, sl_string error = "") {
			std::stringstream ss;
			ss << "\n[User Syntax Error]: ";
			ss << "\nLexer syntax error at line: " << line << " column: " << col
				<< "\n Offending character: " << static_cast<char>(c) << "\nerror detail: " << error;
			return ss.str();
		};

		auto programmer_logic_error = [](sl_size line, sl_size col, char8_t c, sl_string error = "") {
			std::stringstream ss;
			ss << "\n[Compiler programmer logic error]: ";
			ss << "\nFailed to tokenize at line: " << line << " column: " << col
				<< "\n Offending character: " << static_cast<char>(c) << " \nerror detail: " << error;
			return ss.str();
		};
	}
	namespace parser {
		auto programmer_logic_error = [](e_ast attempted_astnode_type, tk_vector_cit error_location, sl_string error_message = "") {
			std::stringstream ss;
			ss << "\n[Compiler programmer logic error]: ";
			ss << "\nFailed to parse ast of type " << sl::to_str(attempted_astnode_type) <<
				" at token: " << sl::to_str(error_location->type())
				<< "\nline: " << error_location->line()
				<< "\ncolumn: " << error_location->col()
				<< "\nliteral: " << error_location->literal_str()
				<< "\nerror detail: " << error_message;
			return ss.str();
		};

		auto operation_missing_operand = [](e_ast attempted_operator_type, tk_vector_cit error_location, sl_string error_message = "") {
			std::stringstream ss;
			ss << "\n[User Syntax Error]: ";
			ss << "\nOperation missing operand at token: " << sl::to_str(error_location->type())
				<< "\noperator: " << sl::to_str(attempted_operator_type)
				<< "\nline: " << error_location->line()
				<< "\ncolumn: " << error_location->col()
				<< "\nliteral: " << error_location->literal_str()
				<< "\nerror detail: " << error_message;
			return ss.str();
		};

		auto invalid_expression = [](tk_vector_cit error_location, sl_string error_message = "") {
			std::stringstream ss;
			ss << "\n[User Syntax Error]: ";
			ss << "\nInvalid expression at token: " << sl::to_str(error_location->type())
				<< "\nline: " << error_location->line()
				<< "\ncolumn: " << error_location->col()
				<< "\nliteral: " << error_location->literal_str()
				<< "\nerror detail: " << error_message;
			return ss.str();
		};
	}
}