#include "pch.h"
#include "unit_test_util.hpp"

#define CAOCO_TEST_ALL 1
#define CAOCO_TEST_NONE 0
#define CAOCO_TEST_TOKENIZER 1

#if CAOCO_TEST_ALL
#define CAOCO_UT_V2Parser_SingleNodes 1
#define CAOCO_UT_V2Parser_ValueExpressions 1
#define CAOCO_UT_V2Parser_ValueStatementREPL 0


#define CAOCO_UT_V2Parser_ValueStatements 1
#define CAOCO_UT_V2Parser_BasicScopeFinder 0
#define CAOCO_UT_V2Parser_ListScopeFinder 0
#define CAOCO_UT_V2Parser_StatementScopeFinder 0

#define CAOCO_UT_V2Parser_TypeAlias 0
#define CAOCO_UT_V2Parser_VariableDeclaration 0
#define CAOCO_UT_V2Parser_Functions 0
#define CAOCO_UT_V2Parser_Classes 0
#define CAOCO_UT_V2Parser_PragmaticBlock 0
#define CAOCO_UT_V2Parser_FunctionalBlock 0
#define CAOCO_UT_V2Parser_ConditionalStatements 0
#define CAOCO_UT_V2Parser_SwitchStatement 0
#define CAOCO_UT_V2Parser_WhileLoop 0
#define CAOCO_UT_V2Parser_ForLoop 0

#define CAOCO_UT_V2Parser_ReturnStatement 0

#define CAOCO_UT_V2Parser_MinimumProgram 0
#define CAOCO_UT_V2Parser_BasicProgram 0

#define CAOCO_UT_V2ParserUtils_SeperatedList 0

#define CAOCO_UT_Preprocessor_Include 0
#endif


#if CAOCO_TEST_NONE
#define CAOCO_UT_Preprocessor_Include 1
#endif

#if CAOCO_TEST_TOKENIZER 
#define CAOCO_TEST_TOKENIZER_Keywords 1
#define CAOCO_TEST_TOKENIZER_KeywordsMixedShouldThrow 1
#define CAOCO_TEST_TOKENIZER_KeywordsDirectiveReportEarlyMisspell 1
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#if CAOCO_TEST_TOKENIZER_Keywords
TEST(ut_Tokenizer_Keywords, ut_Tokenizer) {
	// non-directive keywords
	auto input_vec1 = caoco::sl::to_u8vec(u8"include macro enter start type var class obj private\
 public func const static if else elif while for on break continue return print none int uint real\
 byte bit str\0");
	auto expected_result1 = caoco::tk_vector({
		{caoco::tk_enum::include_},
		{caoco::tk_enum::macro_},
		{caoco::tk_enum::enter_},
		{caoco::tk_enum::start_},
		{caoco::tk_enum::type_},
		{caoco::tk_enum::var_},
		{caoco::tk_enum::class_},
		{caoco::tk_enum::obj_},
		{caoco::tk_enum::private_},
		{caoco::tk_enum::public_},
		{caoco::tk_enum::func_},
		{caoco::tk_enum::const_},
		{caoco::tk_enum::static_},
		{caoco::tk_enum::if_},
		{caoco::tk_enum::else_},
		{caoco::tk_enum::elif_},
		{caoco::tk_enum::while_},
		{caoco::tk_enum::for_},
		{caoco::tk_enum::on_},
		{caoco::tk_enum::break_},
		{caoco::tk_enum::continue_},
		{caoco::tk_enum::return_},
		{caoco::tk_enum::print_},
		{caoco::tk_enum::none_literal_},
		{caoco::tk_enum::aint_},
		{caoco::tk_enum::auint_},
		{caoco::tk_enum::areal_},
		{caoco::tk_enum::aoctet_},
		{caoco::tk_enum::abit_},
		{caoco::tk_enum::astr_}
		});
	auto exp_result1 = caoco::tokenizer(input_vec1.cbegin(), input_vec1.cend())();
	if(!exp_result1.valid()){
		std::cout << exp_result1.error_message() << std::endl;
	}
	auto result1 = exp_result1.extract();
	for (auto& tk : result1) {
		std::cout << tk.type_to_string() << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < result1.size(); ++i) {
		EXPECT_EQ(result1[i].type(), expected_result1[i].type());
		if (result1[i].type() != expected_result1[i].type()) {
			std::cout << "Tokenization result expected keyword token: " << expected_result1[i].type_to_string()
				<< " Got: " << result1[i].type_to_string() << std::endl;
		}
	}


	// directive keywords
	auto input_vec2 = caoco::sl::to_u8vec(u8"#include #macro #enter #start #type #var #class #obj \
#private #public #func #const #static #if #else #elif #while #for #on #break #continue #return \
#print #none #int #uint #real #byte #bit #str\0");
	auto expected_result2 = caoco::tk_vector({
		{caoco::tk_enum::include_},
		{caoco::tk_enum::macro_},
		{caoco::tk_enum::enter_},
		{caoco::tk_enum::start_},
		{caoco::tk_enum::type_},
		{caoco::tk_enum::var_},
		{caoco::tk_enum::class_},
		{caoco::tk_enum::obj_},
		{caoco::tk_enum::private_},
		{caoco::tk_enum::public_},
		{caoco::tk_enum::func_},
		{caoco::tk_enum::const_},
		{caoco::tk_enum::static_},
		{caoco::tk_enum::if_},
		{caoco::tk_enum::else_},
		{caoco::tk_enum::elif_},
		{caoco::tk_enum::while_},
		{caoco::tk_enum::for_},
		{caoco::tk_enum::on_},
		{caoco::tk_enum::break_},
		{caoco::tk_enum::continue_},
		{caoco::tk_enum::return_},
		{caoco::tk_enum::print_},
		{caoco::tk_enum::none_literal_},
		{caoco::tk_enum::aint_},
		{caoco::tk_enum::auint_},
		{caoco::tk_enum::areal_},
		{caoco::tk_enum::aoctet_},
		{caoco::tk_enum::abit_},
		{caoco::tk_enum::astr_}
		});
	auto exp_result2 = caoco::tokenizer(input_vec2.cbegin(), input_vec2.cend())();

	if (!exp_result2.valid()) {
		std::cout << exp_result2.error_message() << std::endl;
	}
	auto result2 = exp_result2.extract();
	for (auto& tk : result2) {
		std::cout << tk.type_to_string() << " ";
	}
	for (auto& tk : result2) {
		std::cout << tk.type_to_string() << " ";
	}
	std::cout << std::endl;
	for (size_t i = 0; i < result2.size(); ++i) {
		EXPECT_EQ(result2[i].type(), expected_result2[i].type());
		if (result2[i].type() != expected_result2[i].type()) {
			std::cout << "Tokenization result expected keyword token: " << expected_result2[i].type_to_string()
				<< " Got: " << result2[i].type_to_string() << std::endl;
		}
	}
}
#endif

