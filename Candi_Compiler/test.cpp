#include "pch.h"
#include <type_traits>
#include "char_traits.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "ast_node.hpp"
#include "parser.hpp"
//#include "transpiler.hpp"
//#include "candi_main.hpp"
#include "constant_evaluator.hpp"


//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Tokenizer Tests
// dependencies: tokenizer.hpp
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
std::string token_type_to_string(caoco::Tk::eType type) {
	/*
			enum class eType : int {
				// Abstract
				none = -1,
				invalid,
				eof,

				// Base
				line_comment,
				block_comment,
				string_literal,
				number_literal,
				real_literal,
				newline,
				whitespace,
				alnumus,

				// Assignemnt operators
				simple_assignment, // =
				addition_assignment, // +=
				subtraction_assignment, // -=
				multiplication_assignment, // *=
				division_assignment, // /=
				remainder_assignment, // %=
				bitwise_and_assignment, // &=
				bitwise_or_assignment, // |=
				bitwise_xor_assignment, // ^=
				left_shift_assignment, // <<=
				right_shift_assignment, // >>=

				// Increment and decrement operators
				increment, // ++
				decrement, // --

				// Arithmetic operators
				addition, // +
				subtraction, // -
				multiplication, // *
				division, // /
				remainder, // %
				bitwise_NOT, // ~
				bitwise_AND, // &
				bitwise_OR, // |
				bitwise_XOR, // ^
				bitwise_left_shift, // <<
				bitwise_right_shift, // >>

				// Logical
				negation, // !
				logical_AND, // &&
				logical_OR, // ||

				// Comparison
				equal, // ==
				not_equal, // !=
				less_than, // <
				greater_than, // >
				less_than_or_equal, // <=
				greater_than_or_equal, // >=
				three_way_comparison, // <=>

				// Scopes
				open_scope, // (
				close_scope, // )
				open_list, // {
				close_list, // }
				open_frame, // [
				close_frame, // ]

				// Special
				eos, // ;
				comma, // ,
				period, // .
				ellipsis, // ...

				// Special Tokens
				atype_, // &type
				aidentity_, // &identity
				avalue_, // &value
				aint_, // &int[RANGE[-inf-inf]]
				auint_, // &uint[RANGE[0...inf]]
				areal_, // &real[RANGE[-inf...inf]]
				aureal_, // &ureal[RANGE[0...inf]]
				aoctet_, // &octet[RANGE[0...255]]
				abit_, // &bit[RANGE[0...1]]
				aarray_, // &array[T,Size] // T is a type
				apointer_, // &pointer[T] // T is a type
				amemory_, // &memory[T,Size] // T is a type
				afunction_, // &function

				// Directive Tokens
				enter_, // #enter
				start_, // #start
				type_, // #type
				var_, // #var
				class_, // #class
				func_, // #func
				print_, // #print

				// Modifier Tokens
				public_, // #public
				const_, // #const
				static_, // #static
				ref_, // #ref

				// Control Flow Tokens
				if_, // #if
				else_, // #else
				elif_, // #elif
				while_, // #while
				for_, // #for
				switch_, // #switch
				case_, // #case
				default_, // #default
				break_, // #break
				continue_, // #continue
				return_, // #ret
				into_ // #into
			};

	*/
	switch (type) {
	case(caoco::Tk::eType::none): return "none";
	case(caoco::Tk::eType::invalid): return "invalid";
	case(caoco::Tk::eType::eof): return "eof";
	case(caoco::Tk::eType::line_comment): return "line_comment";
	case(caoco::Tk::eType::block_comment): return "block_comment";
	case(caoco::Tk::eType::string_literal): return "string_literal";
	case(caoco::Tk::eType::number_literal): return "number_literal";
	case(caoco::Tk::eType::real_literal): return "real_literal";
	case(caoco::Tk::eType::newline): return "newline";
	case(caoco::Tk::eType::whitespace): return "whitespace";
	case(caoco::Tk::eType::alnumus): return "alnumus";
	case(caoco::Tk::eType::simple_assignment): return "simple_assignment";
	case(caoco::Tk::eType::addition_assignment): return "addition_assignment";
	case(caoco::Tk::eType::subtraction_assignment): return "subtraction_assignment";
	case(caoco::Tk::eType::multiplication_assignment): return "multiplication_assignment";
	case(caoco::Tk::eType::division_assignment): return "division_assignment";
	case(caoco::Tk::eType::remainder_assignment): return "remainder_assignment";
	case(caoco::Tk::eType::bitwise_and_assignment): return "bitwise_and_assignment";
	case(caoco::Tk::eType::bitwise_or_assignment): return "bitwise_or_assignment";
	case(caoco::Tk::eType::bitwise_xor_assignment): return "bitwise_xor_assignment";
	case(caoco::Tk::eType::left_shift_assignment): return "left_shift_assignment";
	case(caoco::Tk::eType::right_shift_assignment): return "right_shift_assignment";
	case(caoco::Tk::eType::increment): return "increment";
	case(caoco::Tk::eType::decrement): return "decrement";
	case(caoco::Tk::eType::addition): return "addition";
	case(caoco::Tk::eType::subtraction): return "subtraction";
	case(caoco::Tk::eType::multiplication): return "multiplication";
	case(caoco::Tk::eType::division): return "division";
	case(caoco::Tk::eType::remainder): return "remainder";
	case(caoco::Tk::eType::bitwise_NOT): return "bitwise_NOT";
	case(caoco::Tk::eType::bitwise_AND): return "bitwise_AND";
	case(caoco::Tk::eType::bitwise_OR): return "bitwise_OR";
	case(caoco::Tk::eType::bitwise_XOR): return "bitwise_XOR";
	case(caoco::Tk::eType::bitwise_left_shift): return "bitwise_left_shift";
	case(caoco::Tk::eType::bitwise_right_shift): return "bitwise_right_shift";
	case(caoco::Tk::eType::negation): return "negation";
	case(caoco::Tk::eType::logical_AND): return "logical_AND";
	case(caoco::Tk::eType::logical_OR): return "logical_OR";
	case(caoco::Tk::eType::equal): return "equal";
	case(caoco::Tk::eType::not_equal): return "not_equal";
	case(caoco::Tk::eType::less_than): return "less_than";
	case(caoco::Tk::eType::greater_than): return "greater_than";
	case(caoco::Tk::eType::less_than_or_equal): return "less_than_or_equal";
	case(caoco::Tk::eType::greater_than_or_equal): return "greater_than_or_equal";
	case(caoco::Tk::eType::three_way_comparison): return "three_way_comparison";
	case(caoco::Tk::eType::open_scope): return "open_scope";
	case(caoco::Tk::eType::close_scope): return "close_scope";
	case(caoco::Tk::eType::open_list): return "open_list";
	case(caoco::Tk::eType::close_list): return "close_list";
	case(caoco::Tk::eType::open_frame): return "open_frame";
	case(caoco::Tk::eType::close_frame): return "close_frame";
	case(caoco::Tk::eType::eos): return "eos";
	case(caoco::Tk::eType::comma): return "comma";
	case(caoco::Tk::eType::period): return "period";
	case(caoco::Tk::eType::ellipsis): return "ellipsis";
	case(caoco::Tk::eType::atype_): return "atype_";
	case(caoco::Tk::eType::aidentity_): return "aidentity_";
	case(caoco::Tk::eType::avalue_): return "avalue_";
	case(caoco::Tk::eType::aint_): return "aint_";
	case(caoco::Tk::eType::auint_): return "auint_";
	case(caoco::Tk::eType::areal_): return "areal_";
	//case(caoco::Tk::eType::aureal_): return "aureal_";
	case(caoco::Tk::eType::aoctet_): return "aoctet_";
	case(caoco::Tk::eType::abit_): return "abit_";
	case(caoco::Tk::eType::aarray_): return "aarray_";
	case(caoco::Tk::eType::apointer_): return "apointer_";
	case(caoco::Tk::eType::amemory_): return "amemory_";
	case(caoco::Tk::eType::afunction_): return "afunction_";
	case(caoco::Tk::eType::enter_): return "enter_";
	case(caoco::Tk::eType::start_): return "start_";
	case(caoco::Tk::eType::type_): return "type_";
	case(caoco::Tk::eType::var_): return "var_";
	case(caoco::Tk::eType::class_): return "class_";
	case(caoco::Tk::eType::func_): return "func_";
	case(caoco::Tk::eType::print_): return "print_";
	case(caoco::Tk::eType::public_): return "public_";
	case(caoco::Tk::eType::const_): return "const_";
	case(caoco::Tk::eType::static_): return "static_";
	case(caoco::Tk::eType::ref_): return "ref_";
	case(caoco::Tk::eType::if_): return "if_";
	case(caoco::Tk::eType::else_): return "else_";
	case(caoco::Tk::eType::elif_): return "elif_";
	case(caoco::Tk::eType::while_): return "while_";
	case(caoco::Tk::eType::for_): return "for_";
	case(caoco::Tk::eType::switch_): return "switch_";
	case(caoco::Tk::eType::case_): return "case_";
	case(caoco::Tk::eType::default_): return "default_";
	case(caoco::Tk::eType::break_): return "break_";
	case(caoco::Tk::eType::continue_): return "continue_";
	case(caoco::Tk::eType::return_): return "return_";
	case(caoco::Tk::eType::into_): return "into_";
	case(caoco::Tk::eType::none_literal_): return "none_literal_";
	case(caoco::Tk::eType::bit_literal): return "bit_literal";
	case(caoco::Tk::eType::octet_literal): return "octet_literal";
	case(caoco::Tk::eType::unsigned_literal): return "unsigned_literal";
	default: return "This token type is not string-convertible. Please implement a string conversion for this token type in the token_type_to_string function in test.cpp.";
	}
}

