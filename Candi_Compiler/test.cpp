#include "pch.h"
#include "unit_test_util.hpp"

#define CAOCO_TEST_ALL 1
#define CAOCO_TEST_NONE 0

#if CAOCO_TEST_ALL
#define CAOCO_UT_Tokenizer_Tokens 1
#define CAOCO_UT_Tokenizer_NumberAndReal 1

#define CAOCO_UT_Parser_BasicNode_SingularNodes 1
#define CAOCO_UT_Parser_BasicNode_BasicScopes 1
#define CAOCO_UT_Parser_BasicNode_StatementScope 1
#define CAOCO_UT_Parser_BasicNode_PrimaryExpression 1
#define CAOCO_UT_Parser_BasicNode_SimpleStatements 1
#define CAOCO_UT_Parser_BasicNode_Functions 1
#define CAOCO_UT_Parser_BasicNode_Classes 1
#define CAOCO_UT_Parser_MinimumProgram 1
#define CAOCO_UT_Parser_BasicNode_ConditionalStatements 1
#define CAOCO_UT_Parser_BasicNode_Loops 1

#define CAOCO_UT_ConstantEvaluator_Literals 1
#define CAOCO_UT_ConstantEvaluator_Operators 1
#define CAOCO_UT_ConstantEvaluator_VariableDeclaration 1
#define CAOCO_UT_ConstantEvaluator_Structs 1
#define CAOCO_UT_ConstantEvaluator_FreeFunctions 1

#define CAOCO_UT_Program_BasicProgram 1
#endif

#if CAOCO_TEST_NONE
#define CAOCO_UT_Tokenizer_Tokens 0
#define CAOCO_UT_Tokenizer_NumberAndReal 0

#define CAOCO_UT_Parser_BasicNode_SingularNodes 0
#define CAOCO_UT_Parser_BasicNode_BasicScopes 0
#define CAOCO_UT_Parser_BasicNode_StatementScope 0
#define CAOCO_UT_Parser_BasicNode_PrimaryExpression 0
#define CAOCO_UT_Parser_BasicNode_SimpleStatements 0
#define CAOCO_UT_Parser_BasicNode_Functions 0
#define CAOCO_UT_Parser_BasicNode_Classes 0
#define CAOCO_UT_Parser_MinimumProgram 0
#define CAOCO_UT_Parser_BasicNode_ConditionalStatements 0
#define CAOCO_UT_Parser_BasicNode_Loops 0

#define CAOCO_UT_ConstantEvaluator_Literals 0
#define CAOCO_UT_ConstantEvaluator_Operators 0
#define CAOCO_UT_ConstantEvaluator_VariableDeclaration 0
#define CAOCO_UT_ConstantEvaluator_Structs 0
#define CAOCO_UT_ConstantEvaluator_FreeFunctions 0

#define CAOCO_UT_Program_BasicProgram 0
#endif