#if CAOCO_TEST_TOKENIZER_KeywordsMixedShouldThrow
TEST(ut_Tokenizer_KeywordsMixedShouldThrow, ut_Tokenizer) {
	auto input_vec = caoco::sl::to_u8vec(u8"#include #macro #enter #start #type #var #class #obj #private\
 public #func #const #static #if #else #elif #while #for #on #break #continue #return #print #none #int #uint #real\
 #byte #bit #str");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
	EXPECT_FALSE(result.valid());
	if(!result.valid()){
		std::cout << result.error_message() << std::endl;
	}


	auto input_vec2 = caoco::sl::to_u8vec(u8"include macro enter start type var class obj private\
 #public func const static if else elif while for on break continue return print none int uint real\
 byte bit str\0");

	result = caoco::tokenizer(input_vec2.cbegin(), input_vec2.cend())();
	EXPECT_FALSE(result.valid());
	if (!result.valid()) {
		std::cout << result.error_message() << std::endl;
	}
}
#endif

#if CAOCO_TEST_TOKENIZER_KeywordsDirectiveReportEarlyMisspell 
TEST(ut_Tokenizer_KeywordsDirectiveReportEarlyMisspell, ut_Tokenizer) {
	auto input_vec = caoco::sl::to_u8vec(u8"#inclde");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
	EXPECT_FALSE(result.valid());
	if (!result.valid()) {
		std::cout << result.error_message() << std::endl;
	}
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#if CAOCO_UT_V2Parser_ValueExpressions
TEST(ut_Parser_Expression_SingleOperand, LiteralInt) {
	using namespace caoco;

	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralInt",
		astnode(astnode_enum::number_literal_, u8"1"),
		u8"1\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, LiteralReal) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralReal",
		astnode(astnode_enum::real_literal_, u8"1.1"),
		u8"1.1\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, LiteralUint) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralUint",
		astnode(astnode_enum::unsigned_literal_, u8"1u"),
		u8"1u\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, LiteralBit) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralBit",
		astnode(astnode_enum::bit_literal_, u8"1b"),
		u8"1b\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, LiteralString) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralString",
		astnode(astnode_enum::string_literal_, u8"'hello'"),
		u8"'hello'\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, LiteralByte) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralByte",
		astnode(astnode_enum::octet_literal_, u8"1c"),
		u8"1c\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, LiteralNone) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_LiteralNone",
		astnode(astnode_enum::none_literal_, u8"#none"),
		u8"#none\0"
	));
}

TEST(ut_Parser_Expression_SingleOperand, ValueInBrackets) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperand_ValueInBrackets",
		astnode(astnode_enum::number_literal_, u8"1"),
		u8"(1)\0"
	));
}

TEST(ut_Parser_Expression_SingleOperation, Binary) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperation_BinaryAddition",
		astnode(astnode_enum::addition_, u8"+",
			astnode(astnode_enum::number_literal_, u8"1"),
			astnode(astnode_enum::number_literal_, u8"1")
		),
		u8"1 + 1\0"
	));
}

TEST(ut_Parser_Expression_SingleOperation, EmptyScopeIsAnError) {
	auto input_vec = caoco::sl::to_u8vec(u8"()\0");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
	auto parse_result = caoco::expression_split_parse(caoco::tk_cursor(result.expected().cbegin(), result.expected().cend()));
	EXPECT_FALSE(parse_result.valid());
}

TEST(ut_Parser_Expression_SingleOperation, FunctionCall) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperation_FunctionCall",
		astnode(astnode_enum::function_call_, u8"()",
			{ astnode(astnode_enum::alnumus_, u8"foo"),
			astnode(astnode_enum::arguments_,u8"()") }),
		u8"foo()\0"
	));
}