std::string token_to_string(const caoco::Tk& token) {
	return token_type_to_string(token.type()) + std::string(" : ") + caoco::to_std_string(token.literal());
}

// Workaround.
// Google Test will not do check on caoco::string_t, so we need to define the << operator for char8_t
std::ostream& operator<<(std::ostream& os, char8_t u8) {
	os << u8;
	return os;
}

// This is defined for printing direct u8 literals
std::ostream& operator<<(std::ostream& os, const char8_t * u8_cstr) {
	os << reinterpret_cast<const char*>(u8_cstr);
	return os;
}

using tk_etype = caoco::Tk::eType;

#define caoco_CaocoTokenizer_Tokens 1
#define caoco_CaocoTokenizer_NumberAndReal 1

// Convert u8 string to a vector of char_8t
auto u8str_to_u8vec(const char8_t* str) {
	std::vector<caoco::char_t> vec;
	for (int i = 0; str[i] != '\0'; i++) {
		vec.push_back(str[i]);
	}
	return vec;
}

void test_single_token(const char8_t* input, tk_etype expected_type,caoco::string_t expected_literal) {
	auto input_vec = u8str_to_u8vec(input);
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
	EXPECT_EQ(result.size(), 1);
	EXPECT_EQ(result.at(0).type(), expected_type);
	EXPECT_EQ(result.at(0).literal(), expected_literal);
	std::cout << "[Single token Test][" << input << "]" << token_to_string(result.at(0)) << std::endl;
}

