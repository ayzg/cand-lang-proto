#include "pch.h"
#include <iostream>
#include <array>
#include <type_traits>
#include "import.hpp"
#include <list>
#include <any>

#include "tokenizer.hpp"
#include "parser.hpp"

// Testing Utilities

// Recursive print statement, prints the statement literal and all of its children recursively.
static void print_statement(cil::Statement& statement,int depth) {
	auto kind = statement.kind();
	auto statement_type = [kind]() {
		switch (kind) {
			case cil::StatementKind::invalid_
			: return "Invalid";
			case cil::StatementKind::none_
			: return "None";
			case cil::StatementKind::statement_
			: return "Statement";
			case cil::StatementKind::expression_
			: return "Expression";
			case cil::StatementKind::declaration_
			: return "Declaration";
			case cil::StatementKind::program_
			: return "Program";
			case cil::StatementKind::compiled_program_
			: return "Compiled Program";
			case cil::StatementKind::interpreted_program_
			: return "Interpreted Program";
			case cil::StatementKind::method_declaration_
			: return "Method Declaration";
			case cil::StatementKind::variable_declaration_
			: return "Variable Declaration";
			case cil::StatementKind::type_directive_
			: return "Type Directive";	
			case cil::StatementKind::type_expression_
			: return "Type Expression";
			case cil::StatementKind::identifier_expression_
			: return "Identifier Expression";
			case cil::StatementKind::value_expression_
			: return "Value Expression";
			case cil::StatementKind::variable_expression_
			: return "Variable Expression";
			case cil::StatementKind::literal_expression_
			: return "Literal Expression";
			case cil::StatementKind::literal_value_
			: return "Literal Value";
			case cil::StatementKind::integer_literal_
			: return "Integer Literal";
			case cil::StatementKind::real_literal_
			: return "Real Literal";
			case cil::StatementKind::string_literal_
			: return "String Literal";
			case cil::StatementKind::variable_definition_
			: return "Variable Definition";
			case cil::StatementKind::method_definition_
			: return "Method Definition";

			case cil::StatementKind::type_declaration_: return "Type Declaration";
			case cil::StatementKind::type_definition_: return "Type Definition";
			case cil::StatementKind::literal_alnumus_: return "Literal Alnumus";
			case cil::StatementKind::special_int_: return "Special Int";
			case cil::StatementKind::range_operation_: return "Range Operation";
			case cil::StatementKind::append_operation_: return "Append Operation";
			case cil::StatementKind::enumerate_operation_: return "Enumerate Operation";
			case cil::StatementKind::literal_number_: return "Literal Number" ;

			case cil::StatementKind::eof_
			: return "EOF";
			default:
			return "Unknown";
		}
	}();

	for(auto i = 0; i < depth;i++) {
		std::cout << "  >";
	}

	std::cout << statement_type << ":" << statement.to_string() << std::endl;

	if(statement.body.size() == 0) {
		return;
	}
	
	for (auto& child : statement.body) {
		print_statement(child,depth + 1);
	}
}
#define IMPORT_TEST false
#define PHASE1_TEST false
#define MINIMUM_PROGRAM_TEST false

#define PRINT_TOKENIZER_BASICCIL false
#if IMPORT_TEST
TEST(Tokenizer, BasicCil) {
	std::vector<char> source_file = cil::load_source_file("candi/basic.cil");
	//for (int i = 0; i < source.size(); i++) {
	//	source_file.push_back(source[i]);
	//}

	cil::Tokenizer tokenizer(source_file);
	auto tester = tokenizer.tokenize();

#if PRINT_TOKENIZER_BASICCIL
	for (auto& token : tester) {
		std::cout << "[" << static_cast<int>(token.kind) << "|" << token.lit() << "]";
	}
#endif
}
#endif
#define PRINT_TOKENIZER_MINIMUMPROGRAM false
#if PHASE1_TEST
TEST(Tokenizer, MinimumProgram) {
	std::vector<char> source_file = cil::load_source_file("candi/minimum_program.cil");
	//for (int i = 0; i < source.size(); i++) {
	//	source_file.push_back(source[i]);
	//}

	cil::Tokenizer tokenizer(source_file);
	auto tester = tokenizer.tokenize();

#if PRINT_TOKENIZER_MINIMUMPROGRAM
	for (auto& token : tester) {
		std::cout << "[" << static_cast<int>(token.kind) << "|" << token.lit() << "]";
	}
#endif
}
#endif
#if MINIMUM_PROGRAM_TEST
TEST(ProgramParser, MinimumProgram) {
	std::vector<char> source_file = cil::load_source_file("candi/minimum_program.cil");
	cil::Tokenizer tokenizer(source_file);
	auto tester = tokenizer.tokenize();
	auto sanitized = cil::TokenSanitizer::sanitize(tester);
	cil::ProgramParser parser(sanitized);

	auto result = parser.parse_tokens();
	
	print_statement(result);

}
#endif

TEST(ProgramParser, CSL_LangTools) {
	std::vector<char> source_file = cil::load_source_file("candi/csl_langtools.cil");
	cil::Tokenizer tokenizer(source_file);
	auto tester = tokenizer.tokenize();
	auto sanitized = cil::TokenSanitizer::sanitize(tester);
	cil::ProgramParser parser(sanitized);

	auto result = parser.parse_tokens();

	//std::cout << result.body.begin()->to_string();

	print_statement(result,0);

}


	int main(int argc, char** argv) {

		::testing::InitGoogleTest(&argc, argv);
		return RUN_ALL_TESTS();
	}
