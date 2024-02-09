#include "pch.h"
#include "char_traits.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "ast_node.hpp"
#include "parser.hpp"
#include "constant_evaluator.hpp"

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Unit Testing Utilities
//----------------------------------------------------------------------------------------------------------------------------------------------------------//

// Google Test will not do check on caoco::sl_u8string, so we need to define the << operator for char8_t
std::ostream& operator<<(std::ostream& os, char8_t u8) {
	os << u8;
	return os;
}

// This is defined for printing direct u8 literals
std::ostream& operator<<(std::ostream& os, const char8_t* u8_cstr) {
	os << reinterpret_cast<const char*>(u8_cstr);
	return os;
}

caoco::sl_string token_type_to_string(caoco::tk_enum type) {
	switch (type) {
	case(caoco::tk_enum::none): return "none";
	case(caoco::tk_enum::invalid): return "invalid";
	case(caoco::tk_enum::eof): return "eof";
	case(caoco::tk_enum::line_comment): return "line_comment";
	case(caoco::tk_enum::block_comment): return "block_comment";
	case(caoco::tk_enum::string_literal): return "string_literal";
	case(caoco::tk_enum::number_literal): return "number_literal";
	case(caoco::tk_enum::real_literal): return "real_literal";
	case(caoco::tk_enum::newline): return "newline";
	case(caoco::tk_enum::whitespace): return "whitespace";
	case(caoco::tk_enum::alnumus): return "alnumus";
	case(caoco::tk_enum::simple_assignment): return "simple_assignment";
	case(caoco::tk_enum::addition_assignment): return "addition_assignment";
	case(caoco::tk_enum::subtraction_assignment): return "subtraction_assignment";
	case(caoco::tk_enum::multiplication_assignment): return "multiplication_assignment";
	case(caoco::tk_enum::division_assignment): return "division_assignment";
	case(caoco::tk_enum::remainder_assignment): return "remainder_assignment";
	case(caoco::tk_enum::bitwise_and_assignment): return "bitwise_and_assignment";
	case(caoco::tk_enum::bitwise_or_assignment): return "bitwise_or_assignment";
	case(caoco::tk_enum::bitwise_xor_assignment): return "bitwise_xor_assignment";
	case(caoco::tk_enum::left_shift_assignment): return "left_shift_assignment";
	case(caoco::tk_enum::right_shift_assignment): return "right_shift_assignment";
	case(caoco::tk_enum::increment): return "increment";
	case(caoco::tk_enum::decrement): return "decrement";
	case(caoco::tk_enum::addition): return "addition";
	case(caoco::tk_enum::subtraction): return "subtraction";
	case(caoco::tk_enum::multiplication): return "multiplication";
	case(caoco::tk_enum::division): return "division";
	case(caoco::tk_enum::remainder): return "remainder";
	case(caoco::tk_enum::bitwise_NOT): return "bitwise_NOT";
	case(caoco::tk_enum::bitwise_AND): return "bitwise_AND";
	case(caoco::tk_enum::bitwise_OR): return "bitwise_OR";
	case(caoco::tk_enum::bitwise_XOR): return "bitwise_XOR";
	case(caoco::tk_enum::bitwise_left_shift): return "bitwise_left_shift";
	case(caoco::tk_enum::bitwise_right_shift): return "bitwise_right_shift";
	case(caoco::tk_enum::negation): return "negation";
	case(caoco::tk_enum::logical_AND): return "logical_AND";
	case(caoco::tk_enum::logical_OR): return "logical_OR";
	case(caoco::tk_enum::equal): return "equal";
	case(caoco::tk_enum::not_equal): return "not_equal";
	case(caoco::tk_enum::less_than): return "less_than";
	case(caoco::tk_enum::greater_than): return "greater_than";
	case(caoco::tk_enum::less_than_or_equal): return "less_than_or_equal";
	case(caoco::tk_enum::greater_than_or_equal): return "greater_than_or_equal";
	case(caoco::tk_enum::three_way_comparison): return "three_way_comparison";
	case(caoco::tk_enum::open_scope): return "open_scope";
	case(caoco::tk_enum::close_scope): return "close_scope";
	case(caoco::tk_enum::open_list): return "open_list";
	case(caoco::tk_enum::close_list): return "close_list";
	case(caoco::tk_enum::open_frame): return "open_frame";
	case(caoco::tk_enum::close_frame): return "close_frame";
	case(caoco::tk_enum::eos): return "eos";
	case(caoco::tk_enum::comma): return "comma";
	case(caoco::tk_enum::period): return "period";
	case(caoco::tk_enum::ellipsis): return "ellipsis";
	case(caoco::tk_enum::atype_): return "atype_";
	case(caoco::tk_enum::aidentity_): return "aidentity_";
	case(caoco::tk_enum::avalue_): return "avalue_";
	case(caoco::tk_enum::aint_): return "aint_";
	case(caoco::tk_enum::auint_): return "auint_";
	case(caoco::tk_enum::areal_): return "areal_";
		//case(caoco::tk_enum::aureal_): return "aureal_";
	case(caoco::tk_enum::aoctet_): return "aoctet_";
	case(caoco::tk_enum::abit_): return "abit_";
	case(caoco::tk_enum::aarray_): return "aarray_";
	case(caoco::tk_enum::apointer_): return "apointer_";
	case(caoco::tk_enum::amemory_): return "amemory_";
	case(caoco::tk_enum::afunction_): return "afunction_";
	case(caoco::tk_enum::enter_): return "enter_";
	case(caoco::tk_enum::start_): return "start_";
	case(caoco::tk_enum::type_): return "type_";
	case(caoco::tk_enum::var_): return "var_";
	case(caoco::tk_enum::class_): return "class_";
	case(caoco::tk_enum::func_): return "func_";
	case(caoco::tk_enum::print_): return "print_";
	case(caoco::tk_enum::public_): return "public_";
	case(caoco::tk_enum::const_): return "const_";
	case(caoco::tk_enum::static_): return "static_";
	case(caoco::tk_enum::ref_): return "ref_";
	case(caoco::tk_enum::if_): return "if_";
	case(caoco::tk_enum::else_): return "else_";
	case(caoco::tk_enum::elif_): return "elif_";
	case(caoco::tk_enum::while_): return "while_";
	case(caoco::tk_enum::for_): return "for_";
	case(caoco::tk_enum::switch_): return "switch_";
	case(caoco::tk_enum::case_): return "case_";
	case(caoco::tk_enum::default_): return "default_";
	case(caoco::tk_enum::break_): return "break_";
	case(caoco::tk_enum::continue_): return "continue_";
	case(caoco::tk_enum::return_): return "return_";
	case(caoco::tk_enum::into_): return "into_";
	case(caoco::tk_enum::none_literal_): return "none_literal_";
	case(caoco::tk_enum::bit_literal): return "bit_literal";
	case(caoco::tk_enum::octet_literal): return "octet_literal";
	case(caoco::tk_enum::unsigned_literal): return "unsigned_literal";
	default: return "This token type is not string-convertible. Please implement a string conversion for this token type in the token_type_to_string function in test.cpp.";
	}
}