#if caoco_CaocoTokenizer_Tokens
TEST(CaocoTokenizer_Test, CaocoTokenizer_Tokens) {
	// NOTE: the sanitized output will not contain comments/newlines/whitespace. As such there is a seperate test for those tokens.

	// Literals
	// number
	test_single_token(u8"1234\0", tk_etype::number_literal, u8"1234");
	// real
	test_single_token(u8"1234.5678\0", tk_etype::real_literal, u8"1234.5678");
	// string 
	test_single_token(u8"'hello world'\0", tk_etype::string_literal, u8"'hello world'");
	// string with escape character '\''
	test_single_token(u8"'hello \\' world'\0", tk_etype::string_literal, u8"'hello \\' world'");
	// alnumus
	test_single_token(u8"hello_world\0", tk_etype::alnumus, u8"hello_world");
	// unsigned literal u
	test_single_token(u8"1234u\0", tk_etype::unsigned_literal, u8"1234u");
	// bit literal 0b 1b
	test_single_token(u8"0b\0", tk_etype::bit_literal, u8"0b");
	test_single_token(u8"1b\0", tk_etype::bit_literal, u8"1b");
	// octet literal 0c 255c
	test_single_token(u8"0c\0", tk_etype::octet_literal, u8"0c");
	test_single_token(u8"255c\0", tk_etype::octet_literal, u8"255c");

	// octet literal 'a'c
	test_single_token(u8"'a'c\0", tk_etype::octet_literal, u8"'a'c");
	// octet literal '\''c
	test_single_token(u8"'\\''c\0", tk_etype::octet_literal, u8"'\\''c");

	// Directives
	// Functional
	// #enter,#start,#type,#var,#class,#print,#func
	test_single_token(u8"#enter\0", tk_etype::enter_, u8"#enter"); 
	test_single_token(u8"#start\0", tk_etype::start_, u8"#start");
	test_single_token(u8"#type\0", tk_etype::type_, u8"#type");
	test_single_token(u8"#var\0", tk_etype::var_, u8"#var");
	test_single_token(u8"#class\0", tk_etype::class_, u8"#class");
	test_single_token(u8"#print\0", tk_etype::print_, u8"#print");
	test_single_token(u8"#func\0", tk_etype::func_, u8"#func");
	test_single_token(u8"#none\0", tk_etype::none_literal_, u8"#none"); 

	// Modifiers
	// #public,#const,#static,#ref,
	test_single_token(u8"#public\0", tk_etype::public_, u8"#public");
	test_single_token(u8"#const\0", tk_etype::const_, u8"#const");
	test_single_token(u8"#static\0", tk_etype::static_, u8"#static");
	test_single_token(u8"#ref\0", tk_etype::ref_, u8"#ref");

	// Control Flow
	// #if,#else,#elif,#while,#for,#switch,#case,#default,#break,#continue,#return,#into
	test_single_token(u8"#if\0", tk_etype::if_, u8"#if");
	test_single_token(u8"#else\0", tk_etype::else_, u8"#else");
	test_single_token(u8"#elif\0", tk_etype::elif_, u8"#elif");
	test_single_token(u8"#while\0", tk_etype::while_, u8"#while");
	test_single_token(u8"#for\0", tk_etype::for_, u8"#for");
	test_single_token(u8"#switch\0", tk_etype::switch_, u8"#switch");
	test_single_token(u8"#case\0", tk_etype::case_, u8"#case");
	test_single_token(u8"#default\0", tk_etype::default_, u8"#default");
	test_single_token(u8"#break\0", tk_etype::break_, u8"#break");
	test_single_token(u8"#continue\0", tk_etype::continue_, u8"#continue");
	test_single_token(u8"#return\0", tk_etype::return_, u8"#return");
	test_single_token(u8"#into\0", tk_etype::into_, u8"#into");

	// Assignment Operators
	// =,+=,-=,*=,/=,%=,&=,|=,^=,<<=,>>=
	test_single_token(u8"=\0", tk_etype::simple_assignment, u8"=");
	test_single_token(u8"+=\0", tk_etype::addition_assignment, u8"+=");
	test_single_token(u8"-=\0", tk_etype::subtraction_assignment, u8"-=");
	test_single_token(u8"*=\0", tk_etype::multiplication_assignment, u8"*=");
	test_single_token(u8"/=\0", tk_etype::division_assignment, u8"/=");
	test_single_token(u8"%=\0", tk_etype::remainder_assignment, u8"%=");
	test_single_token(u8"&=\0", tk_etype::bitwise_and_assignment, u8"&=");
	test_single_token(u8"|=\0", tk_etype::bitwise_or_assignment, u8"|=");
	test_single_token(u8"^=\0", tk_etype::bitwise_xor_assignment, u8"^=");
	test_single_token(u8"<<=\0", tk_etype::left_shift_assignment, u8"<<=");
	test_single_token(u8">>=\0", tk_etype::right_shift_assignment, u8">>=");

	// Increment and Decrement Operators
	// ++,--
	test_single_token(u8"++\0", tk_etype::increment, u8"++");
	test_single_token(u8"--\0", tk_etype::decrement, u8"--");

	//// Arithmetic Operators
	//// +,-,*,/,%,~,&,|,^,<<,>>
	test_single_token(u8"+\0", tk_etype::addition, u8"+");
	test_single_token(u8"-\0", tk_etype::subtraction, u8"-");
	test_single_token(u8"*\0", tk_etype::multiplication, u8"*");
	test_single_token(u8"/\0", tk_etype::division, u8"/");
	test_single_token(u8"%\0", tk_etype::remainder, u8"%");
	test_single_token(u8"~\0", tk_etype::bitwise_NOT, u8"~");
	test_single_token(u8"&\0", tk_etype::bitwise_AND, u8"&");
	test_single_token(u8"|\0", tk_etype::bitwise_OR, u8"|");
	test_single_token(u8"^\0", tk_etype::bitwise_XOR, u8"^");
	test_single_token(u8"<<\0", tk_etype::bitwise_left_shift, u8"<<");
	test_single_token(u8">>\0", tk_etype::bitwise_right_shift, u8">>");

	//// Logical Operators
	//// !,&&,||
	test_single_token(u8"!\0", tk_etype::negation, u8"!");
	test_single_token(u8"&&\0", tk_etype::logical_AND, u8"&&");
	test_single_token(u8"||\0", tk_etype::logical_OR, u8"||");

	//// Comparison Operators
	//// ==,!=,<,>,<=,>=,<=>
	test_single_token(u8"==\0", tk_etype::equal, u8"==");
	test_single_token(u8"!=\0", tk_etype::not_equal, u8"!=");
	test_single_token(u8"<\0", tk_etype::less_than, u8"<");
	test_single_token(u8">\0", tk_etype::greater_than, u8">");
	test_single_token(u8"<=\0", tk_etype::less_than_or_equal, u8"<=");
	test_single_token(u8">=\0", tk_etype::greater_than_or_equal, u8">=");
	test_single_token(u8"<=>\0", tk_etype::three_way_comparison, u8"<=>"); 

	//// Scopes
	//// (,),{,},[,]
	test_single_token(u8"(\0", tk_etype::open_scope, u8"(");
	test_single_token(u8")\0", tk_etype::close_scope, u8")");
	test_single_token(u8"{\0", tk_etype::open_list, u8"{");
	test_single_token(u8"}\0", tk_etype::close_list, u8"}");
	test_single_token(u8"[\0", tk_etype::open_frame, u8"[");
	test_single_token(u8"]\0", tk_etype::close_frame, u8"]");

	//// Special Tokens
	//// ;,:,...
	test_single_token(u8";\0", tk_etype::eos, u8";");
	test_single_token(u8",\0", tk_etype::comma, u8",");
	test_single_token(u8".\0", tk_etype::period, u8".");
	test_single_token(u8"...\0", tk_etype::ellipsis, u8"...");

	// Cand Special Objects
	// &type, &identity, &value, &int, &uint, &real, &octet, &bit, &array, &pointer, &memory, &function
	test_single_token(u8"&type\0", tk_etype::atype_, u8"&type");
	test_single_token(u8"&identity\0", tk_etype::aidentity_, u8"&identity");
	test_single_token(u8"&value\0", tk_etype::avalue_, u8"&value");
	test_single_token(u8"&int\0", tk_etype::aint_, u8"&int");
	test_single_token(u8"&uint\0", tk_etype::auint_, u8"&uint");
	test_single_token(u8"&real\0", tk_etype::areal_, u8"&real");
	//test_single_token(u8"&ureal\0", tk_etype::aureal_, u8"&ureal");
	test_single_token(u8"&octet\0", tk_etype::aoctet_, u8"&octet");
	test_single_token(u8"&bit\0", tk_etype::abit_, u8"&bit");
	test_single_token(u8"&array\0", tk_etype::aarray_, u8"&array");
	test_single_token(u8"&pointer\0", tk_etype::apointer_, u8"&pointer");
	test_single_token(u8"&memory\0", tk_etype::amemory_, u8"&memory");
	test_single_token(u8"&function\0", tk_etype::afunction_, u8"&function");

};
#endif
#if caoco_CaocoTokenizer_NumberAndReal
TEST(CaocoTokenizer_Test, CaocoTokenizer_NumberAndReal) {
	// Specific test for a number followed by an ellipsis

	auto input_vec = u8str_to_u8vec(u8"1234...\0");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result.at(0).type(), tk_etype::number_literal);
	EXPECT_EQ(result.at(0).literal(), caoco::string_t(u8"1234"));
	EXPECT_EQ(result.at(1).type(), tk_etype::ellipsis);
	EXPECT_EQ(result.at(1).literal(), caoco::string_t(u8"..."));

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

