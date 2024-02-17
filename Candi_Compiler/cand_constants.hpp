#pragma once
#include "global_dependencies.hpp"

// U8 String Constants
namespace caoco {
	namespace ca_constant {
		namespace u8 {
			namespace chars {
				SL_CXA HASH = u8'#';
				SL_CXA ADD = u8'+';
				SL_CXA SUB = u8'-';
				SL_CXA MUL = u8'*';
				SL_CXA DIV = u8'/';
				SL_CXA MOD = u8'%';
				SL_CXA AND = u8'&';
				SL_CXA OR = u8'|';
				SL_CXA EQ = u8'=';



			}
			namespace keywords {
				SL_CXA INCLUDE = u8"include";
				SL_CXA MACRO = u8"macro";
				SL_CXA ENTER = u8"enter";
				SL_CXA START = u8"start";
				SL_CXA TYPE = u8"type";
				SL_CXA VAR = u8"var";
				SL_CXA CLASS = u8"class";
				SL_CXA OBJECT = u8"obj";
				SL_CXA PRIVATE = u8"private";
				SL_CXA PUBLIC = u8"public";
				SL_CXA FUNC = u8"func";
				SL_CXA CONST = u8"const";
				SL_CXA STATIC = u8"static";
				SL_CXA IF = u8"if";
				SL_CXA ELSE = u8"else";
				SL_CXA ELIF = u8"elif";
				SL_CXA WHILE = u8"while";
				SL_CXA FOR = u8"for";
				SL_CXA ON = u8"on";
				SL_CXA BREAK = u8"break";
				SL_CXA CONTINUE = u8"continue";
				SL_CXA RETURN = u8"return";

				SL_CXA PRINT = u8"print";
				SL_CXA NONE = u8"none";

				SL_CXA INT = u8"int";
				SL_CXA UINT = u8"uint";
				SL_CXA REAL = u8"real";
				SL_CXA BYTE = u8"byte";
				SL_CXA BIT = u8"bit";
				SL_CXA STR = u8"str";

				SL_CX sl_ilist all = { INCLUDE, MACRO, ENTER, START, TYPE, VAR, CLASS, OBJECT, PRIVATE, PUBLIC, FUNC, CONST,
						STATIC, IF, ELSE, ELIF, WHILE, FOR, ON, BREAK, CONTINUE, RETURN, PRINT, NONE, INT,
						UINT, REAL, BYTE, BIT, STR };
			}
			namespace directives {
				SL_CXA INCLUDE = u8"#include";
				SL_CXA MACRO = u8"#macro";
				SL_CXA ENTER = u8"#enter";
				SL_CXA START = u8"#start";
				SL_CXA TYPE = u8"#type";
				SL_CXA VAR = u8"#var";
				SL_CXA CLASS = u8"#class";
				SL_CXA OBJECT = u8"#obj";
				SL_CXA PRIVATE = u8"#private";
				SL_CXA PUBLIC = u8"#public";
				SL_CXA FUNC = u8"#func";
				SL_CXA CONST = u8"#const";
				SL_CXA STATIC = u8"#static";
				SL_CXA IF = u8"#if";
				SL_CXA ELSE = u8"#else";
				SL_CXA ELIF = u8"#elif";
				SL_CXA WHILE = u8"#while";
				SL_CXA FOR = u8"#for";
				SL_CXA ON = u8"#on";
				SL_CXA BREAK = u8"#break";
				SL_CXA CONTINUE = u8"#continue";
				SL_CXA RETURN = u8"#return";

				SL_CXA PRINT = u8"#print";
				SL_CXA NONE = u8"#none";

				SL_CXA INT = u8"#int";
				SL_CXA UINT = u8"#uint";
				SL_CXA REAL = u8"#real";
				SL_CXA BYTE = u8"#byte";
				SL_CXA BIT = u8"#bit";
				SL_CXA STR = u8"#str";

