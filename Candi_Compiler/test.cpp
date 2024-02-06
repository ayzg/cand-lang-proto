#include "pch.h"
#include <type_traits>
#include "char_traits.hpp"
#include "token.hpp"
#include "tokenizer.hpp"
#include "ast_node.hpp"
#include "parser.hpp"
//#include "transpiler.hpp"
//#include "candi_main.hpp"

//!!TODO: test for loading files, if file does not exist throw a clear exception

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Tokenizer Tests
// dependencies: tokenizer.hpp
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
auto token_type_to_string(caoco::Tk::eType type) {
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
	case(caoco::Tk::eType::aureal_): return "aureal_";
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
	default: return "This token type is not string-convertible. Please implement a string conversion for this token type in the token_type_to_string function in test.cpp.";
	}
}

auto token_to_string(const caoco::Tk& token) {
	return token_type_to_string(token.type()) + std::string(" : ") + caoco::to_std_string(token.literal());
}

using tk_etype = caoco::Tk::eType;
struct expected_token {
	caoco::Tk::eType type;
	std::string literal;
};

#define caoco_CaocoTokenizer_Tokens 1

#if caoco_CaocoTokenizer_Tokens
TEST(CaocoTokenizer_Test, CaocoTokenizer_Tokens) {
	// NOTE: the sanitized output will not contain comments/newlines/whitespace. As such there is a seperate test for those tokens.
	auto source_file = caoco::load_source_file("ut0_CaocoTokenizer_Tokens.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();
	
	for(auto& token : result)
		std::cout << "L:" << token.line() << ":" << token_to_string(token) << std::endl;
	
	// The tokens we expect to get from lexing the file.
	/*
		L:6:string_literal : 'kgfjhgjhgjkhgjk'
		L:7:string_literal : 'oogabooga\'aaaaa'
		L:8:number_literal : 1234
		L:9:real_literal : 1234.5678
		L:10:alnumus : thisisan_identifier
		L:14:type_ : #enter
		L:15:start_ : #start
		L:16:type_ : #type
		L:17:var_ : #var
		L:18:class_ : #class
		L:19:print_ : #print
		L:20:func_ : #func
		L:22:public_ : #public
		L:23:const_ : #const
		L:24:static_ : #static
		L:25:ref_ : #ref
		L:27:if_ : #if
		L:28:else_ : #else
		L:29:elif_ : #elif
		L:30:while_ : #while
		L:31:for_ : #for
		L:32:switch_ : #switch
		L:33:case_ : #case
		L:34:default_ : #default
		L:35:break_ : #break
		L:36:continue_ : #continue
		L:37:return_ : #return
		L:38:into_ : #into
		L:41:atype_ : &type
		L:42:avalue_ : &value
		L:43:aint_ : &int
		L:43:open_frame : [
		L:43:alnumus : RANGE
		L:43:open_frame : [
		L:43:subtraction : -
		L:43:alnumus : inf
		L:43:subtraction : -
		L:43:alnumus : inf
		L:43:close_frame : ]
		L:43:close_frame : ]
		L:44:auint_ : &uint
		L:44:open_frame : [
		L:44:alnumus : RANGE
		L:44:open_frame : [
		L:44:number_literal : 0
		L:44:subtraction : -
		L:44:alnumus : inf
		L:44:close_frame : ]
		L:44:close_frame : ]
		L:45:areal_ : &real
		L:45:open_frame : [
		L:45:alnumus : RANGE
		L:45:open_frame : [
		L:45:subtraction : -
		L:45:alnumus : inf
		L:45:subtraction : -
		L:45:alnumus : inf
		L:45:close_frame : ]
		L:45:close_frame : ]
		L:46:aureal_ : &ureal
		L:46:open_frame : [
		L:46:alnumus : RANGE
		L:46:open_frame : [
		L:46:number_literal : 0
		L:46:subtraction : -
		L:46:alnumus : inf
		L:46:close_frame : ]
		L:46:close_frame : ]
		L:47:aoctet_ : &octet
		L:47:open_frame : [
		L:47:alnumus : RANGE
		L:47:open_frame : [
		L:47:number_literal : 0
		L:47:subtraction : -
		L:47:number_literal : 255
		L:47:close_frame : ]
		L:47:close_frame : ]
		L:48:abit_ : &bit
		L:48:open_frame : [
		L:48:alnumus : RANGE
		L:48:open_frame : [
		L:48:number_literal : 0
		L:48:subtraction : -
		L:48:number_literal : 1
		L:48:close_frame : ]
		L:48:close_frame : ]
		L:49:aarray_ : &array
		L:49:open_frame : [
		L:49:atype_ : &type
		L:49:comma : ,
		L:49:auint_ : &uint
		L:49:close_frame : ]
		L:50:apointer_ : &pointer
		L:50:open_frame : [
		L:50:atype_ : &type
		L:50:close_frame : ]
		L:51:amemory_ : &memory
		L:51:open_frame : [
		L:51:atype_ : &type
		L:51:comma : ,
		L:51:auint_ : &uint
		L:51:close_frame : ]
		L:52:afunction_ : &function
		L:55:alnumus : a
		L:55:period : .
		L:55:alnumus : b
		L:56:alnumus : a
		L:56:ellipsis : ...
		L:56:alnumus : b
		L:59:var_ : #var
		L:59:alnumus : a
		L:59:simple_assignment : =
		L:59:alnumus : b
		L:59:eos : ;
		L:62:alnumus : a
		L:62:simple_assignment : =
		L:62:alnumus : b
		L:63:alnumus : a
		L:63:addition_assignment : +=
		L:63:alnumus : b
		L:64:alnumus : a
		L:64:subtraction_assignment : -=
		L:64:alnumus : b
		L:65:alnumus : a
		L:65:multiplication_assignment : *=
		L:65:alnumus : b
		L:66:alnumus : a
		L:66:division_assignment : /=
		L:66:alnumus : b
		L:67:alnumus : a
		L:67:remainder_assignment : %=
		L:67:alnumus : b
		L:68:alnumus : a
		L:68:bitwise_and_assignment : &=
		L:68:alnumus : b
		L:69:alnumus : a
		L:69:bitwise_or_assignment : |=
		L:69:alnumus : b
		L:70:alnumus : a
		L:70:bitwise_xor_assignment : ^=
		L:70:alnumus : b
		L:71:alnumus : a
		L:71:left_shift_assignment : <<=
		L:71:alnumus : b
		L:72:alnumus : a
		L:72:right_shift_assignment : >>=
		L:72:alnumus : b
		L:73:increment : ++
		L:73:alnumus : a
		L:74:decrement : --
		L:74:alnumus : a
		L:75:alnumus : a
		L:75:addition : +
		L:75:alnumus : b
		L:76:alnumus : a
		L:76:subtraction : -
		L:76:alnumus : b
		L:77:alnumus : a
		L:77:multiplication : *
		L:77:alnumus : b
		L:78:alnumus : a
		L:78:division : /
		L:78:alnumus : b
		L:79:alnumus : a
		L:79:remainder : %
		L:79:alnumus : b
		L:80:bitwise_NOT : ~
		L:80:alnumus : a
		L:81:alnumus : a
		L:81:bitwise_AND : &
		L:81:alnumus : b
		L:82:alnumus : a
		L:82:bitwise_OR : |
		L:82:alnumus : b
		L:83:alnumus : a
		L:83:bitwise_XOR : ^
		L:83:alnumus : b
		L:84:alnumus : a
		L:84:bitwise_left_shift : <<
		L:84:alnumus : b
		L:85:alnumus : a
		L:85:bitwise_right_shift : >>
		L:85:alnumus : b
		L:86:negation : !
		L:86:alnumus : a
		L:87:alnumus : a
		L:87:logical_AND : &&
		L:87:alnumus : b
		L:88:alnumus : a
		L:88:logical_OR : ||
		L:88:alnumus : b
		L:89:alnumus : a
		L:89:equal : ==
		L:89:alnumus : b
		L:90:alnumus : a
		L:90:not_equal : !=
		L:90:alnumus : b
		L:91:alnumus : a
		L:91:less_than : <
		L:91:alnumus : b
		L:92:alnumus : a
		L:92:greater_than : >
		L:92:alnumus : b
		L:93:alnumus : a
		L:93:less_than_or_equal : <=
		L:93:alnumus : b
		L:94:alnumus : a
		L:94:greater_than_or_equal : >=
		L:94:alnumus : b
		L:95:alnumus : a
		L:95:less_than_or_equal : <=
		L:95:greater_than : >
		L:95:alnumus : b
		L:98:open_scope : (
		L:98:alnumus : a
		L:98:close_scope : )
		L:99:open_frame : [
		L:99:alnumus : b
		L:99:close_frame : ]
		L:100:open_list : {
		L:100:alnumus : c
		L:100:close_list : }
		L:109:type_ : #enter
		L:109:open_scope : (
		L:109:close_scope : )
		L:109:open_scope : (
		L:109:close_scope : )
		L:110:start_ : #start
		L:110:open_scope : (
		L:110:close_scope : )
		L:110:open_scope : (
		L:110:close_scope : )
		L:113:type_ : #enter
		L:113:open_scope : (
		L:113:close_scope : )
		L:113:open_scope : (
		L:113:close_scope : )
		L:114:start_ : #start
		L:114:open_scope : (
		L:114:close_scope : )
		L:114:open_scope : (
		L:116:print_ : #print
		L:116:open_scope : (
		L:116:string_literal : 'Hello, World!'
		L:116:close_scope : )
		L:117:close_scope : )
		L:120:type_ : #enter
		L:120:open_scope : (
		L:120:close_scope : )
		L:120:open_scope : (
		L:122:print_ : #print
		L:122:open_scope : (
		L:122:string_literal : 'Hello, World!'
		L:122:close_scope : )
		L:123:close_scope : )
		L:124:start_ : #start
		L:124:open_scope : (
		L:124:close_scope : )
		L:124:open_scope : (
		L:124:close_scope : )
		L:125:eof :
	*/
	std::vector<expected_token> expected = {
		expected_token(tk_etype::string_literal,"'kgfjhgjhgjkhgjk'"),
		expected_token(tk_etype::string_literal,"'oogabooga\\\'aaaaa'"),
		expected_token(tk_etype::number_literal,"1234"),
		expected_token(tk_etype::real_literal,"1234.5678"),
		expected_token(tk_etype::alnumus,"thisisan_identifier"),
		expected_token(tk_etype::enter_,"#enter"),
		expected_token(tk_etype::start_,"#start"),
		expected_token(tk_etype::type_,"#type"),
		expected_token(tk_etype::var_,"#var"),
		expected_token(tk_etype::class_,"#class"),
		expected_token(tk_etype::print_,"#print"),
		expected_token(tk_etype::func_,"#func"),
		expected_token(tk_etype::public_,"#public"),
		expected_token(tk_etype::const_,"#const"),
		expected_token(tk_etype::static_,"#static"),
		expected_token(tk_etype::ref_,"#ref"),
		expected_token(tk_etype::if_,"#if"),
		expected_token(tk_etype::else_,"#else"),
		expected_token(tk_etype::elif_,"#elif"),
		expected_token(tk_etype::while_,"#while"),
		expected_token(tk_etype::for_,"#for"),
		expected_token(tk_etype::switch_,"#switch"),
		expected_token(tk_etype::case_,"#case"),
		expected_token(tk_etype::default_,"#default"),
		expected_token(tk_etype::break_,"#break"),
		expected_token(tk_etype::continue_,"#continue"),
		expected_token(tk_etype::return_,"#return"),
		expected_token(tk_etype::into_,"#into"),
		expected_token(tk_etype::atype_,"&type"),
		expected_token(tk_etype::avalue_,"&value"),
		expected_token(tk_etype::aint_,"&int"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"RANGE"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"inf"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"inf"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::auint_,"&uint"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"RANGE"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::number_literal,"0"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"inf"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::areal_,"&real"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"RANGE"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"inf"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"inf"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::aureal_,"&ureal"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"RANGE"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::number_literal,"0"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"inf"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::aoctet_,"&octet"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"RANGE"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::number_literal,"0"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::number_literal,"255"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::abit_,"&bit"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"RANGE"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::number_literal,"0"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::number_literal,"1"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::aarray_,"&array"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::atype_,"&type"),
		expected_token(tk_etype::comma,","),
		expected_token(tk_etype::auint_,"&uint"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::apointer_,"&pointer"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::atype_,"&type"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::amemory_,"&memory"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::atype_,"&type"),
		expected_token(tk_etype::comma,","),
		expected_token(tk_etype::auint_,"&uint"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::afunction_,"&function"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::period,"."),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::ellipsis,"..."),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::var_,"#var"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::simple_assignment,"="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::eos,";"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::simple_assignment,"="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::addition_assignment,"+="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::subtraction_assignment,"-="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::multiplication_assignment,"*="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::division_assignment,"/="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::remainder_assignment,"%="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_and_assignment,"&="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_or_assignment,"|="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_xor_assignment,"^="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::left_shift_assignment,"<<="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::right_shift_assignment,">>="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::increment,"++"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::decrement,"--"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::addition,"+"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::subtraction,"-"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::multiplication,"*"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::division,"/"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::remainder,"%"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::bitwise_NOT,"~"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_AND,"&"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_OR,"|"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_XOR,"^"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_left_shift,"<<"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::bitwise_right_shift,">>"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::negation,"!"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::logical_AND,"&&"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::logical_OR,"||"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::equal,"=="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::not_equal,"!="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::less_than,"<"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::greater_than,">"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::less_than_or_equal,"<="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::greater_than_or_equal,">="),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::less_than_or_equal,"<="),
		expected_token(tk_etype::greater_than,">"),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::alnumus,"a"),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_frame,"["),
		expected_token(tk_etype::alnumus,"b"),
		expected_token(tk_etype::close_frame,"]"),
		expected_token(tk_etype::open_list,"{"),
		expected_token(tk_etype::alnumus,"c"),
		expected_token(tk_etype::close_list,"}"),
		expected_token(tk_etype::enter_,"#enter"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::start_,"#start"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::enter_,"#enter"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::start_,"#start"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::print_,"#print"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::string_literal,"'Hello, World!'"),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::enter_,"#enter"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::start_,"#start"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::open_scope,"("),
		expected_token(tk_etype::close_scope,")"),
		expected_token(tk_etype::eof,"")
};


	};
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
#define caocotest_CaocoConstantEvaluator_Literals 0

