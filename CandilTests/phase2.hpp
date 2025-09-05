#include "phase1.hpp"

namespace cil {
	namespace p2 {
		namespace eToken {
			using enum_type = std::uint8_t;
			enum enum_ : std::uint8_t {
				invalid_,				// Invalid token. Used to indicate a syntax error.
				none_,					// No token. Used to no token is required.
				eof_,					// End of file token. Used to indicate the end of the source code.

				// Literal operators
				litop_line_comment_,	// Line comment operator. eg. '//' = p1::Solidus, p1::Solidus
				litop_block_comment_,	// Block comment operator. eg. '///' = p1::Solidus, p1::Solidus, p1::Solidus
				litop_string_,			// String literal operator. eg. '\'' = p1::Apostrophe
				litop_real_,			// Real literal operator. eg. '.' = p1::Period

				line_comment_,			// Comment token. eg. "''"
				block_comment_,			// Comment token. eg. "'''"
				integral_,				// Integral literal value 1234
				real_,					// real literal value 1234.5678
				string_,				// String literal value. 'hello'
				identity_,				// Identity value. eg. hello_world
				separator_,				// Separator token. eg. ','
				end_of_statement_,		//
				open_scope_,			// '('
				closed_scope_,          // ')'
				open_type_scope_,		// '<'
				closed_type_scope_,		// '>'
				open_list_scope_,		// '{'
				closed_list_scope_,		// '}'

				directive_operator_,	// # followed by a directive name #define
				dot_operator_,			// '.' operator
				auto_operator_,			// '@' operator

				operator_,				// Unknown Operator token. eg. '+'
				// Binary operators
				operator_plus_,			// '+'
				operator_minus_,		// '-'
				operator_multiply_,		// '*'
				operator_divide_,		// '/'
				operator_modulo_,		// '%'

				// Unrary operators
				operator_pre_increment_,	// '++'
				operator_pre_decrement_,	// '--'
				operator_post_increment_,	// '++'
				operator_post_decrement_,	// '--'
				operator_negative_,			// '-'
				operator_flip_,				// '~'

				// Assignment operators
				operator_assign_,			// '='
				operator_plus_assign_,		// '+='
				operator_minus_assign_,		// '-='
				operator_multiply_assign_,	// '*='
				operator_divide_assign_,	// '/='
				operator_modulo_assign_,	// '%='

				// Comparison operators
				operator_equal_,			// '=='
				operator_not_equal_,		// '!='
				operator_less_,				// '<'
				operator_greater_,			// '>'
				operator_less_equal_,		// '<='
				operator_greater_equal_,	// '>='

				// Logical operators
				operator_logical_and_,		// '&&'
				operator_logical_or_,		// '||'
				operator_logical_not_,		// '!'
				operator_logical_xor_,		// '^'
			}; // enum eToken
		}; // namespace eToken

		struct Token {
			const eToken::enum_type type;
			rvector<rvector<p1::Token>::const_iterator> tokens;

			Token(eToken::enum_type type, rvector<rvector<p1::Token>::const_iterator> tokens)
				: type(type) , tokens(tokens) {
			}

			rstring lit() const {
				std::string s;
				for (auto it = tokens.cbegin(); it != tokens.cend(); it++) {
					s += (*it)->lit();
				}
				return rstring(s);
			}
		};

		struct TokenTraits {
			using token_enum_type = eToken::enum_type;

