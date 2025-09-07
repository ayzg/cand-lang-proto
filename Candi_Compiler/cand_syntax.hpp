#pragma once
#include "global_dependencies.hpp"

template <auto STR,auto U8STR>
struct string_constant {
	static constexpr const char * str = STR();
	static constexpr const char8_t* u8 = U8STR();
};
#define STRING_CONSTANT(STR) string_constant<LAMBDA_STRING(STR),LAMBDA_U8STRING(STR)>

template<char c, char8_t u8c>
struct character_constant {
	static constexpr const char value = c;
	static constexpr const char8_t u8 = u8c;
};

namespace grammar {
	namespace characters {
		using EOFILE = character_constant<'\0', u8'\0'>;
		using WHITESPACE = character_constant<' ', u8' '>;
		using NEWLINE = character_constant<'\n', u8'\n'>;

		using HASH = character_constant<'#', u8'#'>;
		using COMMERICIAL_AT = character_constant<'@', u8'@'>;
		using ADD = character_constant<'+', u8'+'>;
		using SUB = character_constant<'-', u8'-'>;
		using MUL = character_constant<'*', u8'*'>;
		using DIV = character_constant<'/', u8'/' >;
		using MOD = character_constant<'%', u8'%' >;
		using AND = character_constant<'&', u8'&'>;
		using OR = character_constant<'|', u8'|'>;
		using XOR = character_constant<'^', u8'^'>;
		using NOT = character_constant<'!', u8'!'>;
		using LSH = character_constant<'<', u8'<'>;
		using RSH = character_constant<'>', u8'>'>;
		using EQ = character_constant<'=', u8'='>;


		using LPAREN = character_constant<'(', u8'('>;
		using RPAREN = character_constant<')', u8')'>;
		using LBRACE = character_constant<'{', u8'{'>;
		using RBRACE = character_constant<'}', u8'}'>;
		using LBRACKET = character_constant<'[', u8'['>;
		using RBRACKET = character_constant<']', u8']'>;

		using SEMICOLON = character_constant<';', u8';'>;
		using COLON = character_constant<':', u8':'>;
		using COMMA = character_constant<',', u8','>;
		using PERIOD = character_constant<'.', u8'.'>;
		using BACKLASH = character_constant<'\\', u8'\\'>;
		using APOSTROPHE = character_constant<'\'' , u8'\''>;
		using TILDE = character_constant<'~', u8'~'>;
	}

	namespace keywords {
		using INCLUDE = STRING_CONSTANT(include);
		using MACRO = STRING_CONSTANT(macro);
		using ENDMACRO = STRING_CONSTANT(endmacro);
		using ENTER = STRING_CONSTANT(enter);
		using START = STRING_CONSTANT(start);
		using USE = STRING_CONSTANT(use);
		using CLASS = STRING_CONSTANT(class);
		using OBJ = STRING_CONSTANT(obj);
		using PRIVATE = STRING_CONSTANT(private);
		using PUBLIC = STRING_CONSTANT(public);
		using CONST = STRING_CONSTANT(const);
		using STATIC = STRING_CONSTANT(static);
		using IF = STRING_CONSTANT(if);
		using ELSE = STRING_CONSTANT(else);
		using ELIF = STRING_CONSTANT(elif);
		using WHILE = STRING_CONSTANT(while);
		using FOR = STRING_CONSTANT(for);
		using SWITCH = STRING_CONSTANT(switch);
		using CASE = STRING_CONSTANT(case);
		using DEFAULT = STRING_CONSTANT(default);
		using BREAK = STRING_CONSTANT(break);
		using CONTINUE = STRING_CONSTANT(continue);
		using RETURN = STRING_CONSTANT(return);

		using PRINT = STRING_CONSTANT(print);
		using NONE = STRING_CONSTANT(none);

		using VALUE = STRING_CONSTANT(value);
		using TYPE = STRING_CONSTANT(type);
		using IDENTITY = STRING_CONSTANT(identity);
		using INT = STRING_CONSTANT(int);
		using UINT = STRING_CONSTANT(uint);
		using REAL = STRING_CONSTANT(real);
		using BYTE = STRING_CONSTANT(byte);
		using BIT = STRING_CONSTANT(bit);
		using STR = STRING_CONSTANT(str);
		using ARRAY = STRING_CONSTANT(array);
		using POINTER = STRING_CONSTANT(pointer);
		using MEMORY = STRING_CONSTANT(memory);
		using FUNCTION = STRING_CONSTANT(function);
	}