TEST(ut_Parser_Expression_SingleOperation, Unary) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"SingleOperation_UnaryNegation",
		astnode(astnode_enum::negation_, u8"!",
			{ astnode(astnode_enum::number_literal_, u8"1") }),
		u8"!1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation, BinaryDiffPriority) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_BinaryDiffPriority",
		astnode(astnode_enum::addition_, u8"+",
			{ astnode(astnode_enum::number_literal_, u8"1"),
			astnode(astnode_enum::multiplication_, u8"*",
				{ astnode(astnode_enum::number_literal_, u8"1"),
				astnode(astnode_enum::number_literal_, u8"1") }) }),
		u8"1 + 1 * 1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,LogicalOperators) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_LogicalOperators",
		astnode(astnode_enum::logical_AND_, u8"&&",
			{ astnode(astnode_enum::logical_OR_, u8"||",
				{ astnode(astnode_enum::alnumus_, u8"a"),
				astnode(astnode_enum::alnumus_, u8"b") }),
			astnode(astnode_enum::alnumus_, u8"c") }),
		u8"a || b && c\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,Scopes) {
	//[multiplication_] * |
	//	[addition_] + |
	//	[number_literal_] 1 |
	//	[number_literal_] 1 |
	//	[number_literal_] 1 |
	using namespace caoco;
	auto expected = astnode(astnode_enum::multiplication_, u8"*",
		{ astnode(astnode_enum::addition_, u8"+",
			{ astnode(astnode_enum::number_literal_, u8"1"),
			astnode(astnode_enum::number_literal_, u8"1") }),
		astnode(astnode_enum::number_literal_, u8"1") });
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_Scopes",
		expected,
		u8"(1 + 1) * 1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,AssingmentIsRightAssoc) {
	//a = b = c is parsed as a = (b = c), and not as (a = b) = c because of right-to-left associativity of assignment
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_AssingmentIsRightAssoc",
		astnode(astnode_enum::simple_assignment_, u8"=",
			{ astnode(astnode_enum::alnumus_, u8"a"),
			astnode(astnode_enum::simple_assignment_, u8"=",
				{ astnode(astnode_enum::alnumus_, u8"b"),
				astnode(astnode_enum::alnumus_, u8"c") }) }),
		u8"a = b = c\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,SumIsLeftAssoc) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_SumIsLeftAssoc",
		astnode(astnode_enum::subtraction_, u8"-",
			{ astnode(astnode_enum::addition_, u8"+",
				{ astnode(astnode_enum::alnumus_, u8"a"),
				astnode(astnode_enum::alnumus_, u8"b") }),
			astnode(astnode_enum::alnumus_, u8"c") }),
		u8"a + b - c\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,MemberAccessIsLeftAssoc) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_MemberAccessIsLeftAssoc",
		astnode(astnode_enum::period_, u8".",
			{ astnode(astnode_enum::period_, u8".",
				{ astnode(astnode_enum::alnumus_, u8"a"),
				astnode(astnode_enum::alnumus_, u8"b") }),
			astnode(astnode_enum::alnumus_, u8"c") }),
		u8"a.b.c\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,BinaryAfterUnaryIsAnError) {

	auto input_vec = caoco::sl::to_u8vec(u8"!+1\0");
	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
	auto parse_result = caoco::expression_split_parse(caoco::tk_cursor(result.expected().cbegin(), result.expected().cend()));

	EXPECT_FALSE(parse_result.valid());
}

TEST(ut_Parser_Expression_ChainOperation,UnaryRepeated) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_UnaryRepeated",
		astnode(astnode_enum::negation_, u8"!",
			{ astnode(astnode_enum::negation_, u8"!",
				{ astnode(astnode_enum::number_literal_, u8"1") }) }),
		u8"!!1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,UnaryThenBinary) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_UnaryThenBinary",
		astnode(astnode_enum::addition_, u8"+",
			{ astnode(astnode_enum::negation_, u8"!",
				{ astnode(astnode_enum::number_literal_, u8"1") }),
			astnode(astnode_enum::number_literal_, u8"1") }),
		u8"!1 + 1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,UnaryThenHigherPriority) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_UnaryThenHigherPriority",
		astnode(astnode_enum::multiplication_, u8"*",
			{ astnode(astnode_enum::negation_, u8"!",
				{ astnode(astnode_enum::number_literal_, u8"1") }),
			astnode(astnode_enum::number_literal_, u8"1") }),
		u8"!1 * 1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,UnaryAfterBinary) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_UnaryAfterBinary",
		astnode(astnode_enum::addition_, u8"+",
			{ astnode(astnode_enum::number_literal_, u8"1"),
			astnode(astnode_enum::negation_, u8"!",
				{ astnode(astnode_enum::number_literal_, u8"1") }) }),
		u8"1 + !1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,UnaryThenFunctionCall) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_UnaryThenFunctionCall",
		astnode(astnode_enum::negation_, u8"!",
			{ astnode(astnode_enum::function_call_, u8"()",
				{ astnode(astnode_enum::alnumus_, u8"foo"),
				astnode(astnode_enum::arguments_, u8"()") }) }),
		u8"!foo()\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,FunctionCallThenBinary) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_FunctionCallThenBinary",
		astnode(astnode_enum::addition_, u8"+",
			{ astnode(astnode_enum::function_call_, u8"()",
				{ astnode(astnode_enum::alnumus_, u8"foo"),
				astnode(astnode_enum::arguments_, u8"()") }),
			astnode(astnode_enum::number_literal_, u8"1") }),
		u8"foo() + 1\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,BinaryDotOperatorThenFunctionCall) {
	using namespace caoco;
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_BinaryDotOperatorThenFunctionCall",
		astnode(astnode_enum::function_call_, u8"()",
			{ astnode(astnode_enum::period_, u8".",
				{ astnode(astnode_enum::alnumus_, u8"foo"),
				astnode(astnode_enum::alnumus_, u8"bar") }),
			astnode(astnode_enum::arguments_, u8"()") }),
		u8"foo.bar()\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,BinaryThenFunctionCall) {
	//[addition_] + |
	//	[number_literal_] 1 |
	//	[function_call_]() |
	//		[alnumus_] foo |
	//		[arguments_] |
	using namespace caoco;
	auto expected = astnode(astnode_enum::addition_, u8"+",
		{ astnode(astnode_enum::number_literal_, u8"1"),
		astnode(astnode_enum::function_call_, u8"()",
			{ astnode(astnode_enum::alnumus_, u8"foo"),
			astnode(astnode_enum::arguments_, u8"()") }) });
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_BinaryThenFunctionCall",
		expected,
		u8"1 + foo()\0"
	));
}