			static token_enum_type opening(const rvector<p1::Token>& input
				, rvector<p1::Token>::const_iterator it) {
				if (it == input.cend())
					return eToken::eof_;
				switch (it->type)
				{
				case p1::eToken::number_: 												// Number value. eg. "1234"
					// If the next token is a period, then we have a real literal.
					if ((it + 1) != input.cend() && (it + 1)->type == p1::eToken::period_)
						return eToken::real_;
					else
						return eToken::integral_;
					break;																
				case p1::eToken::alnumus_: return eToken::identity_; break;						// Alphanumeric value with underscores. eg. "hello_world"
				case p1::eToken::whitespace_: return eToken::none_;  break;						// Whitespace token ' '
				case p1::eToken::newline_: return eToken::end_of_statement_; break;				// Newline token '\n'
				case p1::eToken::eof_: return eToken::eof_; break;								// End of file token. '\0'
				case p1::eToken::open_paren_: return eToken::open_scope_; break;				// '('
				case p1::eToken::closed_paren_: return eToken::closed_scope_; break;			// ')'
				case p1::eToken::comma_: return eToken::separator_; break;						// ','
				case p1::eToken::period_: return eToken::dot_operator_; break;					// '.'
				case p1::eToken::pound_: return eToken::directive_operator_; break;				// '#'
				case p1::eToken::commercial_at_: return eToken::auto_operator_; break;			// '@'
				case p1::eToken::exclamation_mark_: return eToken::operator_logical_not_; break;// '!'
				case p1::eToken::question_mark_: return eToken::operator_logical_xor_; break;	// '?'
				case p1::eToken::apostrophe_:return eToken::litop_string_; break;				// '\''
				case p1::eToken::semicolon_: return eToken::end_of_statement_; break;			// ';'
				case p1::eToken::percent_sign_: return eToken::operator_modulo_; break;			// '%'
				case p1::eToken::asterisk_: return eToken::operator_multiply_; break;			// '*'
				case p1::eToken::plus_sign_: return eToken::operator_plus_; break;				// '+'
				case p1::eToken::minus_sign_: return eToken::operator_minus_; break;			// '-'
				case p1::eToken::solidus_:
					// if single then its a division operator
					// if double then its a line comment operator
					// if triple then its a block comment operator
					if ((it + 1) != input.cend() && (it + 1)->type == p1::eToken::solidus_) {
						if ((it + 2) != input.cend() && (it + 2)->type == p1::eToken::solidus_)
							return eToken::litop_block_comment_;
						else
							return eToken::litop_line_comment_;
					}
					else
						return eToken::operator_divide_;
					break;
				case p1::eToken::equals_sign_: return eToken::operator_assign_; break;				// '='
				case p1::eToken::less_than_sign_: return eToken::operator_less_; break;				// '<'
				case p1::eToken::greater_than_sign_: return eToken::operator_greater_; break;		// '>'
				case p1::eToken::circumflex_accent_: return eToken::operator_logical_xor_; break;	// '^'
				case p1::eToken::vertical_line_: return eToken::operator_logical_or_; break;		// '|'

					// User Operator tokens
				case p1::eToken::quotation_mark_:		// '"'
				case p1::eToken::low_line_:				// '_'
				case p1::eToken::tilde_:				// '~'
				case p1::eToken::left_curly_bracket_:
					break;								// '{'
				case p1::eToken::right_curly_bracket_:
					break;								// '}'
				case p1::eToken::left_square_bracket_:	// '['
				case p1::eToken::right_square_bracket_: // ']'
				case p1::eToken::left_apostrophe_:		// '`'
				case p1::eToken::reverse_solidus_:		// '\'
				case p1::eToken::ampersand_:			// '&'
				case p1::eToken::colon_:				// ':'
				case p1::eToken::symbol_:
					return eToken::operator_; break;	// Extended symbol value. eg. "÷"
				default:
					return eToken::invalid_;
				} // switch (it->type)
			} // static token_enum_type open_independent(const rvector<p1::Token>& input, rvector<p1::Token>::const_iterator it)

			static token_enum_type seperating(const token_enum_type type, rvector<p1::Token>::const_iterator it)

			// Given a token type, and p1::Token. Determine if the p1::Token will close the token type.
			static bool closing(const token_enum_type type, rvector<p1::Token>::const_iterator it) {
				switch (type)
				{
					case eToken::line_comment_:
						return it->type == p1::eToken::newline_; break;
					case eToken::block_comment_:
						return	it->type == p1::eToken::solidus_
							&&	(it + 1)->type != p1::eToken::eof_ && (it + 1)->type == p1::eToken::solidus_
							&&	(it + 2)->type != p1::eToken::eof_ && (it + 2)->type == p1::eToken::solidus_; 
						break;
					case eToken::string_:
						return it->type == p1::eToken::apostrophe_; break;
				default:
					return false;
				}
			}



			// Base case for token combination: Should never be called - indicates unimplemented token combination.
			// Returns the invalid token.
			template<token_enum_type token_v>
			static Token perform_lex(rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input) {
				return Token{ eToken::invalid_,it,it };
			}

			template<token_enum_type token_v> requires (token_v == eToken::line_comment_)
			static Token perform_lex(rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input) {
				// All following tokens until the end of the line are part of the comment.
				auto start = _operator.begin; // Starting from the first solidus.
				// Iterate from after the first two solidus.
				while (it != input.cend() && it->type != p1::eToken::newline_) {
					it++;
				}
				return Token{ eToken::line_comment_,start,it };
			}

			template<token_enum_type token_v> requires (token_v == eToken::block_comment_)
				static Token perform_lex(rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input) {
				// All following tokens until the next occurence of a triple solidus are part of the comment.
				auto start = _operator.begin;	// Starting from the first solidus.	
				while (it != input.cend()) {	// Iterate from after the first three solidus.
					if (it->type == p1::eToken::solidus_
						&& (it + 1) != input.cend() && (it + 1)->type == p1::eToken::solidus_
						&& (it + 2) != input.cend() && (it + 2)->type == p1::eToken::solidus_) {
						it += 3;
						break;
					}
					it++;
				}
				return Token{ eToken::block_comment_,start,it };
			}
			