// Util method to print the AST for debugging purposes
void print_ast(const caoco::Node& node, int depth = 0) {
	for (int i = 0; i < depth; i++)
		std::cout << "  ";
	std::cout << node.debug_string() << std::endl;
	for (auto& child : node.body())
		print_ast(child, depth + 1);
}

// Test a parsing functor given a subset of tokens. Prints the test_name followed by the AST.
template<typename ParsingFunctorT> requires std::is_base_of_v<caoco::ParsingProcess, ParsingFunctorT>
caoco::tk_iterator_t test_parsing_functor(std::string test_name, ParsingFunctorT&& parsing_functor, caoco::tk_iterator_t begin, caoco::tk_iterator_t end) {
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


#if caocotest_CaocoParser_BasicNode_SingularNodes
TEST(CaocoParser_Test, CaocoParser_BasicNode_SingularNodes) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_basic_node.candi");
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
TEST(CaocoParser_Test, CaocoParser_BasicNode_BasicScopes) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_scopes.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// empty scope
	std::cout << "Testing empty scope:" << std::endl;
	caoco::ScopeResult empty_scope = caoco::find_scope(result.cbegin(), result.cend());
	EXPECT_TRUE(empty_scope.valid);


	// scope with 1 element
	std::cout << "Testing scope with 1 element:" << std::endl;
	caoco::ScopeResult scope_with_1_element = caoco::find_scope(empty_scope.scope_end(), result.cend());
	EXPECT_TRUE(scope_with_1_element.valid);

	// double scope
	std::cout << "Testing double scope:" << std::endl;
	caoco::ScopeResult double_scope = caoco::find_scope(scope_with_1_element.scope_end(), result.cend());
	EXPECT_TRUE(double_scope.valid);

	// complex scope
	std::cout << "Testing complex scope:" << std::endl;
	caoco::ScopeResult complex_scope = caoco::find_scope(double_scope.scope_end(), result.cend());
	EXPECT_TRUE(complex_scope.valid);


	// invalid scope should be invalid	
	std::cout << "Testing invalid scope:" << std::endl;
	caoco::ScopeResult invalid_scope = caoco::find_scope(complex_scope.scope_end(), result.cend());
	EXPECT_FALSE(invalid_scope.valid);

}
#endif
#if caocotest_CaocoParser_BasicNode_StatementScope 
TEST(CaocoParser_Test, CaocoParser_BasicNode_StatementScope) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_statements.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Single value statement : 1;
	std::cout << "Testing single value statement" << std::endl;
	caoco::ScopeResult empty_statement = caoco::find_statement(caoco::tk_enum::number_literal,caoco::tk_enum::eos,result.cbegin(), result.cend());
	EXPECT_TRUE(empty_statement.valid);

	// statement with multiple tokens: #var a = 1;
	std::cout << "Testing statement with multiple tokens" << std::endl;
	caoco::ScopeResult multiple_token_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos, empty_statement.scope_end(), result.cend());
	EXPECT_TRUE(multiple_token_statement.valid);

	// statement with multiple tokens and scopes: #var a = (1;2;3);
	std::cout << "Testing statement with multiple tokens and scopes" << std::endl;
	caoco::ScopeResult multiple_token_scope_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos, multiple_token_statement.scope_end(), result.cend());
	EXPECT_TRUE(multiple_token_scope_statement.valid);

	// statement with lists frames and scopes nested in diffrent ways containing end tokens. #var a = 1 + ([ 2 ;3 + {4;5;6}]);
	std::cout << "Testing statement with lists frames and scopes nested in diffrent ways containing end tokens." << std::endl;
	caoco::ScopeResult complex_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos, multiple_token_scope_statement.scope_end(), result.cend());
	EXPECT_TRUE(complex_statement.valid);
	//EXPECT_TRUE(complex_statement.scope_end() == result.cend() - 1);

	// Test finding an "open" statement which allows for repeated open tokens. ex a = a + a + ([ a ;a + {a;a;a}]);
	std::cout << "Testing statement with lists frames and scopes nested in diffrent ways containing begin and end tokens." << std::endl;
	caoco::ScopeResult open_statement = caoco::find_open_statement(caoco::tk_enum::alnumus, caoco::tk_enum::eos, complex_statement.scope_end(), result.cend());
	EXPECT_TRUE(open_statement.valid);
	EXPECT_TRUE(open_statement.scope_end() == result.cend() - 1);

}
#endif
#if caocotest_CaocoParser_BasicNode_PrimaryExpression
TEST(CaocoParser_Test, CaocoParser_BasicNode_PrimaryExpression) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_primary_expr.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto found_scope = caoco::find_open_statement(caoco::Tk::eType::alnumus, caoco::Tk::eType::eos, result.cbegin(), result.cend());
	// Test parsing a primary expression using build statement method
	std::cout << "Testing foo;" << std::endl;
	auto px1 = caoco::build_statement(found_scope.scope_begin(), found_scope.contained_end());
	EXPECT_TRUE(px1.has_value());
	print_ast(px1.value());


	// foo + 2
	std::cout << "Testing foo + 2;" << std::endl;
	auto found_scope2 = caoco::find_open_statement(caoco::Tk::eType::alnumus, caoco::Tk::eType::eos, found_scope.scope_end(), result.cend());
	auto px2 = caoco::build_statement(found_scope2.scope_begin(), found_scope2.contained_end());
	EXPECT_TRUE(px2.has_value());
	print_ast(px2.value());

	// foo = 1 + 2;
	std::cout << "Testing foo = 1 + 2;" << std::endl;
	auto found_scope3 = caoco::find_open_statement(caoco::Tk::eType::alnumus, caoco::Tk::eType::eos, found_scope2.scope_end(), result.cend());
	auto px3 = caoco::build_statement(found_scope3.scope_begin(), found_scope3.contained_end());
	EXPECT_TRUE(px3.has_value());
	print_ast(px3.value());

	// Testing period(member access) operator
	std::cout << "Testing foo.bar;" << std::endl;
	auto found_scope4 = caoco::find_open_statement(caoco::Tk::eType::alnumus, caoco::Tk::eType::eos, found_scope3.scope_end(), result.cend());
	auto px4 = caoco::build_statement(found_scope4.scope_begin(), found_scope4.contained_end());
	EXPECT_TRUE(px4.has_value());
	print_ast(px4.value());

	// Testing function call operator ()
	std::cout << "Testing foo.bar();" << std::endl;
	auto found_scope5 = caoco::find_open_statement(caoco::Tk::eType::alnumus, caoco::Tk::eType::eos, found_scope4.scope_end(), result.cend());
	auto px5 = caoco::build_statement(found_scope5.scope_begin(), found_scope5.contained_end());
	EXPECT_TRUE(px5.has_value());
	print_ast(px5.value());

	// Statement with no following binary operator should be invalid and throw an exception.
	std::cout << "Testing foo=; Result should throw." << std::endl;
	auto found_scope6 = caoco::find_open_statement(caoco::Tk::eType::alnumus, caoco::Tk::eType::eos, found_scope5.scope_end(), result.cend());
	EXPECT_ANY_THROW(caoco::build_statement(found_scope6.scope_begin(), found_scope6.contained_end()));

}
#endif
#if caocotest_CaocoParser_BasicNode_SimpleStatements
TEST(CaocoParser_Test, CaocoParser_BasicNode_SimpleStatements) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_basic_statement.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Type definition
	// #type Int = &int;
	std::cout << "Testing #type Int = &int;" << std::endl;
	auto found_scope = caoco::find_open_statement(caoco::Tk::eType::type_, caoco::Tk::eType::eos, result.cbegin(), result.cend());
	auto px1 = caoco::ParseDirectiveType()(found_scope.scope_begin(), found_scope.scope_end());
	EXPECT_TRUE(px1.valid());
	print_ast(px1.node());


	// Type alias
	// #type IntAlias = Int;
	std::cout << "Testing #type IntAlias = Int;" << std::endl;
	auto found_scope2 = caoco::find_open_statement(caoco::Tk::eType::type_, caoco::Tk::eType::eos, found_scope.scope_end(), result.cend());
	auto px2 = caoco::ParseDirectiveType()(found_scope2.scope_begin(), found_scope2.scope_end());
	EXPECT_TRUE(px2.valid());
	print_ast(px2.node());

	// Type definition with contraints
	// #type IntRange = &int[1 ...10];
	std::cout << "Testing #type IntRange = &int[1 ...10];" << std::endl;
	auto found_scope3 = caoco::find_open_statement(caoco::Tk::eType::type_, caoco::Tk::eType::eos, found_scope2.scope_end(), result.cend());
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
TEST(CaocoParser_Test, CaocoParser_BasicNode_Functions) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_function.candi");
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
TEST(CaocoParser_Test, CaocoParser_BasicNode_Classes) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_classes.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto empty_class_def_end = test_parsing_functor("Empty Class Definition", caoco::ParseDirectiveClass(), result.cbegin(), result.cend());
	auto class_def_with_members_end = test_parsing_functor("Class Definition with Members", caoco::ParseDirectiveClass(), empty_class_def_end, result.cend());
	auto class_def_with_members_and_methods_end = test_parsing_functor("Class Definition with Members and Methods", caoco::ParseDirectiveClass(), class_def_with_members_end, result.cend());
}
#endif
#if caocotest_CaocoParser_MinimumProgram 
TEST(CaocoParser_Test, CaocoParser_MinimumProgram) {
	auto source_file = caoco::load_source_file("parser_unit_test_0_minimum_program.candi");
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
//----------------------------------------------------------------------------------------------------------------------------------------------------------//

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Constant Evaluator Tests
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
#define caocotest_CaocoConstantEvaluator_Literals 1
#define caocotest_CaocoConstantEvaluator_Operators 1

#if caocotest_CaocoConstantEvaluator_Literals
TEST(CaocoConstantEvaluator_Test, CaocoConstantEvaluator_Literals) {
	auto source_file = caoco::load_source_file("constant_evaluator_unit_test_0_literals.candi");
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
	EXPECT_EQ(std::get<std::string>(eval_result.value), "Hello'World");

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
TEST(CaocoConstantEvaluator_Test, CaocoConstantEvaluator_Operators) {
	auto source_file = caoco::load_source_file("constant_evaluator_unit_test_0_operators.candi");
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

}

#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------//