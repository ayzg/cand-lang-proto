#pragma once
#include "gtest/gtest.h"
// All unit test dependencies are included here
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
	case(caoco::tk_enum::none_): return "none";
	case(caoco::tk_enum::invalid_): return "invalid";
	case(caoco::tk_enum::eof_): return "eof";
	case(caoco::tk_enum::line_comment_): return "line_comment";
	case(caoco::tk_enum::block_comment_): return "block_comment";
	case(caoco::tk_enum::string_literal_): return "string_literal";
	case(caoco::tk_enum::number_literal_): return "number_literal";
	case(caoco::tk_enum::real_literal_): return "real_literal";
	case(caoco::tk_enum::newline_): return "newline";
	case(caoco::tk_enum::whitespace_): return "whitespace";
	case(caoco::tk_enum::alnumus_): return "alnumus";
	case(caoco::tk_enum::simple_assignment_): return "simple_assignment";
	case(caoco::tk_enum::addition_assignment_): return "addition_assignment";
	case(caoco::tk_enum::subtraction_assignment_): return "subtraction_assignment";
	case(caoco::tk_enum::multiplication_assignment_): return "multiplication_assignment";
	case(caoco::tk_enum::division_assignment_): return "division_assignment";
	case(caoco::tk_enum::remainder_assignment_): return "remainder_assignment";
	case(caoco::tk_enum::bitwise_and_assignment_): return "bitwise_and_assignment";
	case(caoco::tk_enum::bitwise_or_assignment_): return "bitwise_or_assignment";
	case(caoco::tk_enum::bitwise_xor_assignment_): return "bitwise_xor_assignment";
	case(caoco::tk_enum::left_shift_assignment_): return "left_shift_assignment";
	case(caoco::tk_enum::right_shift_assignment_): return "right_shift_assignment";
	case(caoco::tk_enum::increment_): return "increment";
	case(caoco::tk_enum::decrement_): return "decrement";
	case(caoco::tk_enum::addition_): return "addition";
	case(caoco::tk_enum::subtraction_): return "subtraction";
	case(caoco::tk_enum::multiplication_): return "multiplication";
	case(caoco::tk_enum::division_): return "division";
	case(caoco::tk_enum::remainder_): return "remainder";
	case(caoco::tk_enum::bitwise_NOT_): return "bitwise_NOT";
	case(caoco::tk_enum::bitwise_AND_): return "bitwise_AND";
	case(caoco::tk_enum::bitwise_OR_): return "bitwise_OR";
	case(caoco::tk_enum::bitwise_XOR_): return "bitwise_XOR";
	case(caoco::tk_enum::bitwise_left_shift_): return "bitwise_left_shift";
	case(caoco::tk_enum::bitwise_right_shift_): return "bitwise_right_shift";
	case(caoco::tk_enum::negation_): return "negation";
	case(caoco::tk_enum::logical_AND_): return "logical_AND";
	case(caoco::tk_enum::logical_OR_): return "logical_OR";
	case(caoco::tk_enum::equal_): return "equal";
	case(caoco::tk_enum::not_equal_): return "not_equal";
	case(caoco::tk_enum::less_than_): return "less_than";
	case(caoco::tk_enum::greater_than_): return "greater_than";
	case(caoco::tk_enum::less_than_or_equal_): return "less_than_or_equal";
	case(caoco::tk_enum::greater_than_or_equal_): return "greater_than_or_equal";
	case(caoco::tk_enum::three_way_comparison_): return "three_way_comparison";
	case(caoco::tk_enum::open_scope_): return "open_scope";
	case(caoco::tk_enum::close_scope_): return "close_scope";
	case(caoco::tk_enum::open_list_): return "open_list";
	case(caoco::tk_enum::close_list_): return "close_list";
	case(caoco::tk_enum::open_frame_): return "open_frame";
	case(caoco::tk_enum::close_frame_): return "close_frame";
	case(caoco::tk_enum::eos_): return "eos";
	case(caoco::tk_enum::comma_): return "comma";
	case(caoco::tk_enum::period_): return "period";
	case(caoco::tk_enum::ellipsis_): return "ellipsis";
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
	case(caoco::tk_enum::bit_literal_): return "bit_literal";
	case(caoco::tk_enum::octet_literal_): return "octet_literal";
	case(caoco::tk_enum::unsigned_literal_): return "unsigned_literal";
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
	case caoco::astnode_enum::conditional_block_: debug_string += "conditional_block_"; break;
	case caoco::astnode_enum::conditional_statement_: debug_string += "conditional_statement_"; break;
	case caoco::astnode_enum::bit_literal_: debug_string += "bit_literal_"; break;
	case caoco::astnode_enum::octet_literal_: debug_string += "octet_literal_"; break;
	case caoco::astnode_enum::unsigned_literal_: debug_string += "unsigned_literal_"; break;
	case caoco::astnode_enum::none_literal_: debug_string += "none_literal_"; break;
		case caoco::astnode_enum::unary_minus_: debug_string += "unary_minus_"; break;
			case caoco::astnode_enum::aint_constrained_: debug_string += "aint_constrained_"; break;
	default: debug_string += "This node type is not string-convertible. Please implement a string conversion for this node type in the node_debug_string function in test.cpp.";
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
	for (auto& child : node.children())
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

// Test a parsing functor given a subset of tokens. Prints the test_name followed by the AST.
template<typename ParsingFunctorT>
caoco::tk_vector_cit test_parsing_function(caoco::sl_string test_name, ParsingFunctorT&& parsing_functor, caoco::tk_vector_cit begin, caoco::tk_vector_cit end) {
	// Empty Class Definition
	std::cout << "[Testing Parsing Method][Test Case:" << test_name << "]" << std::endl;
	auto parse_result = parsing_functor(begin, end);
	EXPECT_TRUE(parse_result.valid());
	if (!parse_result.valid()) {
		std::cout << parse_result.error_message() << std::endl;
	}
	print_ast(parse_result.expected()); // Print the AST for debugging purposes
	return parse_result.always(); // The correct value is 1 past the end of the parsed tokens, so the next parsing method can start there.
}