caoco::sl_string token_to_string(const caoco::tk& token) {
	return token_type_to_string(token.type()) + caoco::sl_string(" : ") + caoco::sl::to_str(token.literal());
}

void test_single_token(const char8_t* input, caoco::tk_enum expected_type, caoco::sl_u8string expected_literal) {
	auto input_vec = caoco::sl::to_u8vec(input);
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result.at(0).type(), expected_type);
	EXPECT_EQ(result.at(0).literal(), expected_literal);
	std::cout << "[Single token Test][" << input << "]" << token_to_string(result.at(0)) << std::endl;
}

auto node_debug_string(const caoco::astnode& node) {
	std::string debug_string = "[";
	switch (node.type()) {
	case caoco::astnode_enum::none_: debug_string += "none_"; break;
	case caoco::astnode_enum::invalid_: debug_string += "invalid_"; break;
	case caoco::astnode_enum::string_literal_: debug_string += "string_literal_"; break;
	case caoco::astnode_enum::number_literal_: debug_string += "number_literal_"; break;
	case caoco::astnode_enum::real_literal_: debug_string += "real_literal_"; break;
	case caoco::astnode_enum::alnumus_: debug_string += "alnumus_"; break;
	case caoco::astnode_enum::simple_assignment_: debug_string += "simple_assignment_"; break;
	case caoco::astnode_enum::addition_assignment_: debug_string += "addition_assignment_"; break;
	case caoco::astnode_enum::subtraction_assignment_: debug_string += "subtraction_assignment_"; break;
	case caoco::astnode_enum::multiplication_assignment_: debug_string += "multiplication_assignment_"; break;
	case caoco::astnode_enum::division_assignment_: debug_string += "division_assignment_"; break;
	case caoco::astnode_enum::remainder_assignment_: debug_string += "remainder_assignment_"; break;
	case caoco::astnode_enum::bitwise_and_assignment_: debug_string += "bitwise_and_assignment_"; break;
	case caoco::astnode_enum::bitwise_or_assignment_: debug_string += "bitwise_or_assignment_"; break;
	case caoco::astnode_enum::bitwise_xor_assignment_: debug_string += "bitwise_xor_assignment_"; break;
	case caoco::astnode_enum::left_shift_assignment_: debug_string += "left_shift_assignment_"; break;
	case caoco::astnode_enum::right_shift_assignment_: debug_string += "right_shift_assignment_"; break;
	case caoco::astnode_enum::increment_: debug_string += "increment_"; break;
	case caoco::astnode_enum::decrement_: debug_string += "decrement_"; break;
	case caoco::astnode_enum::addition_: debug_string += "addition_"; break;
	case caoco::astnode_enum::subtraction_: debug_string += "subtraction_"; break;
	case caoco::astnode_enum::multiplication_: debug_string += "multiplication_"; break;
	case caoco::astnode_enum::division_: debug_string += "division_"; break;
	case caoco::astnode_enum::remainder_: debug_string += "remainder_"; break;
	case caoco::astnode_enum::bitwise_NOT_: debug_string += "bitwise_NOT_"; break;
	case caoco::astnode_enum::bitwise_AND_: debug_string += "bitwise_AND_"; break;
	case caoco::astnode_enum::bitwise_OR_: debug_string += "bitwise_OR_"; break;
	case caoco::astnode_enum::bitwise_XOR_: debug_string += "bitwise_XOR_"; break;
	case caoco::astnode_enum::bitwise_left_shift_: debug_string += "bitwise_left_shift_"; break;
	case caoco::astnode_enum::bitwise_right_shift_: debug_string += "bitwise_right_shift_"; break;
	case caoco::astnode_enum::negation_: debug_string += "negation_"; break;
	case caoco::astnode_enum::logical_AND_: debug_string += "logical_AND_"; break;
	case caoco::astnode_enum::logical_OR_: debug_string += "logical_OR_"; break;
	case caoco::astnode_enum::equal_: debug_string += "equal_"; break;
	case caoco::astnode_enum::not_equal_: debug_string += "not_equal_"; break;
	case caoco::astnode_enum::less_than_: debug_string += "less_than_"; break;
	case caoco::astnode_enum::greater_than_: debug_string += "greater_than_"; break;
	case caoco::astnode_enum::less_than_or_equal_: debug_string += "less_than_or_equal_"; break;
	case caoco::astnode_enum::greater_than_or_equal_: debug_string += "greater_than_or_equal_"; break;
	case caoco::astnode_enum::three_way_comparison_: debug_string += "three_way_comparison_"; break;
	case caoco::astnode_enum::atype_: debug_string += "atype_"; break;
	case caoco::astnode_enum::aidentity_: debug_string += "aidentity_"; break;
	case caoco::astnode_enum::avalue_: debug_string += "avalue_"; break;
	case caoco::astnode_enum::aint_: debug_string += "aint_"; break;
	case caoco::astnode_enum::auint_: debug_string += "auint_"; break;
	case caoco::astnode_enum::areal_: debug_string += "areal_"; break;
		//case caoco::astnode_enum::aureal_: debug_string += "aureal_"; break;
	case caoco::astnode_enum::aoctet_: debug_string += "aoctet_"; break;
	case caoco::astnode_enum::abit_: debug_string += "abit_"; break;
	case caoco::astnode_enum::aarray_: debug_string += "aarray_"; break;
	case caoco::astnode_enum::apointer_: debug_string += "apointer_"; break;
	case caoco::astnode_enum::amemory_: debug_string += "amemory_"; break;
	case caoco::astnode_enum::afunction_: debug_string += "afunction_"; break;
	case caoco::astnode_enum::enter_: debug_string += "enter_"; break;
	case caoco::astnode_enum::start_: debug_string += "start_"; break;
	case caoco::astnode_enum::type_: debug_string += "type_"; break;
	case caoco::astnode_enum::var_: debug_string += "var_"; break;
	case caoco::astnode_enum::class_: debug_string += "class_"; break;
	case caoco::astnode_enum::func_: debug_string += "func_"; break;
	case caoco::astnode_enum::const_: debug_string += "const_"; break;
	case caoco::astnode_enum::static_: debug_string += "static_"; break;
	case caoco::astnode_enum::ref_: debug_string += "ref_"; break;
	case caoco::astnode_enum::if_: debug_string += "if_"; break;
	case caoco::astnode_enum::else_: debug_string += "else_"; break;
	case caoco::astnode_enum::elif_: debug_string += "elif_"; break;
	case caoco::astnode_enum::while_: debug_string += "while_"; break;
	case caoco::astnode_enum::for_: debug_string += "for_"; break;
	case caoco::astnode_enum::switch_: debug_string += "switch_"; break;
	case caoco::astnode_enum::case_: debug_string += "case_"; break;
	case caoco::astnode_enum::default_: debug_string += "default_"; break;
	case caoco::astnode_enum::break_: debug_string += "break_"; break;
	case caoco::astnode_enum::continue_: debug_string += "continue_"; break;
	case caoco::astnode_enum::return_: debug_string += "return_"; break;
	case caoco::astnode_enum::into_: debug_string += "into_"; break;
	case caoco::astnode_enum::statement_: debug_string += "statement_"; break;
	case caoco::astnode_enum::expression_: debug_string += "expression_"; break;
	case caoco::astnode_enum::declaration_: debug_string += "declaration_"; break;
	case caoco::astnode_enum::program_: debug_string += "program_"; break;
	case caoco::astnode_enum::compiled_program_: debug_string += "compiled_program_"; break;
	case caoco::astnode_enum::interpreted_program_: debug_string += "interpreted_program_"; break;
	case caoco::astnode_enum::pragmatic_block_: debug_string += "pragmatic_block_"; break;
	case caoco::astnode_enum::functional_block_: debug_string += "functional_block_"; break;
	case caoco::astnode_enum::identifier_statement_: debug_string += "identifier_statement_"; break;
	case caoco::astnode_enum::variable_assignment_: debug_string += "variable_assignment_"; break;
	case caoco::astnode_enum::type_definition_: debug_string += "type_definition_"; break;
	case caoco::astnode_enum::anon_variable_definition_: debug_string += "anon_variable_definition_"; break;
	case caoco::astnode_enum::anon_variable_definition_assingment_: debug_string += "anon_variable_definition_assingment_"; break;
	case caoco::astnode_enum::constrained_variable_definition_: debug_string += "constrained_variable_definition_"; break;
	case caoco::astnode_enum::type_constraints_: debug_string += "type_constraints_"; break;
	case caoco::astnode_enum::class_definition_: debug_string += "class_definition_"; break;
	case caoco::astnode_enum::shorthand_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
	case caoco::astnode_enum::shorthand_constrained_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
	case caoco::astnode_enum::arguments_: debug_string += "arguments_"; break;
	case caoco::astnode_enum::function_call_: debug_string += "function_call_"; break;
	case caoco::astnode_enum::method_definition_: debug_string += "method_definition_"; break;
	case caoco::astnode_enum::eof_: debug_string += "eof_"; break;
	case caoco::astnode_enum::period_: debug_string += "period_"; break;

	}

	debug_string += "] ";
	debug_string += node.literal_str();
	debug_string += "| ";
	return debug_string;
}

