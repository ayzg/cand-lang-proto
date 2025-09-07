#pragma once
#include <sstream>
#include "token.hpp"
#include "ast_node.hpp"

namespace caoco {
	namespace ca_error {
		namespace tokenizer {

			auto invalid_char = [](sl_size line, sl_size col, char8_t c,sl_string error="") {
				std::stringstream ss;
				ss << "\n[User Syntax Error]: ";
				ss << "\nInvalid character at line: " << line << " column: " << col << " \nerror detail: " << error;
				ss << "\n Offending character: " << static_cast<char>(c);
				return ss.str();
			};

			auto lexer_syntax_error = [](sl_size line, sl_size col, char8_t c, sl_string error="") {
				std::stringstream ss;
				ss << "\n[User Syntax Error]: ";
				ss << "\nLexer syntax error at line: " << line << " column: " << col 
					<< "\n Offending character: " << static_cast<char>(c) << "\nerror detail: " << error;
				return ss.str();
			};

			auto programmer_logic_error = [](sl_size line, sl_size col, char8_t c, sl_string error="") {
				std::stringstream ss;
				ss << "\n[Compiler programmer logic error]: ";
				ss << "\nFailed to tokenize at line: " << line << " column: " << col 
					<< "\n Offending character: " << static_cast<char>(c) << " \nerror detail: " << error;
				return ss.str();
			};
		}
		namespace parser {
			auto programmer_logic_error = [](astnode_enum attempted_astnode_type, tk_vector_cit error_location, sl_string error_message = "") {
				std::stringstream ss;
				ss << "\n[Compiler programmer logic error]: ";
				ss << "\nFailed to parse astnode of type " << astnode::type_to_string(attempted_astnode_type) << 
					" at token: " << tk::type_to_string(error_location->type()) 
					<< "\nline: " << error_location->line() 
					<< "\ncolumn: " << error_location->col() 
					<< "\nliteral: " << error_location->literal_str()
					<< "\nerror detail: " << error_message;
				return ss.str();
			};

			auto operation_missing_operand = [](astnode_enum attempted_operator_type,tk_vector_cit error_location, sl_string error_message = "") {
				std::stringstream ss;
				ss << "\n[User Syntax Error]: ";
				ss << "\nOperation missing operand at token: " << tk::type_to_string(error_location->type()) 
					<< "\noperator: " << astnode::type_to_string(attempted_operator_type)
					<< "\nline: " << error_location->line() 
					<< "\ncolumn: " << error_location->col() 
					<< "\nliteral: " << error_location->literal_str()
					<< "\nerror detail: " << error_message;
				return ss.str();
			};

			auto invalid_expression = [](tk_vector_cit error_location, sl_string error_message = "") {
				std::stringstream ss;
				ss << "\n[User Syntax Error]: ";
				ss << "\nInvalid expression at token: " << tk::type_to_string(error_location->type()) 
					<< "\nline: " << error_location->line() 
					<< "\ncolumn: " << error_location->col() 
					<< "\nliteral: " << error_location->literal_str()
					<< "\nerror detail: " << error_message;
				return ss.str();
			};
		}
	}
}