TEST(ut_Parser_Expression_ChainOperation,MemberAccessWithFunctionCall) {
	// a.b().c is parsed as (a.b()).c and not as a.(b().c) because of left-to-right associativity of member access.
	using namespace caoco;
	// create ast for a.b().c
	//[period_] .|
	//[function_call_] () |
	//	[period_] . |
	//	[alnumus_] a |
	//	[alnumus_] b |
	//	[arguments_]().c |
	//	[alnumus_] c |
	astnode expected = astnode(astnode_enum::period_, u8".",
		{ astnode(astnode_enum::function_call_, u8"()",
			{ astnode(astnode_enum::period_, u8".",
				{ astnode(astnode_enum::alnumus_, u8"a"),
				astnode(astnode_enum::alnumus_, u8"b") }),
			astnode(astnode_enum::arguments_, u8"()") }),
		astnode(astnode_enum::alnumus_, u8"c") });
	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ChainOperation_MemberAccessWithFunctionCall",
		expected,
		u8"a.b().c\0"
	));
}

TEST(ut_Parser_Expression_ComplexOperation, Operation) {
	//addition_] + |
	//[function_call_]() |
	//	  [period_] . |
	//		[alnumus_] foo |
	//		[alnumus_] bar |
	//	  [arguments_]() |
	//	[multiplication_] * |
	//	  [number_literal_] 1 |
	//	  [number_literal_] 1 |
	using namespace caoco;
	auto expected = astnode(astnode_enum::addition_, u8"+",
		{ astnode(astnode_enum::function_call_, u8"()",
			{ astnode(astnode_enum::period_, u8".",
				{ astnode(astnode_enum::alnumus_, u8"foo"),
				astnode(astnode_enum::alnumus_, u8"bar") }),
			astnode(astnode_enum::arguments_, u8"()") }),
		astnode(astnode_enum::multiplication_, u8"*",
			{ astnode(astnode_enum::number_literal_, u8"1"),
			astnode(astnode_enum::number_literal_, u8"1") }) });

	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ComplexOperation",
		expected,
		u8"foo.bar() + 1 * 1\0"
	));
}

TEST(ut_Parser_Expression_ComplexOperation, OperationWithScopes) {
	//[multiplication_] * |
	//	[addition_] + |
	//		[function_call_]() |
	//			[period_] . |
	//				[alnumus_] foo |
	//				[alnumus_] bar |
	//			[arguments_]() |
	//		[number_literal_] 1 |
	//	[number_literal_] 1 |
	using namespace caoco;
	auto expected = astnode(astnode_enum::multiplication_, u8"*",
		{ astnode(astnode_enum::addition_, u8"+",
			{ astnode(astnode_enum::function_call_, u8"()",
				{ astnode(astnode_enum::period_, u8".",
					{ astnode(astnode_enum::alnumus_, u8"foo"),
					astnode(astnode_enum::alnumus_, u8"bar") }),
				astnode(astnode_enum::arguments_, u8"()") }),
			astnode(astnode_enum::number_literal_, u8"1") }),
		astnode(astnode_enum::number_literal_, u8"1") });


	EXPECT_TRUE(test_and_compare_split_parsing_function_from_u8(
		"ComplexOperationWithScopes",
		expected,
		u8"(foo.bar() + 1) * 1\0"
	));
}
#endif

#if CAOCO_UT_V2Parser_ValueStatementREPL
TEST(ut_repl, ut_Parser) {
	while (true) {
		std::cout << "Caoco Expression AST REPL" << std::endl;
		std::string input;
		std::cout << "Please enter an value statement or #exit to exit: ";
		std::getline(std::cin, input);
		if (input == "#exit") break;
		else {
			auto input_vec = caoco::sl::to_u8vec(input.c_str());
			auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
			if(!result.valid()){
				std::cout << result.error_message() << std::endl;
			}
			else {
				auto exp_result = result.expected();
				auto parse_result = caoco::parse_value_statement(exp_result.cbegin(), exp_result.cend());
				if (!parse_result.valid()) std::cout << parse_result.error_message() << std::endl;
				else print_ast(parse_result.expected());
			}
		}
	}
}
#endif