void print_ast(const caoco::astnode& node, int depth = 0) {
	for (int i = 0; i < depth; i++)
		std::cout << "  ";
	std::cout << node_debug_string(node) << std::endl;
	for (auto& child : node.body())
		print_ast(child, depth + 1);
}

// Test a parsing functor given a subset of tokens. Prints the test_name followed by the AST.
template<typename ParsingFunctorT> requires std::is_base_of_v<caoco::parsing_process, ParsingFunctorT>
caoco::tk_vector_cit test_parsing_functor(caoco::sl_string test_name, ParsingFunctorT&& parsing_functor, caoco::tk_vector_cit begin, caoco::tk_vector_cit end) {
	// Empty Class Definition
	std::cout << "[Testing Parsing Method][Test Case:" << test_name << "]" << std::endl;
	auto parse_result = ParsingFunctorT()(begin, end);
	EXPECT_TRUE(parse_result.valid());
	if (!parse_result.valid()) {
		std::cout << parse_result.error_message() << std::endl;
	}
	print_ast(parse_result.node()); // Print the AST for debugging purposes
	return parse_result.it(); // The correct value is 1 past the end of the parsed tokens, so the next parsing method can start there.
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Tokenizer Tests
// dependencies: tokenizer.hpp
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
#define caoco_CaocoTokenizer_Tokens 1
#define caoco_CaocoTokenizer_NumberAndReal 1

#if caoco_CaocoTokenizer_Tokens
TEST(CaocoTokenizer_Tokens, CaocoTokenizer_Test) {
	// NOTE: the sanitized output will not contain comments/newlines/whitespace. As such there is a seperate test for those tokens.

	// Literals
	// number
	test_single_token(u8"1234\0", caoco::tk_enum::number_literal, u8"1234");
	// real
	test_single_token(u8"1234.5678\0", caoco::tk_enum::real_literal, u8"1234.5678");
	// string 
	test_single_token(u8"'hello world'\0", caoco::tk_enum::string_literal, u8"'hello world'");
	// string with escape character '\''
	test_single_token(u8"'hello \\' world'\0", caoco::tk_enum::string_literal, u8"'hello \\' world'");
	// alnumus
	test_single_token(u8"hello_world\0", caoco::tk_enum::alnumus, u8"hello_world");
	// unsigned literal u
	test_single_token(u8"1234u\0", caoco::tk_enum::unsigned_literal, u8"1234u");
	// bit literal 0b 1b
	test_single_token(u8"0b\0", caoco::tk_enum::bit_literal, u8"0b");
	test_single_token(u8"1b\0", caoco::tk_enum::bit_literal, u8"1b");
	// octet literal 0c 255c
	test_single_token(u8"0c\0", caoco::tk_enum::octet_literal, u8"0c");
	test_single_token(u8"255c\0", caoco::tk_enum::octet_literal, u8"255c");

	// octet literal 'a'c
	test_single_token(u8"'a'c\0", caoco::tk_enum::octet_literal, u8"'a'c");
	// octet literal '\''c
	test_single_token(u8"'\\''c\0", caoco::tk_enum::octet_literal, u8"'\\''c");

	// Directives
	// Functional
	// #enter,#start,#type,#var,#class,#print,#func
	test_single_token(u8"#enter\0", caoco::tk_enum::enter_, u8"#enter"); 
	test_single_token(u8"#start\0", caoco::tk_enum::start_, u8"#start");
	test_single_token(u8"#type\0", caoco::tk_enum::type_, u8"#type");
	test_single_token(u8"#var\0", caoco::tk_enum::var_, u8"#var");
	test_single_token(u8"#class\0", caoco::tk_enum::class_, u8"#class");
	test_single_token(u8"#print\0", caoco::tk_enum::print_, u8"#print");
	test_single_token(u8"#func\0", caoco::tk_enum::func_, u8"#func");
	test_single_token(u8"#none\0", caoco::tk_enum::none_literal_, u8"#none"); 

	// Modifiers
	// #public,#const,#static,#ref,
	test_single_token(u8"#public\0", caoco::tk_enum::public_, u8"#public");
	test_single_token(u8"#const\0", caoco::tk_enum::const_, u8"#const");
	test_single_token(u8"#static\0", caoco::tk_enum::static_, u8"#static");
	test_single_token(u8"#ref\0", caoco::tk_enum::ref_, u8"#ref");

	// Control Flow
	// #if,#else,#elif,#while,#for,#switch,#case,#default,#break,#continue,#return,#into
	test_single_token(u8"#if\0", caoco::tk_enum::if_, u8"#if");
	test_single_token(u8"#else\0", caoco::tk_enum::else_, u8"#else");
	test_single_token(u8"#elif\0", caoco::tk_enum::elif_, u8"#elif");
	test_single_token(u8"#while\0", caoco::tk_enum::while_, u8"#while");
	test_single_token(u8"#for\0", caoco::tk_enum::for_, u8"#for");
	test_single_token(u8"#switch\0", caoco::tk_enum::switch_, u8"#switch");
	test_single_token(u8"#case\0", caoco::tk_enum::case_, u8"#case");
	test_single_token(u8"#default\0", caoco::tk_enum::default_, u8"#default");
	test_single_token(u8"#break\0", caoco::tk_enum::break_, u8"#break");
	test_single_token(u8"#continue\0", caoco::tk_enum::continue_, u8"#continue");
	test_single_token(u8"#return\0", caoco::tk_enum::return_, u8"#return");
	test_single_token(u8"#into\0", caoco::tk_enum::into_, u8"#into");

	// Assignment Operators
	// =,+=,-=,*=,/=,%=,&=,|=,^=,<<=,>>=
	test_single_token(u8"=\0", caoco::tk_enum::simple_assignment, u8"=");
	test_single_token(u8"+=\0", caoco::tk_enum::addition_assignment, u8"+=");
	test_single_token(u8"-=\0", caoco::tk_enum::subtraction_assignment, u8"-=");
	test_single_token(u8"*=\0", caoco::tk_enum::multiplication_assignment, u8"*=");
	test_single_token(u8"/=\0", caoco::tk_enum::division_assignment, u8"/=");
	test_single_token(u8"%=\0", caoco::tk_enum::remainder_assignment, u8"%=");
	test_single_token(u8"&=\0", caoco::tk_enum::bitwise_and_assignment, u8"&=");
	test_single_token(u8"|=\0", caoco::tk_enum::bitwise_or_assignment, u8"|=");
	test_single_token(u8"^=\0", caoco::tk_enum::bitwise_xor_assignment, u8"^=");
	test_single_token(u8"<<=\0", caoco::tk_enum::left_shift_assignment, u8"<<=");
	test_single_token(u8">>=\0", caoco::tk_enum::right_shift_assignment, u8">>=");

	// Increment and Decrement Operators
	// ++,--
	test_single_token(u8"++\0", caoco::tk_enum::increment, u8"++");
	test_single_token(u8"--\0", caoco::tk_enum::decrement, u8"--");

	//// Arithmetic Operators
	//// +,-,*,/,%,~,&,|,^,<<,>>
	test_single_token(u8"+\0", caoco::tk_enum::addition, u8"+");
	test_single_token(u8"-\0", caoco::tk_enum::subtraction, u8"-");
	test_single_token(u8"*\0", caoco::tk_enum::multiplication, u8"*");
	test_single_token(u8"/\0", caoco::tk_enum::division, u8"/");
	test_single_token(u8"%\0", caoco::tk_enum::remainder, u8"%");
	test_single_token(u8"~\0", caoco::tk_enum::bitwise_NOT, u8"~");
	test_single_token(u8"&\0", caoco::tk_enum::bitwise_AND, u8"&");
	test_single_token(u8"|\0", caoco::tk_enum::bitwise_OR, u8"|");
	test_single_token(u8"^\0", caoco::tk_enum::bitwise_XOR, u8"^");
	test_single_token(u8"<<\0", caoco::tk_enum::bitwise_left_shift, u8"<<");
	test_single_token(u8">>\0", caoco::tk_enum::bitwise_right_shift, u8">>");

	//// Logical Operators
	//// !,&&,||
	test_single_token(u8"!\0", caoco::tk_enum::negation, u8"!");
	test_single_token(u8"&&\0", caoco::tk_enum::logical_AND, u8"&&");
	test_single_token(u8"||\0", caoco::tk_enum::logical_OR, u8"||");

	//// Comparison Operators
	//// ==,!=,<,>,<=,>=,<=>
	test_single_token(u8"==\0", caoco::tk_enum::equal, u8"==");
	test_single_token(u8"!=\0", caoco::tk_enum::not_equal, u8"!=");
	test_single_token(u8"<\0", caoco::tk_enum::less_than, u8"<");
	test_single_token(u8">\0", caoco::tk_enum::greater_than, u8">");
	test_single_token(u8"<=\0", caoco::tk_enum::less_than_or_equal, u8"<=");
	test_single_token(u8">=\0", caoco::tk_enum::greater_than_or_equal, u8">=");
	test_single_token(u8"<=>\0", caoco::tk_enum::three_way_comparison, u8"<=>"); 

	//// Scopes
	//// (,),{,},[,]
	test_single_token(u8"(\0", caoco::tk_enum::open_scope, u8"(");
	test_single_token(u8")\0", caoco::tk_enum::close_scope, u8")");
	test_single_token(u8"{\0", caoco::tk_enum::open_list, u8"{");
	test_single_token(u8"}\0", caoco::tk_enum::close_list, u8"}");
	test_single_token(u8"[\0", caoco::tk_enum::open_frame, u8"[");
	test_single_token(u8"]\0", caoco::tk_enum::close_frame, u8"]");

	//// Special Tokens
	//// ;,:,...
	test_single_token(u8";\0", caoco::tk_enum::eos, u8";");
	test_single_token(u8",\0", caoco::tk_enum::comma, u8",");
	test_single_token(u8".\0", caoco::tk_enum::period, u8".");
	test_single_token(u8"...\0", caoco::tk_enum::ellipsis, u8"...");

	// Cand Special Objects
	// &type, &identity, &value, &int, &uint, &real, &octet, &bit, &array, &pointer, &memory, &function
	test_single_token(u8"&type\0", caoco::tk_enum::atype_, u8"&type");
	test_single_token(u8"&identity\0", caoco::tk_enum::aidentity_, u8"&identity");
	test_single_token(u8"&value\0", caoco::tk_enum::avalue_, u8"&value");
	test_single_token(u8"&int\0", caoco::tk_enum::aint_, u8"&int");
	test_single_token(u8"&uint\0", caoco::tk_enum::auint_, u8"&uint");
	test_single_token(u8"&real\0", caoco::tk_enum::areal_, u8"&real");
	//test_single_token(u8"&ureal\0", caoco::tk_enum::aureal_, u8"&ureal");
	test_single_token(u8"&octet\0", caoco::tk_enum::aoctet_, u8"&octet");
	test_single_token(u8"&bit\0", caoco::tk_enum::abit_, u8"&bit");
	test_single_token(u8"&array\0", caoco::tk_enum::aarray_, u8"&array");
	test_single_token(u8"&pointer\0", caoco::tk_enum::apointer_, u8"&pointer");
	test_single_token(u8"&memory\0", caoco::tk_enum::amemory_, u8"&memory");
	test_single_token(u8"&function\0", caoco::tk_enum::afunction_, u8"&function");

};
#endif
#if caoco_CaocoTokenizer_NumberAndReal
TEST(CaocoTokenizer_NumberAndReal, CaocoTokenizer_Test) {
	// Specific test for a number followed by an ellipsis

	auto input_vec = caoco::sl::to_u8vec(u8"1234...\0");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result.at(0).type(), caoco::tk_enum::number_literal);
	EXPECT_EQ(result.at(0).literal(), caoco::sl_u8string(u8"1234"));
	EXPECT_EQ(result.at(1).type(), caoco::tk_enum::ellipsis);
	EXPECT_EQ(result.at(1).literal(), caoco::sl_u8string(u8"..."));

}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Parser Tests
// dependencies: parser.hpp
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
#define caocotest_CaocoParser_BasicNode_SingularNodes 1
#define caocotest_CaocoParser_BasicNode_BasicScopes 1
#define caocotest_CaocoParser_BasicNode_StatementScope 1
#define caocotest_CaocoParser_BasicNode_PrimaryExpression 1
#define caocotest_CaocoParser_BasicNode_SimpleStatements 1
#define caocotest_CaocoParser_BasicNode_Functions 1
#define caocotest_CaocoParser_BasicNode_Classes 1
#define caocotest_CaocoParser_MinimumProgram 1