#if CAOCO_UT_Tokenizer_Tokens
TEST(CaocoTokenizer_Tokens, CaocoTokenizer_Test) {
	// NOTE: the sanitized output will not contain comments/newlines/whitespace. As such there is a seperate test for those tokens.

	// Literals
	// number
	test_single_token(u8"1234\0", caoco::tk_enum::number_literal_, u8"1234");
	// real
	test_single_token(u8"1234.5678\0", caoco::tk_enum::real_literal_, u8"1234.5678");
	// string 
	test_single_token(u8"'hello world'\0", caoco::tk_enum::string_literal_, u8"'hello world'");
	// string with escape character '\''
	test_single_token(u8"'hello \\' world'\0", caoco::tk_enum::string_literal_, u8"'hello \\' world'");
	// alnumus
	test_single_token(u8"hello_world\0", caoco::tk_enum::alnumus_, u8"hello_world");
	// unsigned literal u
	test_single_token(u8"1234u\0", caoco::tk_enum::unsigned_literal_, u8"1234u");
	// bit literal 0b 1b
	test_single_token(u8"0b\0", caoco::tk_enum::bit_literal_, u8"0b");
	test_single_token(u8"1b\0", caoco::tk_enum::bit_literal_, u8"1b");
	// octet literal 0c 255c
	test_single_token(u8"0c\0", caoco::tk_enum::octet_literal_, u8"0c");
	test_single_token(u8"255c\0", caoco::tk_enum::octet_literal_, u8"255c");

	// octet literal 'a'c
	test_single_token(u8"'a'c\0", caoco::tk_enum::octet_literal_, u8"'a'c");
	// octet literal '\''c
	test_single_token(u8"'\\''c\0", caoco::tk_enum::octet_literal_, u8"'\\''c");

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
	test_single_token(u8"=\0", caoco::tk_enum::simple_assignment_, u8"=");
	test_single_token(u8"+=\0", caoco::tk_enum::addition_assignment_, u8"+=");
	test_single_token(u8"-=\0", caoco::tk_enum::subtraction_assignment_, u8"-=");
	test_single_token(u8"*=\0", caoco::tk_enum::multiplication_assignment_, u8"*=");
	test_single_token(u8"/=\0", caoco::tk_enum::division_assignment_, u8"/=");
	test_single_token(u8"%=\0", caoco::tk_enum::remainder_assignment_, u8"%=");
	test_single_token(u8"&=\0", caoco::tk_enum::bitwise_and_assignment_, u8"&=");
	test_single_token(u8"|=\0", caoco::tk_enum::bitwise_or_assignment_, u8"|=");
	test_single_token(u8"^=\0", caoco::tk_enum::bitwise_xor_assignment_, u8"^=");
	test_single_token(u8"<<=\0", caoco::tk_enum::left_shift_assignment_, u8"<<=");
	test_single_token(u8">>=\0", caoco::tk_enum::right_shift_assignment_, u8">>=");

	// Increment and Decrement Operators
	// ++,--
	test_single_token(u8"++\0", caoco::tk_enum::increment_, u8"++");
	test_single_token(u8"--\0", caoco::tk_enum::decrement_, u8"--");

	//// Arithmetic Operators
	//// +,-,*,/,%,~,&,|,^,<<,>>
	test_single_token(u8"+\0", caoco::tk_enum::addition_, u8"+");
	test_single_token(u8"-\0", caoco::tk_enum::subtraction_, u8"-");
	test_single_token(u8"*\0", caoco::tk_enum::multiplication_, u8"*");
	test_single_token(u8"/\0", caoco::tk_enum::division_, u8"/");
	test_single_token(u8"%\0", caoco::tk_enum::remainder_, u8"%");
	test_single_token(u8"~\0", caoco::tk_enum::bitwise_NOT_, u8"~");
	test_single_token(u8"&\0", caoco::tk_enum::bitwise_AND_, u8"&");
	test_single_token(u8"|\0", caoco::tk_enum::bitwise_OR_, u8"|");
	test_single_token(u8"^\0", caoco::tk_enum::bitwise_XOR_, u8"^");
	test_single_token(u8"<<\0", caoco::tk_enum::bitwise_left_shift_, u8"<<");
	test_single_token(u8">>\0", caoco::tk_enum::bitwise_right_shift_, u8">>");

	//// Logical Operators
	//// !,&&,||
	test_single_token(u8"!\0", caoco::tk_enum::negation_, u8"!");
	test_single_token(u8"&&\0", caoco::tk_enum::logical_AND_, u8"&&");
	test_single_token(u8"||\0", caoco::tk_enum::logical_OR_, u8"||");

	//// Comparison Operators
	//// ==,!=,<,>,<=,>=,<=>
	test_single_token(u8"==\0", caoco::tk_enum::equal_, u8"==");
	test_single_token(u8"!=\0", caoco::tk_enum::not_equal_, u8"!=");
	test_single_token(u8"<\0", caoco::tk_enum::less_than_, u8"<");
	test_single_token(u8">\0", caoco::tk_enum::greater_than_, u8">");
	test_single_token(u8"<=\0", caoco::tk_enum::less_than_or_equal_, u8"<=");
	test_single_token(u8">=\0", caoco::tk_enum::greater_than_or_equal_, u8">=");
	test_single_token(u8"<=>\0", caoco::tk_enum::three_way_comparison_, u8"<=>"); 

	//// Scopes
	//// (,),{,},[,]
	test_single_token(u8"(\0", caoco::tk_enum::open_scope_, u8"(");
	test_single_token(u8")\0", caoco::tk_enum::close_scope_, u8")");
	test_single_token(u8"{\0", caoco::tk_enum::open_list_, u8"{");
	test_single_token(u8"}\0", caoco::tk_enum::close_list_, u8"}");
	test_single_token(u8"[\0", caoco::tk_enum::open_frame_, u8"[");
	test_single_token(u8"]\0", caoco::tk_enum::close_frame_, u8"]");

	//// Special Tokens
	//// ;,:,...
	test_single_token(u8";\0", caoco::tk_enum::eos_, u8";");
	test_single_token(u8",\0", caoco::tk_enum::comma_, u8",");
	test_single_token(u8".\0", caoco::tk_enum::period_, u8".");
	test_single_token(u8"...\0", caoco::tk_enum::ellipsis_, u8"...");

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
#if CAOCO_UT_Tokenizer_NumberAndReal
TEST(CaocoTokenizer_NumberAndReal, CaocoTokenizer_Test) {
	// Specific test for a number followed by an ellipsis

	auto input_vec = caoco::sl::to_u8vec(u8"1234...\0");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();

	EXPECT_EQ(result.size(), 2);
	EXPECT_EQ(result.at(0).type(), caoco::tk_enum::number_literal_);
	EXPECT_EQ(result.at(0).literal(), caoco::sl_u8string(u8"1234"));
	EXPECT_EQ(result.at(1).type(), caoco::tk_enum::ellipsis_);
	EXPECT_EQ(result.at(1).literal(), caoco::sl_u8string(u8"..."));

}
#endif

#if CAOCO_UT_Parser_BasicNode_SingularNodes
TEST(CaocoParser_BasicNode_SingularNodes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_singular_nodes.candi");
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
#if CAOCO_UT_Parser_BasicNode_BasicScopes	
TEST(CaocoParser_BasicNode_BasicScopes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_scopes.candi");
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
#if CAOCO_UT_Parser_BasicNode_StatementScope 
TEST(CaocoParser_BasicNode_StatementScope, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_statementscope.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Single value statement : 1;
	std::cout << "Testing single value statement" << std::endl;
	caoco::parser_scope_result empty_statement = caoco::find_statement(caoco::tk_enum::number_literal_,caoco::tk_enum::eos_,result.cbegin(), result.cend());
	EXPECT_TRUE(empty_statement.valid);

	// statement with multiple tokens: #var a = 1;
	std::cout << "Testing statement with multiple tokens" << std::endl;
	caoco::parser_scope_result multiple_token_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos_, empty_statement.scope_end(), result.cend());
	EXPECT_TRUE(multiple_token_statement.valid);

	// statement with multiple tokens and scopes: #var a = (1;2;3);
	std::cout << "Testing statement with multiple tokens and scopes" << std::endl;
	caoco::parser_scope_result multiple_token_scope_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos_, multiple_token_statement.scope_end(), result.cend());
	EXPECT_TRUE(multiple_token_scope_statement.valid);

	// statement with lists frames and scopes nested in diffrent ways containing end tokens. #var a = 1 + ([ 2 ;3 + {4;5;6}]);
	std::cout << "Testing statement with lists frames and scopes nested in diffrent ways containing end tokens." << std::endl;
	caoco::parser_scope_result complex_statement = caoco::find_statement(caoco::tk_enum::var_, caoco::tk_enum::eos_, multiple_token_scope_statement.scope_end(), result.cend());
	EXPECT_TRUE(complex_statement.valid);
	//EXPECT_TRUE(complex_statement.scope_end() == result.cend() - 1);

	// Test finding an "open" statement which allows for repeated open tokens. ex a = a + a + ([ a ;a + {a;a;a}]);
	std::cout << "Testing statement with lists frames and scopes nested in diffrent ways containing begin and end tokens." << std::endl;
	caoco::parser_scope_result open_statement = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, complex_statement.scope_end(), result.cend());
	EXPECT_TRUE(open_statement.valid);
	EXPECT_TRUE(open_statement.scope_end() == result.cend() - 1);

}
#endif
#if CAOCO_UT_Parser_BasicNode_PrimaryExpression
TEST(CaocoParser_BasicNode_PrimaryExpression, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_primaryexpr.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto found_scope = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, result.cbegin(), result.cend());
	// Test parsing a primary expression using build statement method
	std::cout << "Testing foo;" << std::endl;
	auto px1 = caoco::build_statement(found_scope.scope_begin(), found_scope.contained_end());
	EXPECT_TRUE(px1.has_value());
	print_ast(px1.value());


	// foo + 2
	std::cout << "Testing foo + 2;" << std::endl;
	auto found_scope2 = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, found_scope.scope_end(), result.cend());
	auto px2 = caoco::build_statement(found_scope2.scope_begin(), found_scope2.contained_end());
	EXPECT_TRUE(px2.has_value());
	print_ast(px2.value());

	// foo = 1 + 2;
	std::cout << "Testing foo = 1 + 2;" << std::endl;
	auto found_scope3 = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, found_scope2.scope_end(), result.cend());
	auto px3 = caoco::build_statement(found_scope3.scope_begin(), found_scope3.contained_end());
	EXPECT_TRUE(px3.has_value());
	print_ast(px3.value());

	// Testing period(member access) operator
	std::cout << "Testing foo.bar;" << std::endl;
	auto found_scope4 = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, found_scope3.scope_end(), result.cend());
	auto px4 = caoco::build_statement(found_scope4.scope_begin(), found_scope4.contained_end());
	EXPECT_TRUE(px4.has_value());
	print_ast(px4.value());

	// Testing function call operator ()
	std::cout << "Testing foo.bar();" << std::endl;
	auto found_scope5 = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, found_scope4.scope_end(), result.cend());
	auto px5 = caoco::build_statement(found_scope5.scope_begin(), found_scope5.contained_end());
	EXPECT_TRUE(px5.has_value());
	print_ast(px5.value());

	// Statement with no following binary operator should be invalid and throw an exception.
	std::cout << "Testing foo=; Result should throw." << std::endl;
	auto found_scope6 = caoco::find_open_statement(caoco::tk_enum::alnumus_, caoco::tk_enum::eos_, found_scope5.scope_end(), result.cend());
	EXPECT_ANY_THROW(caoco::build_statement(found_scope6.scope_begin(), found_scope6.contained_end()));

}
#endif
#if CAOCO_UT_Parser_BasicNode_SimpleStatements
TEST(CaocoParser_BasicNode_SimpleStatements, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_simplestatements.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Type definition
	// #type Int = &int;
	std::cout << "Testing #type Int = &int;" << std::endl;
	auto found_scope = caoco::find_open_statement(caoco::tk_enum::type_, caoco::tk_enum::eos_, result.cbegin(), result.cend());
	auto px1 = caoco::ParseDirectiveType()(found_scope.scope_begin(), found_scope.scope_end());
	EXPECT_TRUE(px1.valid());
	print_ast(px1.node());


	// Type alias
	// #type IntAlias = Int;
	std::cout << "Testing #type IntAlias = Int;" << std::endl;
	auto found_scope2 = caoco::find_open_statement(caoco::tk_enum::type_, caoco::tk_enum::eos_, found_scope.scope_end(), result.cend());
	auto px2 = caoco::ParseDirectiveType()(found_scope2.scope_begin(), found_scope2.scope_end());
	EXPECT_TRUE(px2.valid());
	print_ast(px2.node());

	// Type definition with contraints
	// #type IntRange = &int[1 ...10];
	std::cout << "Testing #type IntRange = &int[1 ...10];" << std::endl;
	auto found_scope3 = caoco::find_open_statement(caoco::tk_enum::type_, caoco::tk_enum::eos_, found_scope2.scope_end(), result.cend());
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
#if CAOCO_UT_Parser_BasicNode_Functions
TEST(CaocoParser_BasicNode_Functions, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_function.candi");
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
#if CAOCO_UT_Parser_BasicNode_Classes
TEST(CaocoParser_BasicNode_Classes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_classes.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto empty_class_def_end = test_parsing_functor(
		"Empty Class Definition", caoco::ParseDirectiveClass(), result.cbegin(), result.cend());
	auto class_def_with_members_end = test_parsing_functor(
		"Class Definition with Members", caoco::ParseDirectiveClass(), empty_class_def_end, result.cend());
	auto class_def_with_members_and_methods_end = test_parsing_functor(
		"Class Definition with Members and Methods", caoco::ParseDirectiveClass(), class_def_with_members_end, result.cend());
}
#endif
#if CAOCO_UT_Parser_MinimumProgram 
TEST(CaocoParser_MinimumProgram, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_minimum_program.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto parse_result = caoco::ParsePragmaticBlock()(result.cbegin(), result.cend());
	EXPECT_TRUE(parse_result.valid());
	if (!parse_result.valid()) {
		std::cout << parse_result.error_message() << std::endl;
	}
	print_ast(parse_result.node());
}
#endif

#if CAOCO_UT_Parser_BasicNode_ConditionalStatements
TEST(CaocoParser_ControlFlow, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_conditional.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto if_statement = test_parsing_functor(
		"if statement", caoco::ParseDirectiveIf(), result.cbegin(), result.cend());

	auto if_else_statement = test_parsing_functor("if else statement", caoco::ParseDirectiveIf(), if_statement, result.cend());

	auto if_elif_else = test_parsing_functor("if elif else statement", caoco::ParseDirectiveIf(), if_else_statement, result.cend());

}
#endif

#if CAOCO_UT_Parser_BasicNode_Loops
TEST(CaocoParser_Loops, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_loops.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto while_loop = test_parsing_functor(
		"while loop", caoco::ParseDirectiveWhile(), result.cbegin(), result.cend());

	//auto for_loop = test_parsing_functor("for loop", caoco::ParseDirectiveFor(), while_loop, result.cend());

	//auto switch_statement = test_parsing_functor("switch statement", caoco::ParseDirectiveSwitch(), for_loop, result.cend());

}
#endif

#if CAOCO_UT_ConstantEvaluator_Literals
TEST(CaocoConstantEvaluator_Literals, CaocoConstantEvaluator_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_ceval_literals.candi");
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
#if CAOCO_UT_ConstantEvaluator_Operators
TEST(CaocoConstantEvaluator_Operators, CaocoConstantEvaluator_Operators) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_ceval_operators.candi");
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
#if CAOCO_UT_ConstantEvaluator_VariableDeclaration
TEST(CaocoConstantEvaluator_VariableDeclaration, CaocoConstantEvaluator_VariableDeclaration) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_ceval_vardecl.candi");
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
#if CAOCO_UT_ConstantEvaluator_Structs
TEST(CaocoConstantEvaluator_Structs, CaocoConstantEvaluator_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_ceval_structs.candi");
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
#if CAOCO_UT_ConstantEvaluator_FreeFunctions
TEST(CaocoConstantEvaluator_FreeFunctions, CaocoConstantEvaluator_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_ceval_free_functions.candi");
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

#if CAOCO_UT_Program_BasicProgram
TEST(CaocoProgram_BasicProgram, CaocoProgram_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_program_basic.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	for(auto& token : result){
		std::cout << " |" << token_type_to_string(token.type());
	}

	// Parse the program
	auto parse_result = caoco::parse_program(result.cbegin(), result.cend());
	//EXPECT_TRUE(parse_result.valid());
	//if (!parse_result.valid()) {
	//	std::cout << parse_result.error_message() << std::endl;
	//}
	print_ast(parse_result);
}

#endif