#if CAOCO_UT_V2Parser_ValueStatements
TEST(ut_Parser_ValueStatements, ut_Parser) {

	EXPECT_TRUE(test_and_compare_parsing_function_from_u8(
		"PrimaryExpr",
		&caoco::parse_value_statement,
		caoco::astnode(caoco::astnode_enum::alnumus_, u8"foo"),
		u8"foo;"
	));

	// foo + 2
	EXPECT_TRUE(test_and_compare_parsing_function_from_u8(
		"BinaryExpr",
		&caoco::parse_value_statement,
		caoco::astnode(caoco::astnode_enum::addition_, u8"+",
			{ caoco::astnode(caoco::astnode_enum::alnumus_, u8"foo"),
			caoco::astnode(caoco::astnode_enum::number_literal_, u8"2") }),
		u8"foo + 2;"
	));

	// foo = 1 + 2;
	EXPECT_TRUE(test_and_compare_parsing_function_from_u8(
		"AssignmentExpr",
		&caoco::parse_value_statement,
		caoco::astnode(caoco::astnode_enum::simple_assignment_, u8"=",
			{ caoco::astnode(caoco::astnode_enum::alnumus_, u8"foo"),
			caoco::astnode(caoco::astnode_enum::addition_, u8"+",
				{ caoco::astnode(caoco::astnode_enum::number_literal_, u8"1"),
				caoco::astnode(caoco::astnode_enum::number_literal_, u8"2") }) }),
		u8"foo = 1 + 2;"
	));

	// Testing period(member access) operator
	EXPECT_TRUE(test_and_compare_parsing_function_from_u8(
		"PeriodOperator",
		&caoco::parse_value_statement,
		caoco::astnode(caoco::astnode_enum::period_, u8".",
			{ caoco::astnode(caoco::astnode_enum::alnumus_, u8"foo"),
			caoco::astnode(caoco::astnode_enum::alnumus_, u8"bar") }),
		u8"foo.bar;"
	));

	// Testing function call operator ()
	EXPECT_TRUE(test_and_compare_parsing_function_from_u8(
		"FunctionCallOperator",
		&caoco::parse_value_statement,
		caoco::astnode(caoco::astnode_enum::function_call_, u8"()",
			{ caoco::astnode(caoco::astnode_enum::alnumus_, u8"foo"),
			caoco::astnode(caoco::astnode_enum::arguments_, u8"()") }),
		u8"foo();"
	));

	// Statement with no following binary operator should be invalid and throw an exception.
	std::cout << "Testing foo=; Result should be invalid." << std::endl;
	EXPECT_FALSE(test_and_compare_parsing_function_from_u8(
	"InvalidStatement",
	&caoco::parse_value_statement,
	caoco::astnode(caoco::astnode_enum::alnumus_, u8"foo"),
	u8"foo=;"
	));

}
#endif

#if CAOCO_UT_V2Parser_SingleNodes
//TEST(ut_Parser_Arguments, ut_Parser) {
//	auto input_vec = caoco::sl::to_u8vec(u8"(1, 1, 3)\0");
//	auto result = caoco::tokenizer(input_vec.cbegin(), input_vec.cend())();
//	auto parse_result = caoco::parse_arguments(result.cbegin(), result.cend());
//	if (!parse_result.valid()) std::cout << parse_result.error_message() << std::endl;
//	else print_ast(parse_result.expected());
//}

TEST(ut_Parser_SinguleNodes, ut_Parser) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_singular_nodes.candi");
	auto lex_result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	if (!lex_result.valid()) {
		std::cout << lex_result.error_message() << std::endl;
	}
	else {
		auto result = lex_result.expected();

		// C& Special Objects
		auto atype_end = test_parsing_function("&type:", &caoco::parse_cso_type, result.cbegin(), result.cend());
		auto avalue_end = test_parsing_function("&value:", &caoco::parse_cso_value, atype_end, result.cend());
		auto aidentity_end = test_parsing_function("&identity:", &caoco::parse_cso_identity, avalue_end, result.cend());
		auto aint_end = test_parsing_function("&int:", &caoco::parse_cso_int, aidentity_end, result.cend());
		auto aint_positive_range_end = test_parsing_function("&int[positive_range]:", &caoco::parse_cso_int, aint_end, result.cend());
		auto aint_negative_range_end = test_parsing_function("&int[negative_range]:", &caoco::parse_cso_int, aint_positive_range_end, result.cend());
		auto aint_lhs_negative_range_end = test_parsing_function("&int[lhs_negative_range]:", &caoco::parse_cso_int, aint_negative_range_end, result.cend());
		auto aint_rhs_negative_range_end = test_parsing_function("&int[rhs_negative_range]:", &caoco::parse_cso_int, aint_lhs_negative_range_end, result.cend());
		auto auint_end = test_parsing_function("&uint:", &caoco::parse_cso_uint, aint_rhs_negative_range_end, result.cend());
		auto auint_positive_range_end = test_parsing_function("&uint[positive_range]:", &caoco::parse_cso_uint, auint_end, result.cend());
		auto areal_end = test_parsing_function("&real:", &caoco::parse_cso_real, auint_positive_range_end, result.cend());
		auto aoctet_end = test_parsing_function("&octet:", &caoco::parse_cso_octet, areal_end, result.cend());
		auto abit_end = test_parsing_function("&bit:", &caoco::parse_cso_bit, aoctet_end, result.cend());
		//auto apointer_end = test_parsing_function("&pointer[mytype]:", caoco::ParseCsoPointer(), abit_end, result.cend());
		//auto apointer_int_end = test_parsing_function("&pointer[&int]:", caoco::ParseCsoPointer(), apointer_end, result.cend());
		//auto aarray_end = test_parsing_function("&array[mytype, 42]:", caoco::ParseCsoArray(), apointer_int_end, result.cend());
		//auto aarray_int_end = test_parsing_function("&array[&int, 42]:", caoco::ParseCsoArray(), aarray_end, result.cend());
		//auto aarray_int_range_end = test_parsing_function("&array[&int[1 ...42], 42]:", caoco::ParseCsoArray(), aarray_int_end, result.cend());

		// Literals and Operands.
		auto string_literal_end = test_parsing_function("'string literal':", &caoco::parse_string_literal, abit_end, result.cend());
		auto string_literal_with_escape_end = test_parsing_function("'string literal with \\'':", &caoco::parse_string_literal, string_literal_end, result.cend());
		auto number_literal_end = test_parsing_function("number literal:", &caoco::parse_number_literal, string_literal_with_escape_end, result.cend());
		auto real_literal_end = test_parsing_function("real literal:", &caoco::parse_real_literal, number_literal_end, result.cend());
		auto alnumus_end = test_parsing_function("alnumus:", &caoco::parse_alnumus_literal, real_literal_end, result.cend());
		// TODO: test bit, octet and unsigned literals
	}
}
#endif