			template<token_enum_type token_v> requires (token_v == eToken::string_)
				static Token perform_lex(rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input) {
				// All following tokens until the next occurence of a triple solidus are part of the comment.
				auto start = _operator.begin;	// Starting from the first apostrophe	
				while (it != input.cend()) {	// Iterate from after the first apostrophe.
					if (it->type == p1::eToken::apostrophe_) {
						it++;
						break;
					}
					it++;
				}
				return Token{ eToken::string_,start,it };
			}
			

			template<token_enum_type token_v> requires (token_v == eToken::litop_line_comment_)
				static Token perform_lex(rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input) {
				// All following tokens until the end of the line are part of the comment.
				auto start = it; // Starting from the first solidus.
				
				if (it + 1 != input.cend() && (it + 1)->type == p1::eToken::solidus_) {
					it += 2;
					return Token{ eToken::litop_line_comment_,start,it };
				}
				else {
					return Token{ eToken::invalid_,it,it };
				}
			}

			template<token_enum_type token_v> requires (token_v == eToken::litop_block_comment_)
				static Token perform_lex(rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input) {
				// All following tokens until the next occurence of a triple solidus are part of the comment.
				auto start = it;	// Starting from the first solidus.	
				if (it + 2 != input.cend() && (it + 1)->type == p1::eToken::solidus_
					&& (it + 2)->type == p1::eToken::solidus_) {
					it += 3;
					return Token{ eToken::litop_block_comment_,start,it };
				}
				else {
					return Token{ eToken::invalid_,it,it };
				}
			}


		
			Token lex(const token_enum_type type, rvector<p1::Token>::const_iterator& it, const rvector<p1::Token>& input, rvector<Token>& operator_stack) {
				switch (type)
				{
				case eToken::line_comment_:
					return lex_line_comment(it, input, operator_stack); break;
				case eToken::block_comment_:
					return lex_block_comment(it, input, operator_stack); break;
				case eToken::string_:
					return lex_string(it, input, operator_stack); break;
				case eToken::integral_:
					return lex_integral(it, input, operator_stack); break;
				case eToken::real_:
					return lex_real(it, input, operator_stack); break;
				case eToken::identity_:
					return lex_identity(it, input, operator_stack); break;
				case eToken::separator_:
					return lex_separator(it, input, operator_stack); break;
				case eToken::end_of_statement_:
					return lex_end_of_statement(it, input, operator_stack); break;
				case eToken::open_scope_:
					return lex_open_scope(it, input, operator_stack); break;
				case eToken::closed_scope_:
					return lex_closed_scope(it, input, operator_stack); break;
				case eToken::open_type_scope_:
					return lex_open_type_scope(it, input, operator_stack); break;
				case eToken::closed_type_scope_:
					return lex_closed_type_scope(it, input, operator_stack); break;
				case eToken::open_list_scope_:
					return lex_open_list_scope(it, input, operator_stack); break;
				case eToken::closed_list_scope_:
					return lex_closed_list_scope(it, input, operator_stack); break;
				case eToken::directive_operator_:
					return lex_directive_operator(it, input, operator_stack); break;
				case eToken::dot_operator_:
					return lex_dot_operator(it, input, operator_stack); break;
				case eToken::auto_operator_:
					return lex_auto_operator(it, input, operator_stack); break;
				case eToken::operator_plus_:
					return lex_operator_plus(it, input, operator_stack); break;
				case eToken::operator_minus_:
					return lex_operator_minus(it, input, operator_stack); break;
				case eToken::operator_multiply_:
					return lex_operator_multiply(it, input,)
				}
			}

			static rvector<Token> tokenize(const rvector<p1::Token>& source) {
				rvector<Token> output;
				rvector<Token> operator_stack;


				// While there are tokens left in the source,based current cursor token determine the next type of token we should attempt to lex.
				auto cursor = source.cbegin();
				while (!source.empty()) {
					eToken::enum_type open_token_type = TokenTraits::opening(source, cursor);

					// If we have a valid token type, then we should lex until we reach the end of the token.
					if (open_token_type != eToken::invalid_) {
						Token lex_result = TokenTraits::lex(open_token_type, cursor, source);
						if (lex_result.type != eToken::invalid_) {
							if (lex_result.type == eToken::eof_) {
								output.push_back(Token{ eToken::eof_,source.begin(),cursor });
								return output; // Eof token lexing is complete.
							}
							else if(lex_result.type == eToken::none_) { // no token produced, maybe an operator pushed to the stack.
								cursor = lex_result.end; // Ignore, move cursor to the end of the token.
							}
							else {
								output.push_back(lex_result);
								cursor = lex_result.end;
							}
						}
						else {
							output.push_back(lex_result);
							return output; // Stop lexing, error has occured.
						}
					}
					else {
						output.push_back(Token{ eToken::invalid_,source.begin(),cursor });
						return output; // Stop lexing, error has occured.
					}
				}

				// End of file, push back eof token.
				output.push_back(Token{ eToken::eof_,source.begin(),cursor });
				return output;
			}
		};
	};
};