				SL_CX sl_ilist all = { INCLUDE, MACRO, ENTER, START, TYPE, VAR, CLASS, OBJECT, PRIVATE, PUBLIC, FUNC, CONST,
						STATIC, IF, ELSE, ELIF, WHILE, FOR, ON, BREAK, CONTINUE, RETURN, PRINT, NONE, INT,
						UINT, REAL, BYTE, BIT, STR };
			}
			namespace operators {
				SL_CXA HASH = u8"#";
				SL_CXA ADD = u8"+";
				SL_CXA SUB = u8"-";
				SL_CXA MUL = u8"*";
				SL_CXA DIV = u8"/";
				SL_CXA MOD = u8"%";
				SL_CXA AND = u8"&";
				SL_CXA OR = u8"|";
				SL_CXA XOR = u8"^";
				SL_CXA NOT = u8"!";
				SL_CXA LSH = u8"<<";
				SL_CXA RSH = u8">>";
				SL_CXA EQ = u8"==";
				SL_CXA NEQ = u8"!=";
				SL_CXA LT = u8"<";
				SL_CXA GT = u8">";
				SL_CXA LTE = u8"<=";
				SL_CXA GTE = u8">=";
				SL_CXA ASSIGN = u8"=";
				SL_CXA NEW_ASSIGN = u8":=";
				SL_CXA ADD_ASSIGN = u8"+=";
				SL_CXA SUB_ASSIGN = u8"-=";
				SL_CXA MUL_ASSIGN = u8"*=";
				SL_CXA DIV_ASSIGN = u8"/=";
				SL_CXA MOD_ASSIGN = u8"%=";
				SL_CXA AND_ASSIGN = u8"&=";
				SL_CXA OR_ASSIGN = u8"|=";
				SL_CXA XOR_ASSIGN = u8"^=";
				SL_CXA LSH_ASSIGN = u8"<<=";
				SL_CXA RSH_ASSIGN = u8">>=";
				SL_CXA INC = u8"++";
				SL_CXA DEC = u8"--";
				SL_CXA DOT = u8".";
				SL_CXA BNOT = u8"~";
				SL_CXA BAND = u8"&&";
				SL_CXA BOR = u8"||";

				SL_CX sl_ilist all = { ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, NOT, LSH, RSH, EQ, NEQ, LT, GT
					, LTE, GTE, ASSIGN, NEW_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN
				, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, LSH_ASSIGN, RSH_ASSIGN, INC, DEC, DOT, BNOT, BAND, BOR };

			}
			namespace scopes {
				SL_CXA LPAREN = u8"(";
				SL_CXA RPAREN = u8")";
				SL_CXA LBRACE = u8"{";
				SL_CXA RBRACE = u8"}";
				SL_CXA LBRACKET = u8"[";
				SL_CXA RBRACKET = u8"]";

				SL_CX sl_ilist all = { LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET };
			}
			namespace seperators {
				SL_CXA COMMA = u8",";
				SL_CXA COLON = u8":";
				SL_CXA SEMICOLON = u8";";

				SL_CX sl_ilist all = { COMMA, COLON, SEMICOLON };
			}
			namespace other {
				SL_CXA EOFILE = u8"\0";
				SL_CXA WHITESPACE = u8" ";
				SL_CXA NEWLINE = u8"\n";
				SL_CXA BLOCK_COMMENT = u8"///";
				SL_CXA LINE_COMMENT = u8"//";

				SL_CX sl_ilist<const char8_t* > all = { EOFILE, WHITESPACE, NEWLINE, BLOCK_COMMENT, LINE_COMMENT };
			}
			SL_CXS char8_t EOF_CHAR = '\0';
		}
		namespace keywords {
			SL_CXA INCLUDE = "include";
			SL_CXA MACRO = "macro";
			SL_CXA ENTER = "enter";
			SL_CXA START = "start";
			SL_CXA TYPE = "type";
			SL_CXA VAR = "var";
			SL_CXA CLASS = "class";
			SL_CXA OBJECT = "#obj";
			SL_CXA PRIVATE = "private";
			SL_CXA PUBLIC = "public";
			SL_CXA FUNC = "func";
			SL_CXA CONST = "const";
			SL_CXA STATIC = "static";
			SL_CXA IF = "if";
			SL_CXA ELSE = "else";
			SL_CXA ELIF = "elif";
			SL_CXA WHILE = "while";
			SL_CXA FOR = "for";
			SL_CXA ON = "on";
			SL_CXA BREAK = "break";
			SL_CXA CONTINUE = "continue";
			SL_CXA RETURN = "return";

			SL_CXA PRINT = "print";
			SL_CXA NONE = "none";

			SL_CXA INT = "int";
			SL_CXA UINT = "uint";
			SL_CXA REAL = "real";
			SL_CXA BYTE = "byte";
			SL_CXA BIT = "bit";
			SL_CXA STR = "str";