#if CAOCO_UT_V2Parser_BasicScopeFinder 
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

#if CAOCO_UT_V2Parser_ListScopeFinder 
TEST(CaocoParser_BasicNode_ListScopes, CaocoParser_Test) {
	auto source_file = caoco::sl::to_char8_vector("{}{a}{{}}{({})[{}]{}}");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// empty list
	std::cout << "Testing empty list:";
	caoco::parser_scope_result empty_list = caoco::find_list_scope(result.cbegin(), result.cend());
	EXPECT_TRUE(empty_list.valid);
	for (auto i = empty_list.scope_begin(); i != empty_list.scope_end(); i++) std::cout << i->literal_str();
	std::cout << std::endl;

	// list with 1 element
	std::cout << "Testing list with 1 element:";
	caoco::parser_scope_result list_with_1_element = caoco::find_list_scope(empty_list.scope_end(), result.cend());
	EXPECT_TRUE(list_with_1_element.valid);
	for (auto i = list_with_1_element.scope_begin(); i != list_with_1_element.scope_end(); i++) std::cout << i->literal_str();
	std::cout << std::endl;

	// double list
	std::cout << "Testing double list:";
	caoco::parser_scope_result double_list = caoco::find_list_scope(list_with_1_element.scope_end(), result.cend());
	EXPECT_TRUE(double_list.valid);
	for (auto i = double_list.scope_begin(); i != double_list.scope_end(); i++) std::cout << i->literal_str();
	std::cout << std::endl;

	// complex list
	std::cout << "Testing complex list:";
	caoco::parser_scope_result complex_list = caoco::find_list_scope(double_list.scope_end(), result.cend());
	EXPECT_TRUE(complex_list.valid);
	for (auto i = complex_list.scope_begin(); i != complex_list.scope_end(); i++) std::cout << i->literal_str();
	std::cout << std::endl;
	

}
#endif

#if CAOCO_UT_V2Parser_StatementScopeFinder
TEST(CaocoParser_BasicNode_StatementScope, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_statementscope.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Single value statement : 1;
	std::cout << "Testing single value statement" << std::endl;
	caoco::parser_scope_result empty_statement = caoco::find_statement(caoco::tk_enum::number_literal_, caoco::tk_enum::eos_, result.cbegin(), result.cend());
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


#if CAOCO_UT_V2Parser_TypeAlias
TEST(ut_Parser_TypeAlias, ut_Parser) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_typealias.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();


	// Type alias
	// #type IntAlias = Int;
	std::cout << "Testing #type IntAlias = Int;" << std::endl;
	auto px2 = caoco::parse_directive_type(result.cbegin(), result.cend());
	EXPECT_TRUE(px2.valid());
	if(px2.valid()) print_ast(px2.expected());
	else std::cout << px2.error_message() << std::endl;


	// Type definition
	// #type Int = &int;
	std::cout << "Testing #type Int = &int;" << std::endl;
	auto px1 = caoco::parse_directive_type(px2.always(), result.cend());
	EXPECT_TRUE(px1.valid());
	if (px1.valid()) print_ast(px1.expected());
	else std::cout << px1.error_message() << std::endl;


	// Type definition with contraints
	// #type IntRange = &int[1 ...10];
	std::cout << "Testing #type IntRange = &int[1 ...10];" << std::endl;
	auto px3 = caoco::parse_directive_type(px1.always(), result.cend());
	EXPECT_TRUE(px3.valid());
	if (px3.valid()) print_ast(px3.expected());
	else std::cout << px3.error_message() << std::endl;

}
#endif