#if caocotest_CaocoParser_BasicNode_SingularNodes
TEST(CaocoParser_BasicNode_SingularNodes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_basic_node.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto atype_end = test_parsing_functor("&type:", caoco::ParseCsoType(), result.cbegin(), result.cend());
	auto avalue_end = test_parsing_functor("&value:", caoco::ParseCsoValue(), atype_end, result.cend());
	auto aidentity_end = test_parsing_functor("&identity:", caoco::ParseCsoIdentity(), avalue_end, result.cend());
	auto aint_end = test_parsing_functor("&int:", caoco::ParseCsoInt(), aidentity_end, result.cend());
	auto aint_positive_range_end = test_parsing_functor("&int[positive_range]:", caoco::ParseCsoInt(), aint_end, result.cend());
	auto aint_negative_range_end = test_parsing_functor("&int[negative_range]:", caoco::ParseCsoInt(), aint_positive_range_end, result.cend());
	auto aint_lhs_negative_range_end = test_parsing_functor("&int[lhs_negative_range]:", caoco::ParseCsoInt(), aint_negative_range_end, result.cend());
	auto aint_rhs_negative_range_end = test_parsing_functor("&int[rhs_negative_range]:", caoco::ParseCsoInt(), aint_lhs_negative_range_end, result.cend());
	auto auint_end = test_parsing_functor("&uint:", caoco::ParseCsoUint(), aint_rhs_negative_range_end, result.cend());
	auto auint_positive_range_end = test_parsing_functor("&uint[positive_range]:", caoco::ParseCsoUint(), auint_end, result.cend());
	auto areal_end = test_parsing_functor("&real:", caoco::ParseCsoReal(), auint_positive_range_end, result.cend());
	auto aoctet_end = test_parsing_functor("&octet:", caoco::ParseCsoOctet(), areal_end, result.cend());
	auto abit_end = test_parsing_functor("&bit:", caoco::ParseCsoBit(), aoctet_end, result.cend());
	auto apointer_end = test_parsing_functor("&pointer[mytype]:", caoco::ParseCsoPointer(), abit_end, result.cend());	
	auto apointer_int_end = test_parsing_functor("&pointer[&int]:", caoco::ParseCsoPointer(), apointer_end, result.cend());
	auto aarray_end = test_parsing_functor("&array[mytype, 42]:", caoco::ParseCsoArray(), apointer_int_end, result.cend());
	auto aarray_int_end = test_parsing_functor("&array[&int, 42]:", caoco::ParseCsoArray(), aarray_end, result.cend());
	auto aarray_int_range_end = test_parsing_functor("&array[&int[1 ...42], 42]:", caoco::ParseCsoArray(), aarray_int_end, result.cend());
	auto string_literal_end = test_parsing_functor("'string literal':", caoco::ParseStringLiteral(), aarray_int_range_end, result.cend());
	auto string_literal_with_escape_end = test_parsing_functor("'string literal with \\'':", caoco::ParseStringLiteral(), string_literal_end, result.cend());
	auto number_literal_end = test_parsing_functor("number literal:", caoco::ParseNumberLiteral(), string_literal_with_escape_end, result.cend());
	auto real_literal_end = test_parsing_functor("real literal:", caoco::ParseRealLiteral(), number_literal_end, result.cend());
	auto alnumus_end = test_parsing_functor("alnumus:", caoco::ParseAlnumusLiteral(), real_literal_end, result.cend());
}
#endif
#if caocotest_CaocoParser_BasicNode_BasicScopes	
TEST(CaocoParser_BasicNode_BasicScopes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_scopes.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// empty scope
	std::cout << "Testing empty scope:" << std::endl;
	caoco::parser_scope_result empty_scope = caoco::find_scope(result.cbegin(), result.cend());
	EXPECT_TRUE(empty_scope.valid);


	// scope with 1 element
	std::cout << "Testing scope with 1 element:" << std::endl;
	caoco::parser_scope_result scope_with_1_element = caoco::find_scope(empty_scope.scope_end(), result.cend());
	EXPECT_TRUE(scope_with_1_element.valid);

	// double scope
	std::cout << "Testing double scope:" << std::endl;
	caoco::parser_scope_result double_scope = caoco::find_scope(scope_with_1_element.scope_end(), result.cend());
	EXPECT_TRUE(double_scope.valid);

	// complex scope
	std::cout << "Testing complex scope:" << std::endl;
	caoco::parser_scope_result complex_scope = caoco::find_scope(double_scope.scope_end(), result.cend());
	EXPECT_TRUE(complex_scope.valid);


	// invalid scope should be invalid	
	std::cout << "Testing invalid scope:" << std::endl;
	caoco::parser_scope_result invalid_scope = caoco::find_scope(complex_scope.scope_end(), result.cend());
	EXPECT_FALSE(invalid_scope.valid);

}
#endif
#if caocotest_CaocoParser_BasicNode_StatementScope 
TEST(CaocoParser_BasicNode_StatementScope, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_statements.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Single value statement : 1;
	std::cout << "Testing single value statement" << std::endl;
	caoco::parser_scope_result empty_statement = caoco::find_statement(caoco::tk_enum::number_literal,caoco::tk_enum::eos,result.cbegin(), result.cend());
	EXPECT_TRUE(empty_statement.valid);

	// statement with multiple tokens: #var a = 1;
	std::cout << "Testing statement with multiple tokens" << std::endl;
	caoco::parser_scope_result multiple_token_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos, empty_statement.scope_end(), result.cend());
	EXPECT_TRUE(multiple_token_statement.valid);

	// statement with multiple tokens and scopes: #var a = (1;2;3);
	std::cout << "Testing statement with multiple tokens and scopes" << std::endl;
	caoco::parser_scope_result multiple_token_scope_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos, multiple_token_statement.scope_end(), result.cend());
	EXPECT_TRUE(multiple_token_scope_statement.valid);

	// statement with lists frames and scopes nested in diffrent ways containing end tokens. #var a = 1 + ([ 2 ;3 + {4;5;6}]);
	std::cout << "Testing statement with lists frames and scopes nested in diffrent ways containing end tokens." << std::endl;
	caoco::parser_scope_result complex_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos, multiple_token_scope_statement.scope_end(), result.cend());
	EXPECT_TRUE(complex_statement.valid);
	//EXPECT_TRUE(complex_statement.scope_end() == result.cend() - 1);

	// Test finding an "open" statement which allows for repeated open tokens. ex a = a + a + ([ a ;a + {a;a;a}]);
	std::cout << "Testing statement with lists frames and scopes nested in diffrent ways containing begin and end tokens." << std::endl;
	caoco::parser_scope_result open_statement = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, complex_statement.scope_end(), result.cend());
	EXPECT_TRUE(open_statement.valid);
	EXPECT_TRUE(open_statement.scope_end() == result.cend() - 1);

}
#endif
#if caocotest_CaocoParser_BasicNode_PrimaryExpression
TEST(CaocoParser_BasicNode_PrimaryExpression, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_primary_expr.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto found_scope = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, result.cbegin(), result.cend());
	// Test parsing a primary expression using build statement method
	std::cout << "Testing foo;" << std::endl;
	auto px1 = caoco::build_statement(found_scope.scope_begin(), found_scope.contained_end());
	EXPECT_TRUE(px1.has_value());
	print_ast(px1.value());


	// foo + 2
	std::cout << "Testing foo + 2;" << std::endl;
	auto found_scope2 = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, found_scope.scope_end(), result.cend());
	auto px2 = caoco::build_statement(found_scope2.scope_begin(), found_scope2.contained_end());
	EXPECT_TRUE(px2.has_value());
	print_ast(px2.value());

	// foo = 1 + 2;
	std::cout << "Testing foo = 1 + 2;" << std::endl;
	auto found_scope3 = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, found_scope2.scope_end(), result.cend());
	auto px3 = caoco::build_statement(found_scope3.scope_begin(), found_scope3.contained_end());
	EXPECT_TRUE(px3.has_value());
	print_ast(px3.value());

	// Testing period(member access) operator
	std::cout << "Testing foo.bar;" << std::endl;
	auto found_scope4 = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, found_scope3.scope_end(), result.cend());
	auto px4 = caoco::build_statement(found_scope4.scope_begin(), found_scope4.contained_end());
	EXPECT_TRUE(px4.has_value());
	print_ast(px4.value());

	// Testing function call operator ()
	std::cout << "Testing foo.bar();" << std::endl;
	auto found_scope5 = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, found_scope4.scope_end(), result.cend());
	auto px5 = caoco::build_statement(found_scope5.scope_begin(), found_scope5.contained_end());
	EXPECT_TRUE(px5.has_value());
	print_ast(px5.value());

	// Statement with no following binary operator should be invalid and throw an exception.
	std::cout << "Testing foo=; Result should throw." << std::endl;
	auto found_scope6 = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, found_scope5.scope_end(), result.cend());
	EXPECT_ANY_THROW(caoco::build_statement(found_scope6.scope_begin(), found_scope6.contained_end()));

}
#endif
#if caocotest_CaocoParser_BasicNode_SimpleStatements
TEST(CaocoParser_BasicNode_SimpleStatements, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_basic_statement.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Type definition
	// #type Int = &int;
	std::cout << "Testing #type Int = &int;" << std::endl;
	auto found_scope = caoco::find_open_statement(caoco::tk_enum::type_, caoco::tk_enum::eos, result.cbegin(), result.cend());
	auto px1 = caoco::ParseDirectiveType()(found_scope.scope_begin(), found_scope.scope_end());
	EXPECT_TRUE(px1.valid());
	print_ast(px1.node());


	// Type alias
	// #type IntAlias = Int;
	std::cout << "Testing #type IntAlias = Int;" << std::endl;
	auto found_scope2 = caoco::find_open_statement(caoco::tk_enum::type_, caoco::tk_enum::eos, found_scope.scope_end(), result.cend());
	auto px2 = caoco::ParseDirectiveType()(found_scope2.scope_begin(), found_scope2.scope_end());
	EXPECT_TRUE(px2.valid());
	print_ast(px2.node());

	// Type definition with contraints
	// #type IntRange = &int[1 ...10];
	std::cout << "Testing #type IntRange = &int[1 ...10];" << std::endl;
	auto found_scope3 = caoco::find_open_statement(caoco::tk_enum::type_, caoco::tk_enum::eos, found_scope2.scope_end(), result.cend());
	auto px3 = caoco::ParseDirectiveType()(found_scope3.scope_begin(), found_scope3.scope_end());
	EXPECT_TRUE(px3.valid());
	print_ast(px3.node());

	// Anon Var Decl
	// #var foo;
	std::cout << "Testing #var foo;" << std::endl;
	auto found_scope4 = caoco::ParseDirectiveVar()(found_scope3.scope_end(), result.cend());
	EXPECT_TRUE(found_scope4.valid());
	print_ast(found_scope4.node());

	// Anon Var Decl Assingment
	// #var foo = 1;
	std::cout << "Testing #var foo = 1;" << std::endl;
	auto found_scope5 = caoco::ParseDirectiveVar()(found_scope4.it(), result.cend());
	EXPECT_TRUE(found_scope5.valid());
	print_ast(found_scope5.node());

	// Complex Anon Var Decl Assingment
	// #var foo = 1 + c * (3 / 4);
	std::cout << "Testing #var foo = 1 + c * (3 / 4);" << std::endl;
	auto found_scope6 = caoco::ParseDirectiveVar()(found_scope5.it(), result.cend());
	EXPECT_TRUE(found_scope6.valid());
	print_ast(found_scope6.node());

	// Type Contrained Var Decl
	// #var [Int] foo;
	std::cout << "Testing #var [Int] foo;" << std::endl;
	auto found_scope7 = caoco::ParseDirectiveVar()(found_scope6.it(), result.cend());
	EXPECT_TRUE(found_scope7.valid());
	print_ast(found_scope7.node());

	// Type Contrained Var Decl Assingment
	// #var [Int] foo = 1;
	std::cout << "Testing #var [Int] foo = 1;" << std::endl;
	auto found_scope8 = caoco::ParseDirectiveVar()(found_scope7.it(), result.cend());
	EXPECT_TRUE(found_scope8.valid());
	print_ast(found_scope8.node());

	// Complex Type Constrined Var Decl Assingment
	// #var [Int] foo = 1 + c * (3 / 4);
	std::cout << "Testing #var [Int] foo = 1 + c * (3 / 4);" << std::endl;
	auto found_scope9 = caoco::ParseDirectiveVar()(found_scope8.it(), result.cend());
	EXPECT_TRUE(found_scope9.valid());
	print_ast(found_scope9.node());



}
#endif
#if caocotest_CaocoParser_BasicNode_Functions
TEST(CaocoParser_BasicNode_Functions, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_function.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Shorthand Void Arg Method <#func> <alnumus> <functional_block>
	std::cout << "Testing #func foo {};" << std::endl;
	auto shorthand_void_arg_method = caoco::ParseDirectiveFunc()(result.cbegin(), result.cend());
	EXPECT_TRUE(shorthand_void_arg_method.valid());
	if (!shorthand_void_arg_method.valid()) {
		std::cout << shorthand_void_arg_method.error_message() << std::endl;
	}
	print_ast(shorthand_void_arg_method.node());

	// Unconstrained Method Definition <#func> <alnumus> <arguments> <functional_block>
	std::cout << "Testing #func foo (a, b, c) {};" << std::endl;
	auto unconstrained_method = caoco::ParseDirectiveFunc()(shorthand_void_arg_method.it(), result.cend());
	EXPECT_TRUE(unconstrained_method.valid());
	if (!unconstrained_method.valid()) {
		std::cout << unconstrained_method.error_message() << std::endl;
	}
	print_ast(unconstrained_method.node());

	// Unconstrained Method Definition w/ no Args
	std::cout << "Testing #func foo() {};" << std::endl;
	auto unconstrained_method_no_args = caoco::ParseDirectiveFunc()(unconstrained_method.it(), result.cend());
	EXPECT_TRUE(unconstrained_method_no_args.valid());
	if (!unconstrained_method_no_args.valid()) {
		std::cout << unconstrained_method_no_args.error_message() << std::endl;
	}
	print_ast(unconstrained_method_no_args.node());


	// Constrained Shorthand Void Arg Method #func [&int] foo {};
	std::cout << "Testing #func [&int] foo {};" << std::endl;
	auto constrained_shorthand_void_arg_method = caoco::ParseDirectiveFunc()(unconstrained_method_no_args.it(), result.cend());
	EXPECT_TRUE(constrained_shorthand_void_arg_method.valid());
	if (!constrained_shorthand_void_arg_method.valid()) {
		std::cout << constrained_shorthand_void_arg_method.error_message() << std::endl;
	}
	print_ast(constrained_shorthand_void_arg_method.node());


	// Constrained method definition #func [&int] foo (a, b, c) {};
	std::cout << "Testing #func [&int] foo (a, b, c) {};" << std::endl;
	auto constrained_method = caoco::ParseDirectiveFunc()(constrained_shorthand_void_arg_method.it(), result.cend());
	EXPECT_TRUE(constrained_method.valid());
	if (!constrained_method.valid()) {
		std::cout << constrained_method.error_message() << std::endl;
	}
	print_ast(constrained_method.node());

	/*Function definition with returns and statements
		#func foo{
			#return 1;
		};
	*/
	std::cout << "Testing #func foo{#return 1;}; " << std::endl;	
	auto function_with_return = caoco::ParseDirectiveFunc()(constrained_method.it(), result.cend());
	EXPECT_TRUE(function_with_return.valid());
	if (!function_with_return.valid()) {
		std::cout << function_with_return.error_message() << std::endl;
	}
	print_ast(function_with_return.node());


	/* Function definition with return and multiple statements
		#func foo {
			a = 1;
			b = 2;
			#return a + b;
		};
	*/
	std::cout << "Testing #func foo {a = 1;b = 2;#return a + b;}; " << std::endl;
	auto function_with_return_and_statements = caoco::ParseDirectiveFunc()(function_with_return.it(), result.cend());
	EXPECT_TRUE(function_with_return_and_statements.valid());
	if (!function_with_return_and_statements.valid()) {
		std::cout << function_with_return_and_statements.error_message() << std::endl;
	}
	print_ast(function_with_return_and_statements.node());

}
#endif
#if caocotest_CaocoParser_BasicNode_Classes
TEST(CaocoParser_BasicNode_Classes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_classes.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto empty_class_def_end = test_parsing_functor("Empty Class Definition", caoco::ParseDirectiveClass(), result.cbegin(), result.cend());
	auto class_def_with_members_end = test_parsing_functor("Class Definition with Members", caoco::ParseDirectiveClass(), empty_class_def_end, result.cend());
	auto class_def_with_members_and_methods_end = test_parsing_functor("Class Definition with Members and Methods", caoco::ParseDirectiveClass(), class_def_with_members_end, result.cend());
}
#endif
#if caocotest_CaocoParser_MinimumProgram 
TEST(CaocoParser_MinimumProgram, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("parser_unit_test_0_minimum_program.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto parse_result = caoco::ParsePragmaticBlock()(result.cbegin(), result.cend());
	EXPECT_TRUE(parse_result.valid());
	if (!parse_result.valid()) {
		std::cout << parse_result.error_message() << std::endl;
	}
	print_ast(parse_result.node());
}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Constant Evaluator Tests
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
#define caocotest_CaocoConstantEvaluator_Literals 1
#define caocotest_CaocoConstantEvaluator_Operators 1
#define caocotest_CaocoConstantEvaluator_VariableDeclaration 1
#define caocotest_CaocoConstantEvaluator_Structs 1
#define caocotest_CaocoConstantEvaluator_FreeFunctions 1

#if caocotest_CaocoConstantEvaluator_Literals
TEST(CaocoConstantEvaluator_Literals, CaocoConstantEvaluator_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("constant_evaluator_unit_test_0_literals.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Create the runtime namespace/scope/environment.
	// For clarity: - All three have the same meaning in this context,but we will use the term "environment".
	//              - Namespace would clash with C++ namespace, and scope is already used in the parser.
	//              - Each environment has a parent environment, which is nullptr for the root environment(global scope).
	//              - Each environment also has a list of sub-environments, which are the child scopes.
	auto runtime_env = caoco::rtenv("global");
 

	// Test the constant evaluator on literals
	auto int_literal = caoco::ParseNumberLiteral()(result.cbegin(), result.cend());
	auto eval_result = caoco::CNumberEval()(int_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 42);

	// real literal
	auto real_literal = caoco::ParseRealLiteral()(int_literal.it(), result.cend());
	eval_result = caoco::CRealEval()(real_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::REAL);
	EXPECT_EQ(std::get<double>(eval_result.value), 42.42);

	// string literal
	auto string_literal = caoco::ParseStringLiteral()(real_literal.it(), result.cend());
	eval_result = caoco::CStringEval()(string_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::STRING);
	EXPECT_EQ(std::get<caoco::sl_string>(eval_result.value), "Hello'World");

	// bit literal
	auto bit_literal = caoco::ParseBitLiteral()(string_literal.it(), result.cend());
	eval_result = caoco::CBitEval()(bit_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::BIT);
	EXPECT_EQ(std::get<bool>(eval_result.value), true);

	// unsigned int literal
	auto uint_literal = caoco::ParseUnsignedLiteral()(bit_literal.it(), result.cend());
	eval_result = caoco::CUnsignedEval()(uint_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::UNSIGNED);
	EXPECT_EQ(std::get<unsigned int>(eval_result.value), 42u);

	// octet literal
	auto octet_literal = caoco::ParseOctetLiteral()(uint_literal.it(), result.cend());
	eval_result = caoco::COctetEval()(octet_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::OCTET);
	EXPECT_EQ(std::get<unsigned char>(eval_result.value),(unsigned char)42 );

	// octet  from char
	auto octet_from_char =caoco::ParseOctetLiteral()(octet_literal.it(), result.cend());
	eval_result = caoco::COctetEval()(octet_from_char.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::OCTET);
	EXPECT_EQ(std::get<unsigned char>(eval_result.value), (unsigned char)'a');

	// none
	auto none_literal = caoco::ParseDirectiveNone()(octet_from_char.it(), result.cend());
	eval_result = caoco::CNoneEval()(none_literal.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NONE);
	EXPECT_EQ(std::get<caoco::none_t>(eval_result.value), caoco::none_t{});


}
#endif
#if caocotest_CaocoConstantEvaluator_Operators
TEST(CaocoConstantEvaluator_Operators, CaocoConstantEvaluator_Operators) {
	auto source_file = caoco::sl::load_file_to_char8_vector("constant_evaluator_unit_test_0_operators.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto runtime_env = caoco::rtenv("global");

	// operator <numlit><+><numlit> 1+1
	auto expr = caoco::ParseValueExpression()(result.cbegin(), result.cend());
	auto eval_result = caoco::CAddOpEval()(expr.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 2);

	// using CBinopEval
	// operator <numlit><+><numlit> 1+1
	expr = caoco::ParseValueExpression()(result.cbegin() ,result.cend());
	eval_result = caoco::CBinopEval()(expr.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 2);


	// multiple operators <numlit><+><numlit><+><numlit> 1+1+1
	expr = caoco::ParseValueExpression()(expr.it(), result.cend());
	eval_result = caoco::CBinopEval()(expr.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 5);


	// operator -        1 + 1 - 1
	expr = caoco::ParseValueExpression()(expr.it(), result.cend());
	eval_result = caoco::CBinopEval()(expr.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 1);

	// operators + - * / %
	// 1 + 1 - 1 * 1 / 1 % 1 (== 2)
	expr = caoco::ParseValueExpression()(expr.it(), result.cend());
	eval_result = caoco::CBinopEval()(expr.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 2);

	// variable in expression
	//1 + a;
	runtime_env.create_variable("a", caoco::RTValue(caoco::RTValue::eType::NUMBER,42));
	expr = caoco::ParseValueExpression()(expr.it(), result.cend());
	eval_result = caoco::CBinopEval()(expr.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 43);


}
#endif
#if caocotest_CaocoConstantEvaluator_VariableDeclaration
TEST(CaocoConstantEvaluator_VariableDeclaration, CaocoConstantEvaluator_VariableDeclaration) {
	auto source_file = caoco::sl::load_file_to_char8_vector("constant_evaluator_unit_test_0_variable_declaration.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto runtime_env = caoco::rtenv("global");

	// #var a = 1;
	auto var_decl = caoco::ParseDirectiveVar()(result.cbegin(), result.cend());
	//print_ast(var_decl.node());
	EXPECT_TRUE(var_decl.valid());
	auto eval_result = caoco::CVarDeclEval()(var_decl.node(), runtime_env);
	EXPECT_EQ(eval_result.type, caoco::RTValue::eType::NUMBER);
	EXPECT_EQ(std::get<int>(eval_result.value), 1);
	//std::cout << "Variable a = " << std::get<int>(runtime_env.resolve_variable("a").value().value) << std::endl;
	EXPECT_EQ(std::get<int>(runtime_env.resolve_variable("a").value().value), 1);
}
#endif
#if caocotest_CaocoConstantEvaluator_Structs
TEST(CaocoConstantEvaluator_Structs, CaocoConstantEvaluator_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("constant_evaluator_unit_test_0_structs.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto runtime_env = caoco::rtenv("global");

	/* Basic class with variable members
		#class Foo {
			#var a = 1;
			#var b = 2;
		};
	*/
	//caoco::ParseDirectiveClass()
	auto class_decl = caoco::ParseDirectiveClass()(result.cbegin(), result.cend());
	EXPECT_TRUE(class_decl.valid());
	print_ast(class_decl.node());
	auto eval_result = caoco::CClassDeclEval()(class_decl.node(), runtime_env);

	auto class_obj = std::get<std::shared_ptr<caoco::object_t>>(eval_result.value).get();
	EXPECT_EQ(std::get<int>(class_obj->get_member("a").value), 1);
	EXPECT_EQ(std::get<int>(class_obj->get_member("b").value), 2);
	EXPECT_EQ(std::get<int>(class_obj->get_member("c").value), 3);
}
#endif
#if caocotest_CaocoConstantEvaluator_FreeFunctions
TEST(CaocoConstantEvaluator_FreeFunctions, CaocoConstantEvaluator_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("constant_evaluator_unit_test_0_free_functions.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto runtime_env = caoco::rtenv("global");

	/* 
		#func add(x) {
			#return x + 40;
		};
		add(2);
	*/
	auto func_decl = caoco::ParseDirectiveFunc()(result.cbegin(), result.cend());
	EXPECT_TRUE(func_decl.valid());
	print_ast(func_decl.node());
	auto eval_result = caoco::CFunctionDeclEval()(func_decl.node(), runtime_env);

	// Check if function was declared in global scope.
	auto func_obj_from_env = runtime_env.resolve_variable("add");

	// check type of function object
	EXPECT_EQ(func_obj_from_env.value().type, caoco::RTValue::eType::FUNCTION);


	//// Call the function.
	//auto func_call = caoco::ParseValueExpression()(func_decl.it(), result.cend());
	//EXPECT_TRUE(func_call.valid());
	//print_ast(func_call.node());
	//auto eval_result2 = caoco::CFunctionCallEval()(func_call.node(), runtime_env);

	//// Check the result of the function call.
	//EXPECT_EQ(eval_result2.type, caoco::RTValue::eType::NUMBER);
	//EXPECT_EQ(std::get<int>(eval_result2.value), 42);

}
#endif