#if caocotest_CaocoConstantEvaluator_Literals
TEST(CaocoConstantEvaluator_Test, CaocoConstantEvaluator_Literals) {
	auto source_file = caoco::load_source_file("constant_evaluator_unit_test_0_literals.candi");
	auto result = caoco::tokenizer(source_file.cbegin(), source_file.cend())();

	// Create the runtime namespace/scope/environment.
	// For clarity: - All three have the same meaning in this context,but we will use the term "environment".
	//              - Namespace would clash with C++ namespace, and scope is already used in the parser.
	//              - Each environment has a parent environment, which is nullptr for the root environment(global scope).
	//              - Each environment also has a list of sub-environments, which are the child scopes.
	auto runtime_env = caoco::environment("",nullptr);
 

	// Test the constant evaluator on literals
	auto int_literal = caoco::ParseCsoInt()(result.cbegin(), result.cend());
	EXPECT_TRUE(int_literal.valid());


	auto uint_literal = caoco::ParseCsoUint()(int_literal.it(), result.cend());
	EXPECT_TRUE(uint_literal.valid());
	EXPECT_EQ(uint_literal.node().evaluate(), 42);

	auto real_literal = caoco::ParseCsoReal()(uint_literal.it(), result.cend());
	EXPECT_TRUE(real_literal.valid());
	EXPECT_EQ(real_literal.node().evaluate(), 42.42);

	auto octet_literal = caoco::ParseCsoOctet()(real_literal.it(), result.cend());
	EXPECT_TRUE(octet_literal.valid());
	EXPECT_EQ(octet_literal.node().evaluate(), 42);

	auto bit_literal = caoco::ParseCsoBit()(octet_literal.it(), result.cend());
	EXPECT_TRUE(bit_literal.valid());
	EXPECT_EQ(bit_literal.node().evaluate(), 1);

	auto string_literal = caoco::ParseStringLiteral()(bit_literal.it(), result.cend());
	EXPECT_TRUE(string_literal.valid());
	EXPECT_EQ(caoco::to_std_string(string_literal.node().evaluate()), "Hello World");

	auto number_literal = caoco::ParseNumberLiteral()(string_literal.it(), result.cend());
	EXPECT_TRUE(number_literal.valid());
	EXPECT_EQ(number_literal.node().evaluate(), 42);

	auto real_literal2 = caoco::ParseRealLiteral()(number_literal.it(), result.cend());
	EXPECT_TRUE(real_literal2.valid());
	EXPECT_EQ(real_literal2.node().evaluate(), 42.42);

	auto alnumus_literal = caoco::ParseAlnumusLiteral()(real_literal2.it(), result.cend());
	EXPECT_TRUE(alnumus_literal.valid());
	EXPECT_EQ(caoco::to_std_string(alnumus_literal.node().evaluate()), "HelloWorld");
}

#endif
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------------------------------------------------------------------------//