#if CAOCO_UT_V2Parser_VariableDeclaration
TEST(ut_Parser_VariableDeclaration, ut_Parser) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_variabledecl.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Anon Var Decl
	// #var foo;
	std::cout << "Testing #var foo;" << std::endl;
	auto anon_var_decl = caoco::parse_directive_var(result.begin(), result.cend());
	EXPECT_TRUE(anon_var_decl.valid());
	if(anon_var_decl.valid()) print_ast(anon_var_decl.expected());
	else std::cout << anon_var_decl.error_message() << std::endl;

	// Anon Var Decl Assingment
	// #var foo = 1;
	std::cout << "Testing #var foo = 1;" << std::endl;
	auto anon_var_decl_assign = caoco::parse_directive_var(anon_var_decl.always(), result.cend());
	EXPECT_TRUE(anon_var_decl_assign.valid());
	if(anon_var_decl_assign.valid()) print_ast(anon_var_decl_assign.expected());
	else std::cout << anon_var_decl_assign.error_message() << std::endl;	

	// Complex Anon Var Decl Assingment
	// #var foo = 1 + c * (3 / 4);
	std::cout << "Testing #var foo = 1 + c * (3 / 4);" << std::endl;
	auto anon_var_decl_assign_complex = caoco::parse_directive_var(anon_var_decl_assign.always(), result.cend());
	EXPECT_TRUE(anon_var_decl_assign_complex.valid());
	if(anon_var_decl_assign_complex.valid()) print_ast(anon_var_decl_assign_complex.expected());
	else std::cout << anon_var_decl_assign_complex.error_message() << std::endl;

	// Type Contrained Var Decl
	// #var [Int] foo;
	std::cout << "Testing #var [Int] foo;" << std::endl;
	auto type_constrained_var_decl = caoco::parse_directive_var(anon_var_decl_assign_complex.always(), result.cend());
	EXPECT_TRUE(type_constrained_var_decl.valid());
	if(type_constrained_var_decl.valid()) print_ast(type_constrained_var_decl.expected());
	else std::cout << type_constrained_var_decl.error_message() << std::endl;

	// Type Contrained Var Decl Assingment
	// #var [Int] foo = 1;
	std::cout << "Testing #var [Int] foo = 1;" << std::endl;
	auto type_constrained_var_decl_assign = caoco::parse_directive_var(type_constrained_var_decl.always(), result.cend());
	EXPECT_TRUE(type_constrained_var_decl_assign.valid());
	if(type_constrained_var_decl_assign.valid()) print_ast(type_constrained_var_decl_assign.expected());
	else std::cout << type_constrained_var_decl_assign.error_message() << std::endl;


	// Complex Type Constrined Var Decl Assingment
	// #var [Int] foo = 1 + c * (3 / 4);
	std::cout << "Testing #var [Int] foo = 1 + c * (3 / 4);" << std::endl;
	auto type_constrained_var_decl_assign_complex = caoco::parse_directive_var(type_constrained_var_decl_assign.always(), result.cend());
	EXPECT_TRUE(type_constrained_var_decl_assign_complex.valid());
	if(type_constrained_var_decl_assign_complex.valid()) print_ast(type_constrained_var_decl_assign_complex.expected());
	else std::cout << type_constrained_var_decl_assign_complex.error_message() << std::endl;
}
#endif

#if CAOCO_UT_V2Parser_Functions
TEST(ut_Parser_Functions, ut_Parser) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_function.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Shorthand Void Arg Method <#func> <alnumus> <functional_block>
	std::cout << "Testing #func foo {};" << std::endl;
	auto shorthand_void_arg_method = caoco::parse_directive_func(result.cbegin(), result.cend());
	EXPECT_TRUE(shorthand_void_arg_method.valid());
	if (!shorthand_void_arg_method.valid()) {
		std::cout << shorthand_void_arg_method.error_message() << std::endl;
	}
	else 
		print_ast(shorthand_void_arg_method.expected());

	// Unconstrained Method Definition <#func> <alnumus> <arguments> <functional_block>
	std::cout << "Testing #func foo (a, b, c) {};" << std::endl;
	auto unconstrained_method = caoco::parse_directive_func(shorthand_void_arg_method.always(), result.cend());
	EXPECT_TRUE(unconstrained_method.valid());
	if (!unconstrained_method.valid()) {
		std::cout << unconstrained_method.error_message() << std::endl;
	}
	else
	print_ast(unconstrained_method.expected());

	// Unconstrained Method Definition w/ no Args
	std::cout << "Testing #func foo() {};" << std::endl;
	auto unconstrained_method_no_args = caoco::parse_directive_func(unconstrained_method.always(), result.cend());
	EXPECT_TRUE(unconstrained_method_no_args.valid());
	if (!unconstrained_method_no_args.valid()) {
		std::cout << unconstrained_method_no_args.error_message() << std::endl;
	}
	else
	print_ast(unconstrained_method_no_args.expected());

	//// Constrained Shorthand Void Arg Method #func [&int] foo {};
	//std::cout << "Testing #func [&int] foo {};" << std::endl;
	//auto constrained_shorthand_void_arg_method = caoco::parse_directive_func(unconstrained_method_no_args.always(), result.cend());
	//EXPECT_TRUE(constrained_shorthand_void_arg_method.valid());
	//if (!constrained_shorthand_void_arg_method.valid()) {
	//	std::cout << constrained_shorthand_void_arg_method.error_message() << std::endl;
	//}
	//else
	//print_ast(constrained_shorthand_void_arg_method.expected());


	//// Constrained method definition #func [&int] foo (a, b, c) {};
	//std::cout << "Testing #func [&int] foo (a, b, c) {};" << std::endl;
	//auto constrained_method = caoco::parse_directive_func(constrained_shorthand_void_arg_method.always(), result.cend());
	//EXPECT_TRUE(constrained_method.valid());
	//if (!constrained_method.valid()) {
	//	std::cout << constrained_method.error_message() << std::endl;
	//}
	//else
	//print_ast(constrained_method.expected());
	//
	/*Function definition with returns and statements
		#func foo{
			#return 1;
		};
	*/
	std::cout << "Testing #func foo{#return 1;}; " << std::endl;
	auto function_with_return = caoco::parse_directive_func(unconstrained_method.always(), result.cend());
	EXPECT_TRUE(function_with_return.valid());
	if (!function_with_return.valid()) {
		std::cout << function_with_return.error_message() << std::endl;
	}
	else
		print_ast(function_with_return.expected());


	/* Function definition with return and multiple statements
		#func foo {
			a = 1;
			b = 2;
			#return a + b;
		};
	*/
	std::cout << "Testing #func foo {a = 1;b = 2;#return a + b;}; " << std::endl;
	auto function_with_multiple_statements = caoco::parse_directive_func(function_with_return.always(), result.cend());
	EXPECT_TRUE(function_with_multiple_statements.valid());
	if (!function_with_multiple_statements.valid()) {
		std::cout << function_with_multiple_statements.error_message() << std::endl;
	}
	else
	print_ast(function_with_multiple_statements.expected());

}
#endif