	namespace directives {
		using INCLUDE = STRING_CONSTANT(#include);
		using MACRO = STRING_CONSTANT(#macro);
		using ENDMACRO = STRING_CONSTANT(#endmacro);
		using ENTER = STRING_CONSTANT(#enter);
		using START = STRING_CONSTANT(#start);
		using USE = STRING_CONSTANT(#use);
		using CLASS = STRING_CONSTANT(#class);
		using OBJ = STRING_CONSTANT(#obj);
		using PRIVATE = STRING_CONSTANT(#private);
		using PUBLIC = STRING_CONSTANT(#public);
		using CONST = STRING_CONSTANT(#const);
		using STATIC = STRING_CONSTANT(#static);
		using IF = STRING_CONSTANT(#if);
		using ELSE = STRING_CONSTANT(#else);
		using ELIF = STRING_CONSTANT(#elif);
		using WHILE = STRING_CONSTANT(#while);
		using FOR = STRING_CONSTANT(#for);
		using SWITCH = STRING_CONSTANT(#switch);
		using CASE = STRING_CONSTANT(#case);
		using DEFAULT = STRING_CONSTANT(#default);
		using BREAK = STRING_CONSTANT(#break);
		using CONTINUE = STRING_CONSTANT(#continue);
		using RETURN = STRING_CONSTANT(#return);

		using PRINT = STRING_CONSTANT(#print);
		using NONE = STRING_CONSTANT(#none);

		using VALUE = STRING_CONSTANT(#value);
		using TYPE = STRING_CONSTANT(#type);
		using IDENTITY = STRING_CONSTANT(#identity);
		using INT = STRING_CONSTANT(#int);
		using UINT = STRING_CONSTANT(#uint);
		using REAL = STRING_CONSTANT(#real);
		using BYTE = STRING_CONSTANT(#byte);
		using BIT = STRING_CONSTANT(#bit);
		using STR = STRING_CONSTANT(#str);

		using ARRAY = STRING_CONSTANT(#array);
		using POINTER = STRING_CONSTANT(#pointer);
		using MEMORY = STRING_CONSTANT(#memory);
		using FUNCTION = STRING_CONSTANT(#function);
	}

	namespace operators {
		using HASH = STRING_CONSTANT(#);
		using ADD = STRING_CONSTANT(+);
		using SUB = STRING_CONSTANT(-);
		using MUL = STRING_CONSTANT(*);
		using DIV = STRING_CONSTANT(/ );
		using MOD = STRING_CONSTANT(%);
		using AND = STRING_CONSTANT(&);
		using OR = STRING_CONSTANT(| );
		using XOR = STRING_CONSTANT(^);
		using NOT = STRING_CONSTANT(!);
		using LSH = STRING_CONSTANT(<< );
		using RSH = STRING_CONSTANT(>> );
		using EQ = STRING_CONSTANT(== );
		using NEQ = STRING_CONSTANT(!= );
		using LT = STRING_CONSTANT(< );
		using GT = STRING_CONSTANT(> );
		using LTE = STRING_CONSTANT(<= );
		using GTE = STRING_CONSTANT(>= );
		using ASSIGN = STRING_CONSTANT(= );
		using NEW_ASSIGN = STRING_CONSTANT(: = );
		using ADD_ASSIGN = STRING_CONSTANT(+= );
		using SUB_ASSIGN = STRING_CONSTANT(-= );
		using MUL_ASSIGN = STRING_CONSTANT(*= );
		using DIV_ASSIGN = STRING_CONSTANT(/= );
		using MOD_ASSIGN = STRING_CONSTANT(%= );
		using AND_ASSIGN = STRING_CONSTANT(&= );
		using OR_ASSIGN = STRING_CONSTANT(|= );
		using XOR_ASSIGN = STRING_CONSTANT(^= );
		using LSH_ASSIGN = STRING_CONSTANT(<<= );
		using RSH_ASSIGN = STRING_CONSTANT(>>= );
		using INC = STRING_CONSTANT(++);
		using DEC = STRING_CONSTANT(--);
		using DOT = STRING_CONSTANT(.);
		using BNOT = STRING_CONSTANT(~);
		using BAND = STRING_CONSTANT(&&);
		using BOR = STRING_CONSTANT(|| );
	}

	namespace scopes {
		using OPEN_PAREN = string_constant < []()consteval {return "("; }, []()consteval {return u8"("; } > ;
		using CLOSE_PAREN = string_constant < []()consteval {return ")"; }, []()consteval {return u8")"; } > ;
		using OPEN_BRACE = STRING_CONSTANT({ );
		using CLOSE_BRACE = STRING_CONSTANT(});
		using OPEN_BRACKET = STRING_CONSTANT([);
		using CLOSE_BRACKET = STRING_CONSTANT(]);
		using SEMICOLON = STRING_CONSTANT(;);
		using COLON = STRING_CONSTANT(:);
		using COMMA = string_constant < []()consteval {return ","; }, []()consteval {return u8","; } > ;
		using PERIOD = STRING_CONSTANT(.);
		using ELLIPSIS = STRING_CONSTANT(...);
		using COMMERCIAL_AT = STRING_CONSTANT(@);
	}

	using EOFILE = STRING_CONSTANT(\0);
	using WHITESPACE = string_constant < []()consteval {return " "; }, []()consteval {return u8" "; } > ;
	using NEWLINE = STRING_CONSTANT(\n);
	using BLOCK_COMMENT = string_constant < []()consteval {return "///"; }, []()consteval {return u8"///"; } > ;
	using LINE_COMMENT = string_constant < []()consteval {return "//"; }, []()consteval {return u8"//"; } > ;
}

enum class e_assoc : int {
	none_,
	left_,
	right_
};
enum class e_operation : int {
	none_,
	binary_,
	binary_or_prefix_,
	prefix_,
	postfix_,
	variadic_,
	scope_entry_,
	scope_exit_
};
namespace priority {
	enum e_priority : int {
		none_ = 0,
		assignment_ = 20000,
		logical_or_ = 60000,
		logical_and_ = 70000,
		bitwise_or_ = 80000,
		bitwise_xor_ = 85000,
		bitwise_and_ = 90000,
		equality_ = 100000,
		three_way_equality_ = 110000,
		comparison_ = 120000,
		bitshift_ = 130000,
		term_ = 140000,
		factor_ = 150000,
		prefix_ = 160000,
		postfix_ = 170000,
		functional_ = 180000,
		access_ = 190000,
		max_ = INT_MAX
	};
}

enum class e_tk : int {
	// abstract 
	none_ = -1, invalid_, eof_,

	// invisible
	line_comment_,
	block_comment_,
	newline_,
	whitespace_,

	// literals
	string_literal_, 
	number_literal_, 
	real_literal_, 
	byte_literal_,
	bit_literal_, 
	unsigned_literal_, 
	alnumus_,	

	// assignemnt operators
	simple_assignment_, 
	addition_assignment_, 
	subtraction_assignment_,
	multiplication_assignment_, 
	division_assignment_, 
	remainder_assignment_, 
	bitwise_and_assignment_, 
	bitwise_or_assignment_, 
	bitwise_xor_assignment_, 
	left_shift_assignment_, 
	right_shift_assignment_,

	// postifx increment and decrement operators
	increment_,
	decrement_,

	// arithmetic operators
	addition_, 
	subtraction_, 
	multiplication_, 
	division_, 
	remainder_,
	bitwise_and_,
	bitwise_or_,
	bitwise_xor_,
	bitwise_left_shift_,
	bitwise_right_shift_,

	// prefix operators
	negation_,
	bitwise_not_,

	// logical
	logical_and_, 
	logical_or_,

	// comparison
	equal_, 
	not_equal_, 
	less_than_, 
	greater_than_,
	less_than_or_equal_, 
	greater_than_or_equal_, 
	three_way_comparison_,

	// scopes
	open_paren_, 
	close_paren_, 
	open_brace_, 
	close_brace_, 
	open_bracket_, 
	close_bracket_,

	// special
	semicolon_,
	colon_, 
	comma_, 
	period_, 
	ellipsis_,
	commerical_at_,

	// keyword literal values
	none_literal_,
	true_literal_,
	false_literal_,

	// keyword types
	type_, 
	identity_, 
	value_, 
	int_, 
	uint_, 
	real_, 
	byte_,
	bit_, 
	str_, 
	array_, 
	pointer_,
	memory_, 
	function_,

	// preprocessor keywords
	include_,
	macro_,
	endmacro_,

	// directive keywords
	enter_, 
	start_,  
	use_, 
	class_,  
	obj_, 

	// functional keywords
	print_,

	// modifier tokens
	private_,
	public_, 
	const_, 
	static_, 
	ref_,

	// control flow tokens
	if_, 
	else_, 
	elif_, 
	while_, 
	for_, 
	switch_, 
	case_, 
	default_, 
	break_,
	continue_, 
	return_, 
};
enum class e_ast : int {
	// abstract 
	none_ = -1, invalid_, eof_,

	// invisible
	line_comment_,
	block_comment_,
	newline_,
	whitespace_,

	// literals
	string_literal_,
	number_literal_,
	real_literal_,
	byte_literal_,
	bit_literal_,
	unsigned_literal_,
	alnumus_,

	// assignemnt operators
	simple_assignment_,
	addition_assignment_,
	subtraction_assignment_,
	multiplication_assignment_,
	division_assignment_,
	remainder_assignment_,
	bitwise_and_assignment_,
	bitwise_or_assignment_,
	bitwise_xor_assignment_,
	left_shift_assignment_,
	right_shift_assignment_,

	// postifx increment and decrement operators
	increment_,
	decrement_,

	// arithmetic operators
	addition_,
	subtraction_,
	multiplication_,
	division_,
	remainder_,
	bitwise_and_,
	bitwise_or_,
	bitwise_xor_,
	bitwise_left_shift_,
	bitwise_right_shift_,

	// prefix operators
	bitwise_not_,
	negation_,


	// logical
	logical_and_,
	logical_or_,

	// comparison
	equal_,
	not_equal_,
	less_than_,
	greater_than_,
	less_than_or_equal_,
	greater_than_or_equal_,
	three_way_comparison_,

	// scopes
	open_paren_,
	close_paren_,
	open_brace_,
	close_brace_,
	open_bracket_,
	close_bracket_,

	// special
	semicolon_,
	colon_,
	comma_,
	period_,
	ellipsis_,
	commerical_at_,

	// keyword literal values
	none_literal_,
	true_literal_,
	false_literal_,

	// keyword types
	type_,
	identity_,
	value_,
	int_,
	uint_,
	real_,
	byte_,
	bit_,
	str_,
	array_,
	pointer_,
	memory_,
	function_,

	// preprocessor keywords
	include_,
	macro_,
	endmacro_,

	// directive keywords
	enter_,
	start_,
	use_,
	class_,
	obj_,

	// functional keywords
	print_,

	// modifier tokens
	private_,
	public_,
	const_,
	static_,
	ref_,

	// control flow tokens
	if_,
	else_,
	elif_,
	while_,
	for_,
	switch_,
	case_,
	default_,
	break_,
	continue_,
	return_,

	// intermediates
	statement_,
	expression_,

	// inferred abstract nodes (do not perform operations, do not have a literal and are not an operand or operator.)
	program_,
	pragmatic_block_,
	functional_block_,
	conditional_block_,
	iterative_block_,

	parameter_,
	parameter_list_, // () 

	type_constraints_, // [] 
	capture_list_, // [] 

	// operands
	subexpression_, // () 
	type_list_, // [] 
	generic_list_, // {} 
	
	// operators
	function_call_, // <operand><()>
	arguments_, // <(<...><,>)>

	type_call_, // <operand><[]>
	type_arguments_, // <[<...><,>]>

	index_operator_, // <operand><{}>
	index_arguments_, // <{<...><,>}>

	unary_minus_, // -
	dereference_, // *
	address_of_, // &



};

// enum string conversions
namespace sl {
	SL_CXIN sl_string to_str(e_assoc a)
	{
		switch (a)
		{
		case e_assoc::none_: return "none";
		case e_assoc::left_: return "left";
		case e_assoc::right_: return "right";
		default: return "e_assoc invalid enum value";
		}
	}
	SL_CXIN sl_string to_str(e_operation o) {
		switch (o) {
		case e_operation::none_: return "none";
		case e_operation::binary_: return "binary";
		case e_operation::binary_or_prefix_: return "binary_or_prefix";
		case e_operation::prefix_: return "prefix";
		case e_operation::postfix_: return "postfix";
		case e_operation::variadic_: return "variadic";
		case e_operation::scope_entry_: return "scope_entry";
		case e_operation::scope_exit_: return "scope_exit";
		default: return "e_operation invalid enum value";
		}
	}
	SL_CXIN sl_string to_str(priority::e_priority p) {
		switch (p) {
		case priority::e_priority::none_: return "none";
		case priority::e_priority::assignment_: return "assignment";
		case priority::e_priority::logical_or_: return "logical_or";
		case priority::e_priority::logical_and_: return "logical_and";
		case priority::e_priority::bitwise_or_: return "bitwise_or";
		case priority::e_priority::bitwise_xor_: return "bitwise_xor";
		case priority::e_priority::bitwise_and_: return "bitwise_and";
		case priority::e_priority::equality_: return "equality";
		case priority::e_priority::three_way_equality_: return "three_way_equality";
		case priority::e_priority::comparison_: return "comparison";
		case priority::e_priority::bitshift_: return "bitshift";
		case priority::e_priority::term_: return "term";
		case priority::e_priority::factor_: return "factor";
		case priority::e_priority::prefix_: return "prefix";
		case priority::e_priority::postfix_: return "postfix";
		case priority::e_priority::functional_: return "functional";
		case priority::e_priority::access_: return "access";
		case priority::e_priority::max_: return "max";
		default: return "priority::e_priority invalid enum value";
		}
	}
	SL_CXIN sl_string to_str(e_tk t) {
		switch (t) {
		case e_tk::none_: return "none";
		case e_tk::invalid_: return "invalid";
		case e_tk::eof_: return "eof";
		case e_tk::line_comment_: return "line_comment";
		case e_tk::block_comment_: return "block_comment";
		case e_tk::newline_: return "newline";
		case e_tk::whitespace_: return "whitespace";
		case e_tk::string_literal_: return "string_literal";
		case e_tk::number_literal_: return "number_literal";
		case e_tk::real_literal_: return "real_literal";
		case e_tk::byte_literal_: return "byte_literal";
		case e_tk::bit_literal_: return "bit_literal";
		case e_tk::unsigned_literal_: return "unsigned_literal";
		case e_tk::alnumus_: return "alnumus";
		case e_tk::simple_assignment_: return "simple_assignment";
		case e_tk::addition_assignment_: return "addition_assignment";
		case e_tk::subtraction_assignment_: return "subtraction_assignment";
		case e_tk::multiplication_assignment_: return "multiplication_assignment";
		case e_tk::division_assignment_: return "division_assignment";
		case e_tk::remainder_assignment_: return "remainder_assignment";
		case e_tk::bitwise_and_assignment_: return "bitwise_and_assignment";
		case e_tk::bitwise_or_assignment_: return "bitwise_or_assignment";
		case e_tk::bitwise_xor_assignment_: return "bitwise_xor_assignment";
		case e_tk::left_shift_assignment_: return "left_shift_assignment";
		case e_tk::right_shift_assignment_: return "right_shift_assignment";
		case e_tk::increment_: return "increment";
		case e_tk::decrement_: return "decrement";
		case e_tk::addition_: return "addition";
		case e_tk::subtraction_: return "subtraction";
		case e_tk::multiplication_: return "multiplication";
		case e_tk::division_: return "division";
		case e_tk::remainder_: return "remainder";
		case e_tk::bitwise_not_: return "bitwise_not";
		case e_tk::bitwise_and_: return "bitwise_and";
		case e_tk::bitwise_or_: return "bitwise_or";
		case e_tk::bitwise_xor_: return "bitwise_xor";
		case e_tk::bitwise_left_shift_: return "bitwise_left_shift";
		case e_tk::bitwise_right_shift_: return "bitwise_right_shift";
		case e_tk::negation_: return "negation";
		case e_tk::logical_and_: return "logical_and";
		case e_tk::logical_or_: return "logical_or";
		case e_tk::equal_: return "equal";
		case e_tk::not_equal_: return "not_equal";
		case e_tk::less_than_: return "less_than";
		case e_tk::greater_than_: return "greater_than";
		case e_tk::less_than_or_equal_: return "less_than_or_equal";
		case e_tk::greater_than_or_equal_: return "greater_than_or_equal";
		case e_tk::three_way_comparison_: return "three_way_comparison";
		case e_tk::open_paren_: return "(";//"open_paren";
		case e_tk::close_paren_: return ")";//"close_paren";
		case e_tk::open_brace_: return "open_brace";
		case e_tk::close_brace_: return "close_brace";
		case e_tk::open_bracket_: return "open_bracket";
		case e_tk::close_bracket_: return "close_bracket";
		case e_tk::semicolon_: return "semicolon";
		case e_tk::colon_: return "colon";
		case e_tk::comma_: return "comma";
		case e_tk::period_: return "period";
		case e_tk::ellipsis_: return "ellipsis";
		case e_tk::commerical_at_: return "commerical_at";
		case e_tk::none_literal_: return "none_literal";
		case e_tk::true_literal_: return "true_literal";
		case e_tk::false_literal_: return "false_literal";
		case e_tk::type_: return "type";
		case e_tk::identity_: return "identity";
		case e_tk::value_: return "value";
		case e_tk::int_: return "int";
		case e_tk::uint_: return "uint";
		case e_tk::real_: return "real";
		case e_tk::byte_: return "byte";
		case e_tk::bit_: return "bit";
		case e_tk::str_: return "str";
		case e_tk::array_: return "array";
		case e_tk::pointer_: return "pointer";
		case e_tk::memory_: return "memory";
		case e_tk::function_: return "function";
		case e_tk::include_: return "include";
		case e_tk::macro_: return "macro";
		case e_tk::endmacro_: return "endmacro";
		case e_tk::enter_: return "enter";
		case e_tk::start_: return "start";
		case e_tk::use_: return "use";
		case e_tk::class_: return "class";
		case e_tk::obj_: return "obj";
		case e_tk::print_: return "print";
		case e_tk::private_: return "private";
		case e_tk::public_: return "public";
		case e_tk::const_: return "const";
		case e_tk::static_: return "static";
		case e_tk::ref_: return "ref";
		case e_tk::if_: return "if";
		case e_tk::else_: return "else";
		case e_tk::elif_: return "elif";
		case e_tk::while_: return "while";
		case e_tk::for_: return "for";
		case e_tk::switch_: return "switch";
		case e_tk::case_: return "case";
		case e_tk::default_: return "default";
		case e_tk::break_: return "break";
		case e_tk::continue_: return "continue";
		case e_tk::return_: return "return";
		default: return "e_tk invalid enum value";
		}
	}
	SL_CXIN sl_string to_str(e_ast t) {
		switch (t) {
		case e_ast::none_: return "none";
		case e_ast::invalid_: return "invalid";
		case e_ast::eof_: return "eof";
		case e_ast::line_comment_: return "line_comment";
		case e_ast::block_comment_: return "block_comment";
		case e_ast::newline_: return "newline";
		case e_ast::whitespace_: return "whitespace";
		case e_ast::string_literal_: return "string_literal";
		case e_ast::number_literal_: return "number_literal";
		case e_ast::real_literal_: return "real_literal";
		case e_ast::byte_literal_: return "byte_literal";
		case e_ast::bit_literal_: return "bit_literal";
		case e_ast::unsigned_literal_: return "unsigned_literal";
		case e_ast::alnumus_: return "alnumus";
		case e_ast::simple_assignment_: return "simple_assignment";
		case e_ast::addition_assignment_: return "addition_assignment";
		case e_ast::subtraction_assignment_: return "subtraction_assignment";
		case e_ast::multiplication_assignment_: return "multiplication_assignment";
		case e_ast::division_assignment_: return "division_assignment";
		case e_ast::remainder_assignment_: return "remainder_assignment";
		case e_ast::bitwise_and_assignment_: return "bitwise_and_assignment";
		case e_ast::bitwise_or_assignment_: return "bitwise_or_assignment";
		case e_ast::bitwise_xor_assignment_: return "bitwise_xor_assignment";
		case e_ast::left_shift_assignment_: return "left_shift_assignment";
		case e_ast::right_shift_assignment_: return "right_shift_assignment";
		case e_ast::increment_: return "increment";
		case e_ast::decrement_: return "decrement";
		case e_ast::addition_: return "addition";
		case e_ast::subtraction_: return "subtraction";
		case e_ast::multiplication_: return "multiplication";
		case e_ast::division_: return "division";
		case e_ast::remainder_: return "remainder";
		case e_ast::bitwise_not_: return "bitwise_not";
		case e_ast::bitwise_and_: return "bitwise_and";
		case e_ast::bitwise_or_: return "bitwise_or";
		case e_ast::bitwise_xor_: return "bitwise_xor";
		case e_ast::bitwise_left_shift_: return "bitwise_left_shift";
		case e_ast::bitwise_right_shift_: return "bitwise_right_shift";
		case e_ast::negation_: return "negation";
		case e_ast::logical_and_: return "logical_and";
		case e_ast::logical_or_: return "logical_or";
		case e_ast::equal_: return "equal";
		case e_ast::not_equal_: return "not_equal";
		case e_ast::less_than_: return "less_than";
		case e_ast::greater_than_: return "greater_than";
		case e_ast::less_than_or_equal_: return "less_than_or_equal";
		case e_ast::greater_than_or_equal_: return "greater_than_or_equal";
		case e_ast::three_way_comparison_: return "three_way_comparison";
		case e_ast::open_paren_: return "open_paren";
		case e_ast::close_paren_: return "close_paren";
		case e_ast::open_brace_: return "open_brace";
		case e_ast::close_brace_: return "close_brace";
		case e_ast::open_bracket_: return "open_bracket";
		case e_ast::close_bracket_: return "close_bracket";
		case e_ast::semicolon_: return "semicolon";
		case e_ast::colon_: return "colon";
		case e_ast::comma_: return "comma";
		case e_ast::period_: return "period";
		case e_ast::ellipsis_: return "ellipsis";
		case e_ast::commerical_at_: return "commerical_at";
		case e_ast::none_literal_: return "none_literal";
		case e_ast::true_literal_: return "true_literal";
		case e_ast::false_literal_: return "false_literal";
		case e_ast::type_: return "type";
		case e_ast::identity_: return "identity";
		case e_ast::value_: return "value";
		case e_ast::int_: return "int";
		case e_ast::uint_: return "uint";
		case e_ast::real_: return "real";
		case e_ast::byte_: return "byte";
		case e_ast::bit_: return "bit";
		case e_ast::str_: return "str";
		case e_ast::array_: return "array";
		case e_ast::pointer_: return "pointer";
		case e_ast::memory_: return "memory";
		case e_ast::function_: return "function";
		case e_ast::include_: return "include";
		case e_ast::macro_: return "macro";
		case e_ast::endmacro_: return "endmacro";
		case e_ast::enter_: return "enter";
		case e_ast::start_: return "start";
		case e_ast::use_: return "use";
		case e_ast::class_: return "class";
		case e_ast::obj_: return "obj";
		case e_ast::print_: return "print";
		case e_ast::private_: return "private";
		case e_ast::public_: return "public";
		case e_ast::const_: return "const";
		case e_ast::static_: return "static";
		case e_ast::ref_: return "ref";
		case e_ast::if_: return "if";
		case e_ast::else_: return "else";
		case e_ast::elif_: return "elif";
		case e_ast::while_: return "while";
		case e_ast::for_: return "for";
		case e_ast::switch_: return "switch";
		case e_ast::case_: return "case";
		case e_ast::default_: return "default";
		case e_ast::break_: return "break";
		case e_ast::continue_: return "continue";
		case e_ast::return_: return "return";
			// astnode intermediates
		case  e_ast::statement_: return "statement";
		case  e_ast::expression_: return "expression";
		case  e_ast::program_: return "program";
		case  e_ast::pragmatic_block_: return "pragmatic_block";
		case  e_ast::functional_block_: return "functional_block";
		case  e_ast::conditional_block_: return "conditional_block";
		case  e_ast::iterative_block_: return "iterative_block";
		case  e_ast::arguments_: return "arguments";
		case  e_ast::parameter_: return "parameter";
		case  e_ast::parameter_list_: return "parameter_list";
		case  e_ast::type_constraints_: return "type_constraints";
		case  e_ast::capture_list_: return "capture_list";
		case  e_ast::subexpression_: return "subexpression";
		case  e_ast::type_list_: return "type_list";
		case  e_ast::generic_list_: return "generic_list";
		case  e_ast::function_call_: return "function_call";
		case  e_ast::type_call_: return "type_call";
		case  e_ast::type_arguments_: return "type_arguments";
		case  e_ast::index_operator_: return "index_operator";
		case e_ast::index_arguments_: return "index_arguments";
		case  e_ast::unary_minus_: return "unary_minus";
		case  e_ast::dereference_: return "dereference";
		case  e_ast::address_of_: return "address_of";
		default: return "e_ast invalid enum value";
		}
	}
}

SL_CX e_ast tk_type_to_astnode_type(e_tk t) {
    switch(t) {
        case e_tk::none_: return e_ast::none_;
        case e_tk::invalid_: return e_ast::invalid_;
        case e_tk::eof_: return e_ast::eof_;
        case e_tk::line_comment_: return e_ast::line_comment_;
        case e_tk::block_comment_: return e_ast::block_comment_;
        case e_tk::newline_: return e_ast::newline_;
        case e_tk::whitespace_: return e_ast::whitespace_;
        case e_tk::string_literal_: return e_ast::string_literal_;
        case e_tk::number_literal_: return e_ast::number_literal_;
        case e_tk::real_literal_: return e_ast::real_literal_;
        case e_tk::byte_literal_: return e_ast::byte_literal_;
        case e_tk::bit_literal_: return e_ast::bit_literal_;
        case e_tk::unsigned_literal_: return e_ast::unsigned_literal_;
        case e_tk::alnumus_: return e_ast::alnumus_;
        case e_tk::simple_assignment_: return e_ast::simple_assignment_;
        case e_tk::addition_assignment_: return e_ast::addition_assignment_;
        case e_tk::subtraction_assignment_: return e_ast::subtraction_assignment_;
        case e_tk::multiplication_assignment_: return e_ast::multiplication_assignment_;
        case e_tk::division_assignment_: return e_ast::division_assignment_;
        case e_tk::remainder_assignment_: return e_ast::remainder_assignment_;
        case e_tk::bitwise_and_assignment_: return e_ast::bitwise_and_assignment_;
        case e_tk::bitwise_or_assignment_: return e_ast::bitwise_or_assignment_;
        case e_tk::bitwise_xor_assignment_: return e_ast::bitwise_xor_assignment_;
        case e_tk::left_shift_assignment_: return e_ast::left_shift_assignment_;
        case e_tk::right_shift_assignment_: return e_ast::right_shift_assignment_;
        case e_tk::increment_: return e_ast::increment_;
        case e_tk::decrement_: return e_ast::decrement_;
        case e_tk::addition_: return e_ast::addition_;
        case e_tk::subtraction_: return e_ast::subtraction_;
        case e_tk::multiplication_: return e_ast::multiplication_;
        case e_tk::division_: return e_ast::division_;
        case e_tk::remainder_: return e_ast::remainder_;
        case e_tk::bitwise_not_: return e_ast::bitwise_not_;
        case e_tk::bitwise_and_: return e_ast::bitwise_and_;
        case e_tk::bitwise_or_: return e_ast::bitwise_or_;
        case e_tk::bitwise_xor_: return e_ast::bitwise_xor_;
        case e_tk::bitwise_left_shift_: return e_ast::bitwise_left_shift_;
        case e_tk::bitwise_right_shift_: return e_ast::bitwise_right_shift_;
        case e_tk::negation_: return e_ast::negation_;
        case e_tk::logical_and_: return e_ast::logical_and_;
        case e_tk::logical_or_: return e_ast::logical_or_;
        case e_tk::equal_: return e_ast::equal_;
        case e_tk::not_equal_: return e_ast::not_equal_;
        case e_tk::less_than_: return e_ast::less_than_;
        case e_tk::greater_than_: return e_ast::greater_than_;
        case e_tk::less_than_or_equal_: return e_ast::less_than_or_equal_;
        case e_tk::greater_than_or_equal_: return e_ast::greater_than_or_equal_;
        case e_tk::three_way_comparison_: return e_ast::three_way_comparison_;
        case e_tk::open_paren_: return e_ast::open_paren_;
        case e_tk::close_paren_: return e_ast::close_paren_;
        case e_tk::open_brace_: return e_ast::open_brace_;
        case e_tk::close_brace_: return e_ast::close_brace_;
        case e_tk::open_bracket_: return e_ast::open_bracket_;
        case e_tk::close_bracket_: return e_ast::close_bracket_;
        case e_tk::semicolon_: return e_ast::semicolon_;
        case e_tk::colon_: return e_ast::colon_;
        case e_tk::comma_: return e_ast::comma_;
        case e_tk::period_: return e_ast::period_;
        case e_tk::ellipsis_: return e_ast::ellipsis_;
        case e_tk::commerical_at_: return e_ast::commerical_at_;
        case e_tk::none_literal_: return e_ast::none_literal_;
        case e_tk::true_literal_: return e_ast::true_literal_;
        case e_tk::false_literal_: return e_ast::false_literal_;
        case e_tk::type_: return e_ast::type_;
        case e_tk::identity_: return e_ast::identity_;
        case e_tk::value_: return e_ast::value_;
        case e_tk::int_: return e_ast::int_;
        case e_tk::uint_: return e_ast::uint_;
        case e_tk::real_: return e_ast::real_;
        case e_tk::byte_: return e_ast::byte_;
        case e_tk::bit_: return e_ast::bit_;
        case e_tk::str_: return e_ast::str_;
        case e_tk::array_: return e_ast::array_;
        case e_tk::pointer_: return e_ast::pointer_;
        case e_tk::memory_: return e_ast::memory_;
        case e_tk::function_: return e_ast::function_;
        case e_tk::include_: return e_ast::include_;
        case e_tk::macro_: return e_ast::macro_;
        case e_tk::endmacro_: return e_ast::endmacro_;
        case e_tk::enter_: return e_ast::enter_;
        case e_tk::start_: return e_ast::start_;
        case e_tk::use_: return e_ast::use_;
        case e_tk::class_: return e_ast::class_;
        case e_tk::obj_: return e_ast::obj_;
        case e_tk::print_: return e_ast::print_;
        case e_tk::private_: return e_ast::private_;
        case e_tk::public_: return e_ast::public_;
        case e_tk::const_: return e_ast::const_;
        case e_tk::static_: return e_ast::static_;
        case e_tk::ref_: return e_ast::ref_;
        case e_tk::if_: return e_ast::if_;
        case e_tk::else_: return e_ast::else_;
        case e_tk::elif_: return e_ast::elif_;
        case e_tk::while_: return e_ast::while_;
        case e_tk::for_: return e_ast::for_;
        case e_tk::switch_: return e_ast::switch_;
        case e_tk::case_: return e_ast::case_;
        case e_tk::default_: return e_ast::default_;
        case e_tk::break_: return e_ast::break_;
        case e_tk::continue_: return e_ast::continue_;
        case e_tk::return_: return e_ast::return_;
        default: return e_ast::none_;
    }
}
SL_CX e_assoc tk_type_assoc(e_tk t) {
	switch(t) {
		case e_tk::none_:case e_tk::invalid_:case e_tk::eof_:
		case e_tk::line_comment_:case e_tk::block_comment_:case e_tk::newline_:case e_tk::whitespace_:
		case e_tk::string_literal_:case e_tk::number_literal_:case e_tk::real_literal_:case e_tk::byte_literal_:
		case e_tk::bit_literal_:case e_tk::unsigned_literal_:case e_tk::alnumus_:
		case e_tk::none_literal_:case e_tk::true_literal_:case e_tk::false_literal_:
		case e_tk::type_:case e_tk::identity_:case e_tk::value_:case e_tk::int_:case e_tk::uint_:
		case e_tk::real_:case e_tk::byte_:case e_tk::bit_:case e_tk::str_:case e_tk::array_:case e_tk::pointer_:
		case e_tk::memory_:case e_tk::function_:case e_tk::include_:case e_tk::macro_:case e_tk::endmacro_:
		case e_tk::enter_:case e_tk::start_:case e_tk::use_:case e_tk::class_:case e_tk::obj_:case e_tk::print_:
		case e_tk::private_:case e_tk::public_:case e_tk::const_:case e_tk::static_:case e_tk::ref_:
		case e_tk::if_:case e_tk::else_:case e_tk::elif_:case e_tk::while_:case e_tk::for_:case e_tk::switch_:
		case e_tk::case_:case e_tk::default_:case e_tk::break_:case e_tk::continue_:case e_tk::return_:
			return e_assoc::none_;
		case e_tk::simple_assignment_:case e_tk::addition_assignment_:case e_tk::subtraction_assignment_:
		case e_tk::multiplication_assignment_:case e_tk::division_assignment_:case e_tk::remainder_assignment_:
		case e_tk::bitwise_and_assignment_:case e_tk::bitwise_or_assignment_:case e_tk::bitwise_xor_assignment_:
		case e_tk::left_shift_assignment_:case e_tk::right_shift_assignment_:
		case e_tk::negation_:case e_tk::bitwise_not_:
			return e_assoc::right_;
		case e_tk::addition_:case e_tk::subtraction_:case e_tk::multiplication_:case e_tk::division_:
		case e_tk::remainder_:case e_tk::bitwise_and_:case e_tk::bitwise_or_:case e_tk::bitwise_xor_:
		case e_tk::bitwise_left_shift_:case e_tk::bitwise_right_shift_:
		case e_tk::logical_and_:case e_tk::logical_or_:case e_tk::equal_:case e_tk::not_equal_:case e_tk::less_than_:
		case e_tk::greater_than_:case e_tk::less_than_or_equal_:case e_tk::greater_than_or_equal_:case e_tk::three_way_comparison_:
		case e_tk::increment_: case e_tk::decrement_:case e_tk::period_:
			return e_assoc::left_;
		case e_tk::open_paren_:case e_tk::close_paren_:case e_tk::open_brace_:case e_tk::close_brace_:
		case e_tk::open_bracket_:case e_tk::close_bracket_:case e_tk::semicolon_:case e_tk::colon_:case e_tk::comma_:
		return e_assoc::none_;
		case e_tk::ellipsis_:case e_tk::commerical_at_:
			return e_assoc::right_;
		default: return e_assoc::none_;
	}
}
SL_CX e_operation tk_type_operation(e_tk t) {
	switch (t)
	{
	case e_tk::none_:
		
	case e_tk::invalid_:
		
	case e_tk::eof_:
		
	case e_tk::line_comment_:
		
	case e_tk::block_comment_:
		
	case e_tk::newline_:
		
	case e_tk::whitespace_:
		
	case e_tk::string_literal_:
		
	case e_tk::number_literal_:
		
	case e_tk::real_literal_:
		
	case e_tk::byte_literal_:
		
	case e_tk::bit_literal_:
		
	case e_tk::unsigned_literal_:
		
	case e_tk::alnumus_:
		return e_operation::none_;
		break;
	case e_tk::simple_assignment_:
		
	case e_tk::addition_assignment_:
		
	case e_tk::subtraction_assignment_:
		
	case e_tk::multiplication_assignment_:
		
	case e_tk::division_assignment_:
		
	case e_tk::remainder_assignment_:
		
	case e_tk::bitwise_and_assignment_:
		
	case e_tk::bitwise_or_assignment_:
		
	case e_tk::bitwise_xor_assignment_:
		
	case e_tk::left_shift_assignment_:
	case e_tk::right_shift_assignment_:
		return e_operation::binary_;
		break;
	case e_tk::increment_:
	case e_tk::decrement_:
		return e_operation::postfix_;
		break;
	case e_tk::addition_:
		
	case e_tk::subtraction_:
		
	case e_tk::multiplication_:
		
	case e_tk::division_:
		
	case e_tk::remainder_:
		
	case e_tk::bitwise_and_:
		
	case e_tk::bitwise_or_:
		
	case e_tk::bitwise_xor_:
		
	case e_tk::bitwise_left_shift_:
		
	case e_tk::bitwise_right_shift_:
		return e_operation::binary_;
		break;
	case e_tk::negation_:
	case e_tk::bitwise_not_:
		return e_operation::prefix_;
		break;
	case e_tk::logical_and_:
		
	case e_tk::logical_or_:
		
	case e_tk::equal_:
		
	case e_tk::not_equal_:
		
	case e_tk::less_than_:
		
	case e_tk::greater_than_:
		
	case e_tk::less_than_or_equal_:
		
	case e_tk::greater_than_or_equal_:
		
	case e_tk::three_way_comparison_:
		return e_operation::binary_;
		break;
	case e_tk::open_paren_:
		
	case e_tk::close_paren_:
		
	case e_tk::open_brace_:
		
	case e_tk::close_brace_:
		
	case e_tk::open_bracket_:
		
	case e_tk::close_bracket_:
		return e_operation::postfix_;
		break;
	case e_tk::semicolon_:
		
	case e_tk::colon_:
		
	case e_tk::comma_:
		return e_operation::none_;
		break;
	case e_tk::period_:
		return e_operation::binary_;
		break;
	case e_tk::ellipsis_:
		return e_operation::binary_;
		break;
	case e_tk::commerical_at_:
		return e_operation::prefix_;
		break;
	case e_tk::none_literal_:
		
	case e_tk::true_literal_:
		
	case e_tk::false_literal_:
		
	case e_tk::type_:
		
	case e_tk::identity_:
		
	case e_tk::value_:
		
	case e_tk::int_:
		
	case e_tk::uint_:
		
	case e_tk::real_:
		
	case e_tk::byte_:
		
	case e_tk::bit_:
		
	case e_tk::str_:
		
	case e_tk::array_:
		
	case e_tk::pointer_:
		
	case e_tk::memory_:
		
	case e_tk::function_:
		
	case e_tk::include_:
		
	case e_tk::macro_:
		
	case e_tk::endmacro_:
		
	case e_tk::enter_:
		
	case e_tk::start_:
		
	case e_tk::use_:
		
	case e_tk::class_:
		
	case e_tk::obj_:
		
	case e_tk::print_:
		
	case e_tk::private_:
		
	case e_tk::public_:
		
	case e_tk::const_:
		
	case e_tk::static_:
		
	case e_tk::ref_:
		
	case e_tk::if_:
		
	case e_tk::else_:
		
	case e_tk::elif_:
		
	case e_tk::while_:
		
	case e_tk::for_:
		
	case e_tk::switch_:
		
	case e_tk::case_:
		
	case e_tk::default_:
		
	case e_tk::break_:
		
	case e_tk::continue_:
		
	case e_tk::return_:
		return e_operation::none_;
		break;
	default:
		return e_operation::none_;
		break;
	}
}
SL_CX priority::e_priority tk_type_priority(e_tk t) {
	switch (t)
	{
	case e_tk::none_:
	case e_tk::invalid_:
	case e_tk::eof_:
	case e_tk::line_comment_:
	case e_tk::block_comment_:
	case e_tk::newline_:
	case e_tk::whitespace_:
		return priority::e_priority::none_;
		break;
	case e_tk::string_literal_:
	case e_tk::number_literal_:
	case e_tk::real_literal_:
	case e_tk::byte_literal_:
	case e_tk::bit_literal_:
	case e_tk::unsigned_literal_:
	case e_tk::alnumus_:
		return priority::e_priority::max_;
		break;
	case e_tk::simple_assignment_:		
	case e_tk::addition_assignment_:		
	case e_tk::subtraction_assignment_:		
	case e_tk::multiplication_assignment_:		
	case e_tk::division_assignment_:		
	case e_tk::remainder_assignment_:		
	case e_tk::bitwise_and_assignment_:		
	case e_tk::bitwise_or_assignment_:		
	case e_tk::bitwise_xor_assignment_:		
	case e_tk::left_shift_assignment_:		
	case e_tk::right_shift_assignment_:
		return priority::e_priority::assignment_;
		break;
	case e_tk::increment_:
	case e_tk::decrement_:
		return priority::e_priority::postfix_;
		break;
	case e_tk::addition_:
	case e_tk::subtraction_:
		return priority::e_priority::term_;
		break;
	case e_tk::multiplication_:
	case e_tk::division_:
	case e_tk::remainder_:
		return priority::e_priority::factor_;
		break;
	case e_tk::negation_:
	case e_tk::bitwise_not_:
		return priority::e_priority::prefix_;
		break;
	case e_tk::bitwise_and_:		
	case e_tk::bitwise_or_:		
	case e_tk::bitwise_xor_:		
	case e_tk::bitwise_left_shift_:		
	case e_tk::bitwise_right_shift_:		
	case e_tk::logical_and_:		
	case e_tk::logical_or_:		
	case e_tk::equal_:		
	case e_tk::not_equal_:		
	case e_tk::less_than_:		
	case e_tk::greater_than_:		
	case e_tk::less_than_or_equal_:		
	case e_tk::greater_than_or_equal_:		
	case e_tk::three_way_comparison_:
		return priority::e_priority::comparison_;
		break;
	case e_tk::open_paren_:
	case e_tk::close_paren_:
	case e_tk::open_brace_:
	case e_tk::close_brace_:
	case e_tk::open_bracket_:
	case e_tk::close_bracket_:
		return priority::e_priority::postfix_;
		break;
	case e_tk::semicolon_:
	case e_tk::colon_:
	case e_tk::comma_:
	case e_tk::ellipsis_:
	case e_tk::commerical_at_:
		return priority::e_priority::max_;
		break;
	case e_tk::period_:
		return priority::e_priority::access_;
		break;
	case e_tk::none_literal_:		
	case e_tk::true_literal_:		
	case e_tk::false_literal_:		
	case e_tk::type_:		
	case e_tk::identity_:		
	case e_tk::value_:		
	case e_tk::int_:		
	case e_tk::uint_:		
	case e_tk::real_:		
	case e_tk::byte_:		
	case e_tk::bit_:		
	case e_tk::str_:		
	case e_tk::array_:		
	case e_tk::pointer_:		
	case e_tk::memory_:		
	case e_tk::function_:		
	case e_tk::include_:		
	case e_tk::macro_:		
	case e_tk::endmacro_:		
	case e_tk::enter_:		
	case e_tk::start_:		
	case e_tk::use_:		
	case e_tk::class_:		
	case e_tk::obj_:		
	case e_tk::print_:		
	case e_tk::private_:		
	case e_tk::public_:		
	case e_tk::const_:		
	case e_tk::static_:		
	case e_tk::ref_:		
	case e_tk::if_:		
	case e_tk::else_:		
	case e_tk::elif_:		
	case e_tk::while_:		
	case e_tk::for_:		
	case e_tk::switch_:		
	case e_tk::case_:		
	case e_tk::default_:		
	case e_tk::break_:		
	case e_tk::continue_:		
	case e_tk::return_:
		return priority::e_priority::max_;
	default:
		break;
	}
}

SL_CX e_assoc ast_type_assoc(e_ast a) {
return e_assoc::none_;
}
SL_CX e_operation ast_type_operation(e_ast a) {
	return e_operation::none_;
}
SL_CX priority::e_priority ast_type_priority(e_ast a) {
	return priority::e_priority::none_;
}

SL_CX bool tk_type_is_keyword(e_tk t) {
	switch (t) {
	case e_tk::include_:
	case e_tk::macro_:
	case e_tk::endmacro_:
	case e_tk::enter_:
	case e_tk::start_:
	case e_tk::use_:
	case e_tk::class_:
	case e_tk::obj_:
	case e_tk::private_:
	case e_tk::public_:
	case e_tk::const_:
	case e_tk::static_:
	case e_tk::if_:
	case e_tk::else_:
	case e_tk::elif_:
	case e_tk::while_:
	case e_tk::for_:
	case e_tk::switch_:
	case e_tk::case_:
	case e_tk::default_:
	case e_tk::break_:
	case e_tk::continue_:
	case e_tk::return_:
	case e_tk::print_:
	case e_tk::none_literal_:
	case e_tk::true_literal_:
	case e_tk::false_literal_:
	case e_tk::type_:
	case e_tk::identity_:
	case e_tk::value_:
	case e_tk::int_:
	case e_tk::uint_:
	case e_tk::real_:
	case e_tk::byte_:
	case e_tk::bit_:
	case e_tk::str_:
		return true;
	default:
		return false;
	}
}
SL_CX bool ast_type_is_keyword(e_ast a) {
	switch (a) {
	case e_ast::include_:
	case e_ast::macro_:
	case e_ast::endmacro_:
	case e_ast::enter_:
	case e_ast::start_:
	case e_ast::use_:
	case e_ast::class_:
	case e_ast::obj_:
	case e_ast::private_:
	case e_ast::public_:
	case e_ast::const_:
	case e_ast::static_:
	case e_ast::if_:
	case e_ast::else_:
	case e_ast::elif_:
	case e_ast::while_:
	case e_ast::for_:
	case e_ast::switch_:
	case e_ast::case_:
	case e_ast::default_:
	case e_ast::break_:
	case e_ast::continue_:
	case e_ast::return_:
	case e_ast::print_:
	case e_ast::none_literal_:
	case e_ast::true_literal_:
	case e_ast::false_literal_:
	case e_ast::type_:
	case e_ast::identity_:
	case e_ast::value_:
	case e_ast::int_:
	case e_ast::uint_:
	case e_ast::real_:
	case e_ast::byte_:
	case e_ast::bit_:
	case e_ast::str_:
		return true;
	default:
		return false;
	}
}
SL_CX bool tk_type_is_opening_scope(e_tk t) {
	switch (t)
	{
	case(e_tk::open_paren_):
	case(e_tk::open_brace_):
	case(e_tk::open_bracket_):
		return true;
	default:
		return false;
	}
}
SL_CX bool tk_type_is_closing_scope(e_tk t) {
	switch (t)
	{
	case(e_tk::close_paren_):
	case(e_tk::close_brace_):
	case(e_tk::close_bracket_):
		return true;
	default:
		return false;
	}
}
SL_CX bool tk_type_is_closing_scope_of(e_tk topen, e_tk tclose) {
	return (topen == e_tk::open_paren_ && tclose == e_tk::close_paren_) ||
		(topen == e_tk::open_brace_ && tclose == e_tk::close_brace_) ||
		(topen == e_tk::open_bracket_ && tclose == e_tk::close_bracket_);
}

class tk {
private:
	e_tk type_{ e_tk::invalid_ };
	sl_u8string literal_{u8""};
	sl_size line_{ 0 };
	sl_size col_{ 0 };
public:
	// Modifiers
	SL_CX void set_line(sl_size line) { line_ = line; }
	SL_CX void set_col(sl_size col) { col_ = col; }
	// Properties
	SL_CXA type() const noexcept { return type_; }
	SL_CXA node_type() const noexcept { return tk_type_to_astnode_type(type_); }
	SL_CXA size() const { return literal_.size(); }
	SL_CXA line() const noexcept { return line_; }
	SL_CXA col() const noexcept { return col_; }
	SL_CX const sl_u8string& literal() const {return literal_;}

	// Parsing Utilities
	SL_CXA priority() const { return tk_type_priority(type_); };
	SL_CXA assoc() const{ return tk_type_assoc(type_); }
	SL_CXA operation() const { return tk_type_operation(type_); }

	// Fast type queries.
	SL_CXA type_is(e_tk type) const noexcept {	return type_ == type; }
	SL_CXA type_and_lit_is(e_tk kind, const sl_u8string& literal)const {
		return type_ == kind && literal_ == literal;
	}
	SL_CXA is_keyword() const noexcept { return tk_type_is_keyword(type_); }
	SL_CXA is_opening_scope() const noexcept { return tk_type_is_opening_scope(type_); }
	SL_CXA is_closing_scope() const noexcept { return tk_type_is_closing_scope(type_); }
	SL_CXA is_closing_scope_of(e_tk topen)const noexcept {
		return tk_type_is_closing_scope_of(topen, type_);
	}

	// Error Handling/Testing.
	SL_CXA literal_str() const {
		return sl::to_str(literal_);
	}

public:
	SL_CX tk() noexcept : type_(e_tk::none_), line_(0), col_(0), literal_() {}
	SL_CX tk(e_tk type) noexcept : type_(type), line_(0), col_(0) {}
	SL_CX tk(e_tk type, sl_char8_vector_cit beg, sl_char8_vector_cit end) noexcept
		: type_(type), line_(0), col_(0) {
		literal_ = sl_u8string(beg, end);
	}
	SL_CX tk(e_tk type, sl_char8_vector_cit beg, sl_char8_vector_cit end, sl_size line, sl_size col) noexcept
		: type_(type), line_(line), col_(col) {
		literal_ = sl_u8string(beg, end);
	}
	SL_CX tk(e_tk type, sl_u8string literal)
		: type_(type), line_(0), col_(0), literal_(literal) {}
	SL_CX tk(e_tk type, sl_u8string literal, sl_size line, sl_size col)
		: type_(type), line_(line), col_(col), literal_(literal) {}

	SL_CX tk(const tk& other) noexcept
		: type_(other.type_), line_(other.line_), col_(other.col_), literal_(other.literal_) {}
	SL_CX tk(tk&& other) noexcept
		: type_(other.type_), line_(other.line_), col_(other.col_), literal_(std::move(other.literal_)) {}
	SL_CXA operator=(const tk& other) noexcept {
		type_ = other.type_;
		line_ = other.line_;
		col_ = other.col_;
		literal_ = other.literal_;
		return *this;
	}
	SL_CXA operator=(tk&& other) noexcept {
		type_ = other.type_;
		line_ = other.line_;
		col_ = other.col_;
		literal_ = std::move(other.literal_);
		return *this;
	}
	SL_CX bool operator==(const tk& rhs) const {
		return type_ == rhs.type() && literal() == rhs.literal();
	};
	SL_CX bool operator!=(const tk& rhs) const {
		return !(*this == rhs);
	};
};

class ast {
private:
	e_ast type_{e_ast::invalid_};
	sl_u8string literal_{ u8"" };
	ast* parent_{nullptr};
	sl_list<ast> children_;

	void set_parent(ast* parent) { parent_ = parent; }
	void pop_parent() { if(parent_ != nullptr) parent_=nullptr; else throw std::out_of_range("ast node pop_parent() called on node with no parent.");}
public:
	ast() : type_(e_ast::invalid_) {}
	ast(e_ast type) : type_(type) {}
	ast(e_ast type, const sl_u8string& literal) : type_(type), literal_(literal) {}
	ast(e_ast type, sl_u8string&& literal) : type_(type), literal_(std::move(literal)) {}
	ast(e_ast type, const char8_t* literal) : type_(type), literal_(literal) {}
	template<sl_size LIT_SIZE> ast(e_ast type, const char8_t literal[LIT_SIZE]) : type_(type), literal_(literal) {}

	ast(const tk& t) : type_(tk_type_to_astnode_type(t.type())), literal_(t.literal()) {}
	ast(e_ast type, sl_vector<tk>::iterator beg, sl_vector<tk>::iterator end)
		: type_(type) {
		literal_ = u8"";
		for (auto it = beg; it != end; it++) {
			literal_ += it->literal();
		}
	}	
	template<typename...  ChildTs> requires (std::is_same_v<ast, std::decay_t<ChildTs>> && ...)
	ast(e_ast type, const sl_u8string& literal, ChildTs... children) : type_(type), literal_(literal) {
		(push_back(children), ...);
	}
	template<typename...  ChildTs> requires (std::is_same_v<ast, std::decay_t<ChildTs>> && ...)
	ast(e_ast type, sl_vector<tk>::iterator beg, sl_vector<tk>::iterator end, ChildTs... children) : type_(type) {
		literal_ = u8"";
		for (auto it = beg; it != end; it++) {
			literal_ += it->literal();
		}
		(push_back(children), ...);
	}


	SL_CX e_ast type() const noexcept { return type_; }
	SL_CX const sl_u8string& literal() const noexcept { return literal_; }
	bool leaf() const noexcept { return children_.empty(); }
	bool root() const noexcept { return parent_ == nullptr; }
	bool branch() const noexcept { return !children_.empty(); }

	// Parent operations.
	ast& parent() { if (parent_ != nullptr) return *parent_; else throw std::out_of_range("ast node parent() called on node with no parent."); }

	// Child operations.
	sl_size size() const { return children_.size(); }
	ast& push_back(const ast& nd) { children_.push_back(nd); auto& pushed = children_.back(); pushed.set_parent(this); return pushed; }
	ast& push_front(const ast & nd) { children_.push_front(nd); auto& pushed = children_.front(); pushed.set_parent(this); return pushed;}
	ast& push_back(ast && nd) { children_.push_back(std::move(nd)); auto& pushed = children_.back(); pushed.set_parent(this); return pushed;}
	ast& push_front(ast && nd) { children_.push_front(std::move(nd)); auto& pushed = children_.front(); pushed.set_parent(this); return pushed;}
	ast pop_back() { 
		if (!children_.empty()) {
			auto popped = children_.back();
			children_.pop_back();
			popped.pop_parent();
			return popped;
		}
		else 
			throw std::out_of_range("ast node pop_back() called on node with no children.");}
	ast pop_front() { 
		if (!children_.empty()) {
			auto popped = children_.front();
			children_.pop_front();
			popped.pop_parent();
			return popped;
		}
		else 
			throw std::out_of_range("ast node pop_front() called on node with no children.");}
	ast& front() { if (!children_.empty()) return children_.front(); else throw std::out_of_range("ast node front() called on node with no children."); }
	ast& back() { if (!children_.empty()) return children_.back(); else throw std::out_of_range("ast node front() called on node with no children."); }
	ast& at(sl_size index) {
		if (index >= children_.size()) throw std::out_of_range("ast node at() called with index out of range.");
		if (index == 0) return children_.front();

		return [this, &index]()->ast& {auto it = children_.begin();
		for (sl_size i = 0; i < index; i++) { it++; } return *it;
		}();
	}
	// Index operator accesses children.
	const ast& operator[](sl_size index) const {
		if (index >= children_.size()) throw std::out_of_range("ast node [] index operator called with index out of range.");
		if (index == 0) return children_.front();

		return [this, &index]()->const ast& {auto it = children_.begin();
		for (sl_size i = 0; i < index; i++) { it++; } return *it;
		}();
	}
	ast& operator[](sl_size index){
		if (index >= children_.size()) throw std::out_of_range("ast node [] index operator called with index out of range.");
		if (index == 0) return children_.front();

		return [this, &index]()->ast& {auto it = children_.begin();
		for (sl_size i = 0; i < index; i++) { it++; } return *it;
		}();
	}

	// Fast type queries.
	SL_CX bool type_is(e_ast type) const noexcept { return type_ == type; }
	SL_CX bool is_keyword() const noexcept { return ast_type_is_keyword(type_); }

	// Error Handling/Testing.
	SL_CX sl_string literal_str() const {
		return sl::to_str(literal_);
	}
	const auto& children() const { return children_; }
};

using tk_vector = sl_vector<tk>;
using tk_vector_it = sl_vector<tk>::iterator;
using tk_vector_cit = sl_vector<tk>::const_iterator;