			SL_CX sl_ilist all = { INCLUDE, MACRO, ENTER, START, TYPE, VAR, CLASS, OBJECT, PRIVATE, PUBLIC, FUNC, CONST,
					STATIC, IF, ELSE, ELIF, WHILE, FOR, ON, BREAK, CONTINUE, RETURN, PRINT, NONE, INT,
					UINT, REAL, BYTE, BIT, STR };
		}
		namespace directives {
			SL_CXA INCLUDE = "#include";
			SL_CXA DEFINE = "#macro";
			SL_CXA ENTER = "#enter";
			SL_CXA START = "#start";
			SL_CXA TYPE = "#type";
			SL_CXA VAR = "#var";
			SL_CXA CLASS = "#class";
			SL_CXA OBJECT = "#obj";
			SL_CXA PRIVATE = "private";
			SL_CXA PUBLIC = "public";
			SL_CXA FUNC = "#func";
			SL_CXA CONST = "#const";
			SL_CXA STATIC = "#static";
			SL_CXA IF = "#if";
			SL_CXA ELSE = "#else";
			SL_CXA ELIF = "#elif";
			SL_CXA WHILE = "#while";
			SL_CXA FOR = "#for";
			SL_CXA ON = "#on";
			SL_CXA BREAK = "#break";
			SL_CXA CONTINUE = "#continue";
			SL_CXA RETURN = "#return";

			SL_CXA PRINT = "#print";
			SL_CXA NONE = "#none";

			SL_CXA INT = "#int";
			SL_CXA UINT = "#uint";
			SL_CXA REAL = "#real";
			SL_CXA BYTE = "#byte";
			SL_CXA BIT = "#bit";
			SL_CXA STR = "#str";

			SL_CX sl_ilist all = { INCLUDE, DEFINE, ENTER, START, TYPE, VAR, CLASS, OBJECT, PRIVATE, PUBLIC, FUNC, CONST,
					STATIC, IF, ELSE, ELIF, WHILE, FOR, ON, BREAK, CONTINUE, RETURN, PRINT, NONE, INT,
					UINT, REAL, BYTE, BIT, STR };
		}
		namespace operators {
			SL_CXA ADD = "+";
			SL_CXA SUB = "-";
			SL_CXA MUL = "*";
			SL_CXA DIV = "/";
			SL_CXA MOD = "%";
			SL_CXA AND = "&";
			SL_CXA OR = "|";
			SL_CXA XOR = "^";
			SL_CXA NOT = "!";
			SL_CXA LSH = "<<";
			SL_CXA RSH = ">>";
			SL_CXA EQ = "==";
			SL_CXA NEQ = "!=";
			SL_CXA LT = "<";
			SL_CXA GT = ">";
			SL_CXA LTE = "<=";
			SL_CXA GTE = ">=";
			SL_CXA ASSIGN = "=";
			SL_CXA NEW_ASSIGN = ":=";
			SL_CXA ADD_ASSIGN = "+=";
			SL_CXA SUB_ASSIGN = "-=";
			SL_CXA MUL_ASSIGN = "*=";
			SL_CXA DIV_ASSIGN = "/=";
			SL_CXA MOD_ASSIGN = "%=";
			SL_CXA AND_ASSIGN = "&=";
			SL_CXA OR_ASSIGN = "|=";
			SL_CXA XOR_ASSIGN = "^=";
			SL_CXA LSH_ASSIGN = "<<=";
			SL_CXA RSH_ASSIGN = ">>=";
			SL_CXA INC = "++";
			SL_CXA DEC = "--";
			SL_CXA DOT = ".";
			SL_CXA BNOT = "~";
			SL_CXA BAND = "&&";
			SL_CXA BOR = "||";

			SL_CX sl_ilist all = { ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, NOT, LSH, RSH, EQ, NEQ, LT, GT
				, LTE, GTE, ASSIGN, NEW_ASSIGN, ADD_ASSIGN, SUB_ASSIGN, MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN
			, AND_ASSIGN, OR_ASSIGN, XOR_ASSIGN, LSH_ASSIGN, RSH_ASSIGN, INC, DEC, DOT, BNOT, BAND, BOR };

		}
		namespace scopes {
			SL_CXA LPAREN = "(";
			SL_CXA RPAREN = ")";
			SL_CXA LBRACE = "{";
			SL_CXA RBRACE = "}";
			SL_CXA LBRACKET = "[";
			SL_CXA RBRACKET = "]";

			SL_CX sl_ilist all = { LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET };
		}
		namespace seperators {
			SL_CXA COMMA = ",";
			SL_CXA COLON = ":";
			SL_CXA SEMICOLON = ";";

			SL_CX sl_ilist all = { COMMA, COLON, SEMICOLON };
		}
		namespace other {
			SL_CXA EOFILE = "\0";
			SL_CXA WHITESPACE = " ";
			SL_CXA NEWLINE = "\n";
			SL_CXA BLOCK_COMMENT = "///";
			SL_CXA LINE_COMMENT = "//";

			SL_CX sl_ilist all = { EOFILE, WHITESPACE, NEWLINE, BLOCK_COMMENT, LINE_COMMENT };
		}
		SL_CXS char EOF_CHAR = '\0';
	}
}