#if CAOCO_UT_V2Parser_Classes
TEST(CaocoParser_BasicNode_Classes, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_classes.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto empty_class_def_end = test_parsing_function(
		"Empty Class Definition", &caoco::parse_directive_class,result.cbegin(), result.cend());
	auto class_def_with_members_end = test_parsing_function(
		"Class Definition with Members", &caoco::parse_directive_class, empty_class_def_end, result.cend());
	auto class_def_with_members_and_methods_end = test_parsing_function(
		"Class Definition with Members and Methods", &caoco::parse_directive_class, class_def_with_members_end, result.cend());
}
#endif

#if CAOCO_UT_V2Parser_PragmaticBlock

TEST(CaocoParser_BasicNode_PragmaticBlock, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_pragmaticblock.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto pragmatic_block_with_directives_end = test_parsing_function(
		"Pragmatic Block with Directives", &caoco::parse_pragmatic_block, result.cbegin(), result.cend());
}
#endif

#if CAOCO_UT_V2Parser_FunctionalBlock
TEST(CaocoParser_BasicNode_FunctionalBlock, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_functionalblock.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto functional_block_with_statements_end = test_parsing_function(
		"Functional Block with Statements", &caoco::parse_functional_block, result.cbegin(), result.cend());
}
#endif

#if CAOCO_UT_V2Parser_MinimumProgram 
TEST(CaocoParser_MinimumProgram, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_minimum_program.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	try {
		auto parse_result = caoco::parse_program(result.cbegin(), result.cend());
		print_ast(parse_result);
	}catch(std::exception e){
		std::cout << e.what() << std::endl;
	}
}
#endif

#if CAOCO_UT_V2Parser_ConditionalStatements
TEST(CaocoParser_ControlFlow, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_conditional.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto if_statement = test_parsing_function(
		"if statement", &caoco::parse_directive_if,result.cbegin(), result.cend());

	auto if_else_statement = test_parsing_function("if else statement", &caoco::parse_directive_if, if_statement, result.cend());

	auto if_elif_else = test_parsing_function("if elif else statement", &caoco::parse_directive_if, if_else_statement, result.cend());

}
#endif


#if CAOCO_UT_V2Parser_SwitchStatement
TEST(CaocoParser_SwitchStatement, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_switch.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto switch_statement = test_parsing_function(
		"switch statement", &caoco::parse_directive_on, result.cbegin(), result.cend());
}
#endif


#if CAOCO_UT_V2Parser_WhileLoop
TEST(CaocoParser_WhileLoop, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_while.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto while_loop = test_parsing_function(
		"while loop", &caoco::parse_directive_while, result.cbegin(), result.cend());
}
#endif

#if CAOCO_UT_V2Parser_ForLoop
TEST(CaocoParser_ForLoop, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_parser_for.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto for_loop = test_parsing_function(
		"for loop", &caoco::parse_directive_for, result.cbegin(), result.cend());
}

#endif


#if CAOCO_UT_V2ParserUtils_SeperatedList 
TEST(CaocoParserUtils_SeperatedList, CaocoParser_Test) {
	auto source_file = caoco::sl::to_char8_vector("{(a),{b},[c],(a,b),{a,c},{a,d}}\0");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto seperated_list = caoco::find_seperated_list_scopes(result.cbegin(), result.cend(),caoco::tk_enum::comma_);

	int i = 0;
	for(auto& ascope : seperated_list){
		std::cout << "Seperated List Arg" << i++ << ": ";
		for(auto a = ascope.scope_begin(); a != ascope.scope_end(); a++){
			std::cout << a->literal_str();
		}
		std::cout << std::endl;
	}
}
#endif

#if CAOCO_UT_V2Parser_ReturnStatement 
TEST(CaocoParser_ReturnStatement, CaocoParser_Test) {
	auto source_file = caoco::sl::to_char8_vector("#return a;\0");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	auto return_statement = test_parsing_function(
		"return statement", &caoco::parse_directive_return, result.cbegin(), result.cend());
}
#endif

#if CAOCO_UT_V2Parser_BasicProgram
TEST(CaocoParser_BasicProgram, CaocoParser_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_program_basic.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	try {
		auto parse_result = caoco::parse_program(result.cbegin(), result.cend());
		print_ast(parse_result);
	}catch(std::exception e){
		std::cout << e.what() << std::endl;
	}
}
#endif

#if CAOCO_UT_Preprocessor_Include 
TEST(CaocoPreprocessor_Include, CaocoPreprocessor_Test) {
	auto source_file = caoco::sl::load_file_to_char8_vector("ut_preprocessor_include.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	auto preprocessed = caoco::preprocess(result, "ut_preprocessor_include.candi");

	if(!std::get<1>(preprocessed)){
		std::cout << "Preprocessing Error: " << std::get<2>(preprocessed) << std::endl;
	}
	else {
		for (auto& token : std::get<0>(preprocessed)) {
			std::cout << token.literal_str();
		}
	}
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////