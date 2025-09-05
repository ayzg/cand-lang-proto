#pragma once
#include <cassert>				// assert() for programmer logic error checks
#include <unordered_map>		// std::unordered_map for associating parsing methods with statement types
#include <functional>			// std::function for storing parsing methods
#include <list>					// std::list for statement body
#include <any>					// std::any for returning literal values from evaluation methods
#include <optional>				// std::nullopt_t for std::any when returning an error argument from literal value evaluation.
#include <iostream>				// std::cerr for error handling in literal value evaluation.
#include <typeinfo>				// typeid() for handing std::any_cast results in literal value evaluation, and storing the type literal.
#include <typeindex>			// std::type_index for storing the type literal.

#include "syntax_traits.hpp"
#include "tokenizer.hpp"
#include "statement.hpp"
#include "parser_literaltable.hpp"


namespace cil {
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
// <@class:ParsingResult>
// <@brief> A struct containing the result of a parsing method.
// <@member:cursor> The end of the parsed statement.
// <@member:valid>	If the statement is valid, error handling will be done by caller.
// <@member:error_message>	The error message if the statement is invalid. 
//							NOTE: Error messages are passed up a call stack through a ParsingProcess.
	struct ParsingResult {
		Tokenizer::token_view_iterator cursor{};
		Statement statement{};
		bool valid{ true };
		std::string error_message_{ "" };

		ParsingResult() = default;
		ParsingResult(Tokenizer::token_view_iterator cursor = {}, bool valid = true, std::string error_message = "")
			: cursor(cursor), valid(valid), error_message_(error_message) {}
	};
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
	class TokenCursor;
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
// <@class:ScopeResult>
// <@brief> A struct containing the result of a scope search. Use member methods to access the scope.
// <@member:valid> If the scope is valid, invalid scopes indicate a mismatched parenthesis.
	struct ScopeResult {
		bool valid{ false };
		Tokenizer::token_view_iterator begin_;
		Tokenizer::token_view_iterator end_;

		// <@method:contained_end> Returns the end of the scope, not including the close token.
		auto contained_end() const { return end_ - 1; }
		// <@method:contained_begin> Returns the beginning of the scope, not including the open token.
		auto contained_begin() const { return begin_ + 1; }
		// <@method:is_empty> Returns true if the scope is empty.
		auto is_empty() const { return contained_begin() == contained_end(); }
		// <@method:scope_end> Returns the end of the scope, including the close token.
		auto scope_end() const { return end_; }
		// <@method:scope_begin> Returns the beginning of the scope, including the open token.
		auto scope_begin() const { return begin_; }


		ScopeResult() = default;
		ScopeResult(bool valid, Tokenizer::token_view_iterator begin, Tokenizer::token_view_iterator end)
			: valid(valid), begin_(begin), end_(end) {}
	};

	// <@method:get_scope> Returns the last matching scope of the open and close tokens.
	auto get_token_scope(Tokenizer::token_view_iterator cursor, Tokenizer::token_view_iterator end, TokenKind open, TokenKind close){
		auto scope_depth = 0;
		auto last_open = cursor;
		auto last_closed = cursor;

		if (cursor->get().type() != open) {
			return ScopeResult{ false, cursor, cursor }; // Programmer error, first token is not an open token
		}

		// find the last matching close token, if there is no matching close token, return false
		for (auto i = cursor; i < end; i++) {
			if (i->get().type() == open) {
				scope_depth++;
				last_open = i;
			}
			else if (i->get().type() == close) {
				scope_depth--;
				last_closed = i;
			}

			if (scope_depth == 0) {
				break;
			}
		}

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (scope_depth != 0) {
			return ScopeResult{ false, cursor, last_closed + 1 }; // No matching close token
		}
		else {
			return ScopeResult{ true, cursor, last_closed + 1 };
		}
	}
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
	struct TokenCursor {
		Tokenizer::token_view_iterator it_;
		Tokenizer::token_view_iterator beg_;
		Tokenizer::token_view_iterator end_;
		Tokenizer::token_view_iterator source_beg_;
		Tokenizer::token_view_iterator source_end_;

		auto end() const { return end_; }
		auto begin() const { return beg_; }
		auto source_end() const { return source_end_; }
		auto source_begin() const { return source_beg_; }
		bool empty() const { return beg_ == end_; }

		// <@method:next> returns the current token on the cursor, if at end or before begin, returns eof token.
		const Token& get() const {
			if (it_ >= end_) {
				return Token{ TokenKind::eof };
			}
			else if (it_ < beg_) {
				return Token{ TokenKind::eof };
			}
			else
				return it_->get();
		}

		// <@method:lit> returns the literal of the token at the cursor.
		rstring_view lit() const { return get().lit(); }

		// <@method:type> returns the kind of the token at the cursor.
		TokenKind type() const { return get().kind; }

		// <@method:line> returns the line of the token at the cursor.
		auto line() const { return get().line(); }

		// <@method:column> returns the column of the token at the cursor.
		auto column() const { return get().column(); }

		//<@method:precidence> returns the precidence of the token at the cursor.
		auto precidence() const { return get().precidence(); }

		// <@method:associativity> returns the associativity of the token at the cursor.
		auto associativity() const { return get().associativity(); }

		// <@method:operation_type> returns the operation type of the token at the cursor.
		auto operation_type() const { return get().operation_type(); }

		Tokenizer::token_view_iterator get_it() const { return it_; }

		// <@method:advance> advances the cursor by n.
		TokenCursor& advance(int n = 1) {
			if (n != 0) {
				if(it_ + n >= end_) {
					it_ = end_;
				}
				else if (it_ + n < beg_) {
					it_ = beg_;
				}
				else {
					it_ += n;
				}
			}
			return *this;
		}

		// <@method:advance_to> advances the cursor to the new_cursor. Checks that cursor is within beg and end.
		TokenCursor& advance_to(Tokenizer::token_view_iterator new_cursor) {
			if (new_cursor < beg_) {
				throw std::out_of_range("TokenCursor passed advance_to outside of begin.");
			}
			else if (new_cursor >= end_) {
				throw std::out_of_range("TokenCursor passed advance_to outside of end.");
			}
			else
				it_ = new_cursor;
			return *this;
		}

		// <@method:next> returns cursor advanced by N. N may be negative.
		TokenCursor next(int n = 1)const {
			TokenCursor next_cursor = *this;
			next_cursor.advance(n);
			return next_cursor;
		}

		// <@method:peek> returns the token at the cursor + n.
		const Token& peek(int n = 0)const {
			return next(n).get();
		}

		// <@method:is_equal> Returns true if the token at the cursor + offset is equal to the kind and literal.
		auto is_equal(TokenKind kind, rstring_view literal, int offset)const {
			return peek(offset).type() == kind && peek(offset).lit() == literal;
		}

		// <@method:select> Given an iterator pair, sets the cursor to the begin and end of the pair. Checks that pair is within source. It_ is set to the closest valid position.
		// If assinged pair is not withinin source, throws runtime exception.
		TokenCursor& select(Tokenizer::token_view_iterator begin, Tokenizer::token_view_iterator end) {
			if (begin < source_beg_ || end > source_end_) {
				throw std::out_of_range("TokenCursor passed selection outside of source begin or end.");
			}
			else {
				beg_ = begin;
				end_ = end;
				
				// If cursors is currently after end or before begin then it is moved to the end or begin
				if (it_ < beg_) {
					it_ = beg_;
				}
				else if (it_ >= end_) {
					it_ = end_;
				}
			}
			return *this;
			}

		// Creates a new TokenCursor with the same source and cursor. But a different selection.
		TokenCursor make_selection(Tokenizer::token_view_iterator begin, Tokenizer::token_view_iterator end) {
				TokenCursor selection = *this;
				selection.select(begin, end);
				return selection;
			}
	};

	// The purpose of this class is to generate literal value tokens from expression statements.
	// When a value is returned which is not a typed literal in the source code, an entry is added to the literal table.
	class ParserLiteralTable {
		rvector<char> buffer_;
		rvector<Token> tokens_;
		TokenSanitizer::token_view token_view_;
	public:
		auto push(TokenKind token_kind, rstring literal) {
			// If the exact token kind and literal is already defined, return the existing token.
			for (auto token : token_view_) {
				if (token.get().kind == token_kind && token.get().lit() == literal) {
					return std::make_pair(token_view_.cend() - 1, token_view_.cend());
				}
			}

			// Else continue...
			// Push each character of the literal into the buffer.
			auto start = buffer_.size();
			auto lit_size = literal.size();
			for (auto c : literal) {
				buffer_.push_back(c);
			}

			// Push the token. Using the start and size of the literal in the buffer to determine the token's literal begin and end
			tokens_.push_back(Token(token_kind, buffer_.cbegin() + start, buffer_.cbegin() + start + lit_size));

			// Add the token to the token view.
			token_view_.push_back(tokens_.back());

			// return an iterator pair to the token view begin and end.(the last token pushed)
			return std::make_pair(token_view_.cend() - 1, token_view_.cend());
		}
	};
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
// <@class:ParsingProcess>
// <@brief> A virtual base for a functor which performs a parsing process on a statement.
// <@member:error_message_> A stringstream for appending error messages.
// <@method:error> Returns the error_message_ stringstream.
// <@method:make_result> Returns a ParsingResult with the error_message_ stringstream appended.
// <@method:perform> The virtual functor which performs the parsing process.
	class ProgramParser; // fwd decl
	struct ParsingProcess {
		std::stringstream error_message_{ std::string("[") + typeid(*this).name() + "]" };

		auto& error() { return error_message_; }
		ParsingResult make_error(Tokenizer::token_view_iterator cursor,Token offending_token,std::string error_message ) {
			error() << error_message << " Offending token: " << offending_token.lit()
				<< "Line: " << offending_token.line() << " Col: " << offending_token.column() << "\n";
			return ParsingResult{ cursor, false, error_message_.str() };
		}
		ParsingResult make_success(Tokenizer::token_view_iterator cursor) {
			return make_result(cursor, true);
		}
		auto serror() const { return error_message_.str(); }

		ParsingResult make_result(Tokenizer::token_view_iterator cursor, bool valid=true, std::string error_message="") {
			error_message_ << error_message;
			return ParsingResult{ cursor, valid, error_message_.str()};
		}

		auto operator()(TokenCursor cursor) {
			return perform(cursor);
		}

		ParsingProcess() = default;
		ParsingProcess(const ParsingProcess& other) { 
			this->error() << other.serror();
		};
		virtual ParsingResult perform(TokenCursor cursor) = 0;
		virtual ~ParsingProcess() = default;
	};
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
// <@class:ProgramParser>
// <@brief> Constructs an AST using ParsingProcesses stored in a map retrieved based on a given input TokenSanitizer::token_view.
//		    Has multiple const utility methods for observing the token view. 
//          Passes a const reference of itself to the ParsingProcesses. So the functors may utilize the utility methods. 
	class ProgramParser {
	public:
		using token_view = Tokenizer::token_view;
		using token_iterator = Tokenizer::token_view_iterator;
		using statement_list  = std::list<Statement>;
		using value_eval_result = std::any;
		using type_eval_result = std::type_index;
		using identity_eval_result = rstring;
		using parsing_method_t = std::function<ParsingResult(TokenCursor)>;
		using parsing_method_map_t = std::unordered_map<rstring_view, parsing_method_t>;
	private:
		const token_view& tokens_;
		Statement root_;
		ParserLiteralTable literal_table_;
		//std::unordered_map<rstring_view, CandilType> global_type_table_;
		//std::unordered_map<rstring_view, CandilVariable> global_variable_table_;
		static const parsing_method_map_t directive_map_;
	public:
		ProgramParser(const token_view& tokens)
			: tokens_(tokens) ,root_(StatementKind::program_,tokens.cbegin(),tokens.cend()){};

        auto add_literal(TokenKind token_kind, rstring literal) {
			return literal_table_.push(token_kind, literal);
		}
	/* Utility Methods, shared among the parsing and evaluation methods.*/
		auto tokens_end() const { return tokens_.cend(); }
		auto tokens_begin() const { return tokens_.cbegin(); }
		auto get(token_iterator cursor) const;
		auto peek(token_iterator cursor, int n = 0)const;
		auto find_forward_exact(token_iterator cursor, rvector<Token> match)const;
		auto find_forward(token_iterator cursor, rvector<Token> match)const;
		auto get_scope_exact(token_iterator cursor, Token open, Token close)const;
		auto get_scope(token_iterator cursor, TokenKind open, TokenKind close)const;
		auto get_seperated_scope(token_iterator cursor, TokenKind open, TokenKind close, TokenKind separate)const;
		auto get_base_scope(token_iterator cursor)const;
		auto get_base_seperated_scope(ProgramParser::token_iterator cursor)const;
		auto is_equal(token_iterator cursor, TokenKind kind, rstring_view literal, int offset = 0)const;
		inline void advance(token_iterator& cursor, int n = 1) const;
		inline void advance_to(token_iterator& cursor, token_iterator new_cursor) const;
		inline ProgramParser::token_iterator next(token_iterator& cursor) const;
		inline ProgramParser::token_iterator jump(token_iterator& cursor, int offset = 1) const {
			return std::next(cursor, offset);
		};
	/* 
	* Main methods
	* Parser is able to parse a compiled program, or an interpreted program.
	* The parser will parse the compiled program first, then the interpreted program.
	* The parser will return a statement which a abstract syntax tree of the program.
	* Each statement will contain a type and a body of statements, which may be empty.
	* parse_tokens parses the tokens into a program statement. 
	* The compiled code is fully translated and evaluated. It is independent of the interpreted code.
	* parse_compiled_program parses the compiled program statement which may be added to the program statement.
	* parse_interpreted_program parses the interpreted program statement which may be added to the program statement.
	*/
		void parse_compiled_program(TokenCursor token_it);
		Statement parse_tokens();
		}; // end ProgramParser
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
	inline ProgramParser::token_iterator ProgramParser::next(ProgramParser::token_iterator& cursor)const{
		return std::next(cursor);
	}
//-----------------------------------------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------------------------------------//
// <implementation:ProgramParser>	
	//-------------------------------------------------------------------------------------------------------------------------------------------//	
	// <@method:get> Returns the token at the cursor.
	auto ProgramParser::get(ProgramParser::token_iterator cursor) const {
		if (cursor >= tokens_end()) {
			return Token{ TokenKind::eof };
		}
		return cursor->get();
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//	
	
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:peek> Returns the token at the cursor + n.
	auto ProgramParser::peek(ProgramParser::token_iterator cursor, int n)const {
		if (n == 0) {
			return get(cursor);
		}

		auto peeked = std::next(cursor, n);
		if (peeked >= tokens_end() || peeked < tokens_begin()) {
			return Token{ TokenKind::eof };
		}
		return get(peeked);
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:find_forward_exact> Returns true if the next n tokens match the match vector.
	auto ProgramParser::find_forward_exact(ProgramParser::token_iterator cursor, rvector<Token> match)const {
		auto end = std::next(cursor, match.size());
		if (std::equal(match.begin(), match.end(), cursor, end)) {
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:find_forward> Returns the cursor if the next n tokens match the match vector.
	auto ProgramParser::find_forward(ProgramParser::token_iterator cursor, rvector<Token> match)const {
		auto end = std::next(cursor, match.size());
		// Find matching subvector based on the predicate of the type() of the tokens
		auto found = std::search(cursor, end, match.begin(), match.end(), [](const Token& a, const Token& b) {
			return a.type() == b.type();
			});
		return cursor;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:get_scope_exact> Returns the last matching scope of the open and close tokens.
	auto ProgramParser::get_scope_exact(ProgramParser::token_iterator cursor, Token open, Token close)const {
		assert(get(cursor) == open && "Token in cursor passed to get_scope() must be equal to the open token.");
		auto scope_depth = 0;
		auto last_open = cursor;
		auto last_closed = cursor;

		if (get(cursor) != open) {
			return ScopeResult{ false, cursor, cursor }; // Programmer error, first token is not an open token
		}

		// find the last matching close token, if there is no matching close token, return false
		for (auto i = cursor; i < tokens_end(); i++) {
			if (get(i) == open) {
				scope_depth++;
				last_open = i;
			}
			else if (get(i) == close) {
				scope_depth--;
				last_closed = i;
			}

			if (scope_depth == 0) {
				break;
			}
		}

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (scope_depth != 0) {
			return ScopeResult{ false, last_open, last_closed + 1 }; // No matching close token
		}
		else {
			return ScopeResult{ true, last_open, last_closed + 1 };
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:get_scope> Returns the last matching scope of the open and close tokens.
	auto ProgramParser::get_scope(ProgramParser::token_iterator cursor, TokenKind open, TokenKind close)const {
		auto scope_depth = 0;
		auto last_open = cursor;
		auto last_closed = cursor;

		if (get(cursor).type() != open) {
			return ScopeResult{ false, cursor, cursor }; // Programmer error, first token is not an open token
		}

		// find the last matching close token, if there is no matching close token, return false
		for (auto i = cursor; i < tokens_end(); i++) {
			if (get(i).type() == open) {
				scope_depth++;
				last_open = i;
			}
			else if (get(i).type() == close) {
				scope_depth--;
				last_closed = i;
			}

			if (scope_depth == 0) {
				break;
			}
		}

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (scope_depth != 0) {
			return ScopeResult{ false, cursor, last_closed + 1 }; // No matching close token
		}
		else {
			return ScopeResult{ true, cursor, last_closed + 1 };
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:get_seperated_scope> Returns the last matching scope of the open and close tokens.
	auto ProgramParser::get_seperated_scope(ProgramParser::token_iterator cursor, TokenKind open, TokenKind close, TokenKind separate)const {
		auto scope_depth = 0;
		auto last_open = cursor;
		auto last_closed = cursor;
		auto last_separate = cursor;
		auto next_separate = cursor;

		rvector<ScopeResult> separated_scopes;

		if (get(cursor).type() != open) {
			separated_scopes.push_back(ScopeResult{ false, cursor, cursor }); // Programmer error, first token is not an open token
			return separated_scopes;
		}

		// Find the next seperate token at scope depth 0.
		// Push the scope from the last seperate to the next seperate.
		// If a close token at depth 0 is found before the next seperate, 
		// Push the scope from the last seperate to the close token. Break.
		for(auto i = cursor; i < tokens_end(); i++) {
			if (get(i).type() == open) {
				scope_depth++;
				last_open = i;
			}
			else if (get(i).type() == close) {
				scope_depth--;
				last_closed = i;
			}
			else if (get(i).type() == separate && scope_depth == 1) {
				next_separate = i;
				separated_scopes.push_back(ScopeResult{ true, last_separate, next_separate + 1 });
				last_separate = next_separate;
			}

			if (scope_depth == 0) {
				break;
			}
		}

		// NOTE: We are adding 1 to last closed because the end is 1 past the last token.
		if (scope_depth != 0) {
			std::cerr << "[ERROR] No matching close token for open token: " << get(cursor).lit() << std::endl;
			separated_scopes.push_back(ScopeResult{ false, cursor, last_closed + 1 }); // No matching close token
			return separated_scopes;
		}
		else {
			separated_scopes.push_back(ScopeResult{ true, last_separate, last_closed + 1 });
			return separated_scopes;
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:get_base_scope> Returns the last matching scope of the open and close tokens.
	auto ProgramParser::get_base_scope(ProgramParser::token_iterator cursor)const {
		return get_scope(cursor, TokenKind::open_scope, TokenKind::close_scope);
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:get_base_seperated_scope> Returns the last matching scope of the open and close tokens.
	auto ProgramParser::get_base_seperated_scope(ProgramParser::token_iterator cursor)const {
		return get_seperated_scope(cursor, TokenKind::open_scope, TokenKind::close_scope,TokenKind::comma);
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:is_equal> Returns true if the token at the cursor + offset is equal to the kind and literal.
	auto ProgramParser::is_equal(ProgramParser::token_iterator cursor, TokenKind kind, rstring_view literal, int offset)const {
		return peek(cursor, offset).type() == kind && peek(cursor, offset).lit() == literal;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	//
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:advance> Advances the cursor by n.
	inline void ProgramParser::advance(ProgramParser::token_iterator& cursor, int n)  const {
		if (n != 0)
			std::advance(cursor, n);
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// <@method:advance_to> Advances the cursor to the new_cursor.
	inline void ProgramParser::advance_to(ProgramParser::token_iterator& cursor, ProgramParser::token_iterator new_cursor)  const {
		cursor = new_cursor;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------------//
	// 
// <end_implementation:ProgramParser>	
//-----------------------------------------------------------------------------------------------------------------------------------------------//	
//-----------------------------------------------------------------------------------------------------------------------------------------------//	

/*********************************************************************************************************************************
* <@method:parse_compiled_program>
* Loop will not execute if the compiled program is empty. end = beg
* All statements will start with a directive, or alnumus. Otherwise invalid.
*
* Directive Parsing
* #enter and #start and special directives which are not included in the directive_map_.
* Some directives will have arguments, some will not.
* NOTE: for now, all directives have arguments or empty scope.
* NOTE: it is assumed no invalid directives will be passed to the parser.(tokenization programmer error).
**********************************************************************************************************************************/

/*********************************************************************************************************************************
* <@method:parse_tokens>
* -root statement is always of type program_.
* -The beginning of the root statement is the beginning of the source.
* -The end of the root statement is the end of the source.
* -The root statement is constructed upon construction of the parser.
*
* The minimal program is:
* 1. First token of program MUST be a #enter directive, followed by a scope.
* 2. The scope may be empty or contain arguments to the compiled program.
* 3. The scope must be followed by another scope. Within the scope are all statements in the compiled program
* 4. Compiled program statement scope must be followed by a #start directive, followed by a scope.
* 5. The scope may be empty or contain arguments to the interpreted program.
* 6. The scope must be followed by another scope. Within the scope are all statements in the interpreted program
**********************************************************************************************************************************/
	Statement ProgramParser::parse_tokens() {
		token_iterator cursor = tokens_begin();

		// 1
		if (!is_equal(cursor, TokenKind::directive, SyntaxTraits::DirectiveLiteral::enter)) {
			std::cerr << "[ERROR] First token of program must be a #enter directive." << std::endl;
			return Statement(StatementKind::invalid_);
		}
		advance(cursor);

		// 2
		auto compiled_arguments_scope = get_base_scope(cursor);
		if (!compiled_arguments_scope.valid) {
			std::cerr << "[ERROR] #enter directive must be followed by argument scope, mismatched parentheses ?" << std::endl;
			return Statement(StatementKind::invalid_);
		}
		advance_to(cursor, compiled_arguments_scope.scope_end());

		// 3
		auto compiled_program_scope = get_base_scope(cursor);
		if (!compiled_program_scope.valid) {
			std::cerr << "[ERROR] #enter directive argument scope must be followed by statement scope, mismatched parentheses ?" << std::endl;
			return Statement(StatementKind::invalid_);
		}
		auto& compiled_program = root_.add_statement(StatementKind::compiled_program_,
			compiled_program_scope.contained_begin(), compiled_program_scope.contained_end());
		advance_to(cursor, compiled_program_scope.scope_end());

		// 4
		if (!is_equal(cursor, TokenKind::directive, SyntaxTraits::DirectiveLiteral::start)) {
			std::cerr << "[ERROR] #enter directive statement scope must be followed by #start directive." << std::endl;
			return Statement(StatementKind::invalid_);
		}

		advance(cursor);

		// 5

		auto interpreted_arguments_scope = get_base_scope(cursor);
		if (!interpreted_arguments_scope.valid) {
			std::cerr << "[ERROR] #start directive must be followed by argument scope, mismatched parentheses ?" << std::endl;
			return Statement(StatementKind::invalid_);
		}
		advance_to(cursor, interpreted_arguments_scope.scope_end());

		// 6

		auto interpreted_program_scope = get_base_scope(cursor);

		if (!interpreted_program_scope.valid) {
			std::cerr << "[ERROR] #start directive argument scope must be followed by statement scope, mismatched parentheses ?" << std::endl;
			return Statement(StatementKind::invalid_);
		}
		auto& interpreted_program = root_.add_statement(StatementKind::interpreted_program_,
			interpreted_program_scope.contained_begin(), interpreted_program_scope.contained_end());
		advance_to(cursor, interpreted_program_scope.scope_end());

		// 7 Now we can begin to parse the compiled program's statements.
		parse_compiled_program(compiled_program);

		return root_;
	}; // end parse_tokens()

#define ParseMethodDefinition(p) 	ParsingResult p::perform(TokenCursor cursor)
	/// Parsing Functors must be forward declared as they will call eachother recursiveley.
	struct ParseDirectiveType : public ParsingProcess {
		ParsingResult perform(TokenCursor cursor);
	};
	struct ParseTypeExpression : public ParsingProcess {
		ParsingResult perform(TokenCursor cursor);
	};
	struct ParseIdentityExpression : public ParsingProcess {
		ParsingResult perform(TokenCursor cursor);
	};
	struct ParseIdentityList : public ParsingProcess {
		ParsingResult perform(TokenCursor cursor);
	};

	struct ParseCompiledProgramBlock : public ParsingProcess {
		ParsingResult perform(TokenCursor cursor);
	};
	
	ParseMethodDefinition(ParseCompiledProgramBlock) {
		Statement compiled_program(StatementKind::compiled_program_, cursor.begin(), cursor.end());
		if (cursor.empty())
			std::cerr << "[WARNING] Empty compiled program body." << std::endl;
		while (cursor.get_it() < cursor.end()) {	// Loop will not execute if the compiled program is empty. end = beg
			if (cursor.type() == TokenKind::directive) {
				/*
				* 2. Perform the directive's parsing function by looking it up in the directive_map_.
				* 3. ->second is the parsing function from the key value pair in the directive_map_.
				* 4. The parsing function will return end of the parsed statement.(1 past the last token)
				* 4.5. Add the statement to the compiled program.
				* 5. Advance the cursor to the end of the parsed statement.
				*/
				auto parse_method = directive_map_.find(cursor.lit());
				if (parse_method == directive_map_.end()) {
					std::cerr << "[ERROR] Unknown directive statement in compiled program: " << cursor.lit() << std::endl;
					cursor.advance_to(cursor.end());
				}
				else {
					auto parse_result = parse_method->second(cursor);
					if (parse_result.valid) {
						compiled_program.add_statement(parse_result.statement);
						cursor.advance_to(parse_result.cursor);
					}
					else {
						std::cerr << "[ERROR] Invalid directive statement in compiled program: " << parse_result.error_message_ << std::endl;
						cursor.advance_to(cursor.end());
					}
				}
			}
			// 2. Alnumus // TODO: implement later
			//else if (cursor.type() == TokenKind::alnumus) {
			//	

			//}
			else {
				// If the statement starts with anything else, it is invalid.
				// Temporary: advance and ignore the token.
				std::cerr << "[WARNING] Invalid start of statement in compiled program: " << cursor.lit() << "Line" << cursor.line() << "Col" << cursor.column() << std::endl;
				cursor.advance();
			}
		}
	}



	ParseMethodDefinition(ParseIdentityExpression) {
		// Start & End of Recursive Descent Parsing
		if (parent.body.empty()) {
			assert(parent.kind() == StatementKind::identifier_expression_
				&& "Statement passed to ParseIdentityExpression must be of type identity_expression_.");
			if (parser.get(cursor).kind == TokenKind::alnumus || parser.get(cursor).kind == TokenKind::number_literal) {
				auto lhs_token_it = cursor;
				if (next(lhs_token_it) == end) { // No following token entire expression is equal to the literal token.
					auto token = parser.get(lhs_token_it);
					if (!(token.type() == TokenKind::alnumus)) {
						return make_error(cursor, token,
							"[ERROR] Invalid type identity expression. Cannot evaluate to single or multiple alnumus literals.");
					}
					parent.add_statement(StatementKind::literal_alnumus_, lhs_token_it, end);
					return make_success(end); // Single Literal Alnumus Expression.
				}
				else { // This is a binary identity operation, the lhs is the literal alnumus.
					auto op_token_it = next(lhs_token_it);
					auto operator_token = parser.get(op_token_it);
					auto token = parser.get(lhs_token_it);

					if (operator_token.type() == TokenKind::subtraction) {
						if (!(token.type() == TokenKind::number_literal)) {
							return make_error(cursor, token,
								"[ERROR] Invalid type identity expression. Both operands of range operator must be number literals.");
						}
					}
					else {
						if (!(token.type() == TokenKind::alnumus)) {
							return make_error(cursor, token,
								"[ERROR] Invalid type identity expression. Left hand side of append or enumerate operator must evaluate to an alnumus or list of alnumus.");
						}
					}

					Statement* operation_statement_ptr = nullptr;
					// Based on the initial operator, add a operation statement to the parent.
					if (!(operator_token.type() == TokenKind::addition
						|| operator_token.type() == TokenKind::subtraction
						|| operator_token.type() == TokenKind::multiplication)) {
						return make_error(cursor, operator_token,
							"[ERROR] Invalid type identity expression. Must begin with alnumus or number literal:");
					}
					else {
						if (operator_token.type() == TokenKind::addition)
							operation_statement_ptr = &parent.add_statement(StatementKind::append_operation_, lhs_token_it, end);
						else if (operator_token.type() == TokenKind::subtraction)
							operation_statement_ptr = &parent.add_statement(StatementKind::range_operation_, lhs_token_it, end);
						else // Has to be multiplication
							operation_statement_ptr = &parent.add_statement(StatementKind::enumerate_operation_, lhs_token_it, end);
					}
					// Add the literal alnumus to the operation statement.
					operation_statement_ptr->add_statement(StatementKind::literal_alnumus_, lhs_token_it, next(lhs_token_it));

					// Call this method again with binop as the parent statement, operator token as cursor.
					auto parse_result = ParseIdentityExpression()(parser, *operation_statement_ptr, op_token_it, end);

					if (!parse_result.valid) {
						return make_error(cursor, parser.get(parse_result.cursor), parse_result.error_message_);
					}

					return make_success(parse_result.cursor); // Full Binary Identity Expression.
				}
			}
			else if (parser.get(cursor).kind == TokenKind::open_scope) {
				// Try to parse an identity scope
				auto parse_result = ParseIdentityList()(parser, parent, cursor, end);

				if (!parse_result.valid) {
					return make_error(cursor, parser.get(parse_result.cursor), parse_result.error_message_);
				}

				// if the following token is the end of the parent statement, then this is a valid identity expression.
				if (parse_result.cursor == end) {
					return make_success(parse_result.cursor);
				}
				else {
					// Remove parsed statement from parent.
					parent.body.pop_back();
				}

				// else, this is a binary identity expression.

				auto op_token_it = parse_result.cursor;
				auto operator_token = parser.get(op_token_it);

				if (operator_token.type() == TokenKind::subtraction) {
					return make_error(cursor, operator_token,
						"[ERROR] Invalid type identity expression. Both operands of range operator must be number literals.");
				}

				Statement* operation_statement_ptr = nullptr;
				// Based on the initial operator, add a operation statement to the parent.
				if (!(operator_token.type() == TokenKind::addition
					|| operator_token.type() == TokenKind::subtraction
					|| operator_token.type() == TokenKind::multiplication)) {
					return make_error(cursor, operator_token,
						"[ERROR] Invalid operator in identity expr.");
				}
				else {
					if (operator_token.type() == TokenKind::addition)
						operation_statement_ptr = &parent.add_statement(StatementKind::append_operation_, cursor, end);
					else if (operator_token.type() == TokenKind::subtraction)
						operation_statement_ptr = &parent.add_statement(StatementKind::range_operation_, cursor, end);
					else // Has to be multiplication
						operation_statement_ptr = &parent.add_statement(StatementKind::enumerate_operation_, cursor, end);
				}

				// Try to parse an identity scope
				auto parse_result1 = ParseIdentityList()(parser, *operation_statement_ptr, cursor, end);

				if (!parse_result1.valid) {
					return make_error(cursor, parser.get(parse_result1.cursor), parse_result1.error_message_);
				}

				// Call this method again with binop as the parent statement, operator token as cursor.
				auto parse_result2 = ParseIdentityExpression()(parser, *operation_statement_ptr, op_token_it, end);

				if (!parse_result2.valid) {
					return make_error(cursor, parser.get(parse_result2.cursor), parse_result2.error_message_);
				}

				return make_success(parse_result2.cursor); // Full Binary Identity Expression.
			}
			else {
				return make_error(cursor, parser.get(cursor),
					"[ERROR] Invalid type identity expression. Must begin with alnumus or number literal:");
			}
		}
		// Recursive Descent Parsing
		else if (parent.body.size()==1) { 
			assert(parent.kind() == StatementKind::append_operation_ 
				|| parent.kind() == StatementKind::range_operation_ 
				|| parent.kind() == StatementKind::enumerate_operation_
				&& "Statement passed to ParseIdentityExpression must be of type append_operation_, range_operation_, or enumerate_operation_.");
			auto operator_token_it = cursor;
			auto operator_token = parser.get(operator_token_it);
			auto rhs_token_it = next(operator_token_it);
			auto rhs_token = parser.get(rhs_token_it);

			// Operator must be followed by a literal alnumus or number literal.
			if(not(rhs_token.type() == TokenKind::alnumus || rhs_token.type() == TokenKind::number_literal)) {
				return make_error(cursor, parser.get(cursor), 
					"[ERROR] Invalid type identity expression. Must be followed by alnumus or number literal:");
			}

			// Confirm lhs is valid for this operation type.
			const auto& lhs = parent.body.back();
			if (operator_token.type() == TokenKind::subtraction) {
				if (!(lhs.type() == StatementKind::literal_number_)) {
					return make_error(cursor, parser.get(operator_token_it-1),
						"[ERROR] Invalid type identity expression. Both operands of range operator must be number literals.");
				}
			}
			else {
				if (!(lhs.type() == StatementKind::literal_alnumus_ 
					|| lhs.type() == StatementKind::append_operation_ 
					|| lhs.type() == StatementKind::enumerate_operation_
					|| lhs.type() == StatementKind::identifier_list_)) {
					return make_error(cursor, parser.get(operator_token_it - 1),
						"[ERROR] Invalid type identity expression. Left hand side of append or enumerate operator must evaluate to an alnumus or list of alnumus.");
				}
			}

			auto next_operator_it = next(rhs_token_it);
			auto next_operator = parser.get(next_operator_it);
			// if there is no following operator token, then this is the end of the identity expression.
			if (next_operator_it == end) {
				if (rhs_token.type() == TokenKind::number_literal)
					parent.add_statement(StatementKind::literal_number_, rhs_token_it, parser.next(rhs_token_it));
				else
					parent.add_statement(StatementKind::literal_alnumus_, rhs_token_it, parser.next(rhs_token_it));

				return make_success(end); // Full Binary Identity Expression.
			}

			// make sure the following operator is one of the allowed idenetity operators.
			if (!next_operator.is_identity_expr_operator()) {
				return make_error(cursor, parser.get(cursor),
					"[ERROR] Invalid type identity expression. Unknown operator in identity expression.");
			}

			//  Operator with a lower importance following this token
			if (next_operator.identity_expr_importance() < operator_token.identity_expr_importance()) {
				//	2.1.1.1.1.The RHS token of this operation is added as a Statement to this 					    
				//				Operation.A copy is made.
				if (rhs_token.type() == TokenKind::number_literal)
					parent.add_statement(StatementKind::literal_number_, rhs_token_it, parser.next(rhs_token_it));
				else
					parent.add_statement(StatementKind::literal_alnumus_, rhs_token_it, parser.next(rhs_token_it));
				// Set the end of this operation to RHS token.
				//parent.end_ = parser.next(rhs_token_it);


				// Find the root of the binary identity expression.
				// The child of the root becomes the new lhs of the binary identity expression.
				auto* found_parent = parent.find_parent(StatementKind::identifier_expression_);
				if(found_parent == nullptr) {
					return make_error(cursor, parser.get(cursor),
						"[ERROR] Invalid type identity expression. Parent of binary identity expression not found.");
				}
				auto& identity_expr_root = *found_parent;

				// Parent should be a binary identity expression with body size of 1.
				// The body back is the LHS of the next binary identity operation.
				auto parent_copy = identity_expr_root.body.back();
				identity_expr_root.body.pop_back();
				
				// Add the next operator as the parent statement.
				Statement* next_operation_ptr = nullptr;
				if (next_operator.type() == TokenKind::addition)
					next_operation_ptr = &identity_expr_root.add_statement(StatementKind::append_operation_, rhs_token_it, end);
				else if (next_operator.type() == TokenKind::subtraction)
					next_operation_ptr = &identity_expr_root.add_statement(StatementKind::range_operation_, rhs_token_it, end);
				else // Must be multiplication
					next_operation_ptr = &identity_expr_root.add_statement(StatementKind::enumerate_operation_, rhs_token_it, end);
				
				// Add the copy of the previous operation as the LHS of the next operation.
				auto& next_op_lhs = next_operation_ptr->add_statement(parent_copy);
				next_op_lhs.end_ = rhs_token_it + 1;
				//	Go to 2. Starting from the next operator.
				return ParseIdentityExpression()(parser, *next_operation_ptr, next_operator_it, end);
			}
			// Else following operator is more important. RHS of this operation is the next operation.
			else { 
				Statement* next_operation_ptr = nullptr;
				//2.1.1.1.1.Add statement of type Next Operation as RHS of this operation.
				if(next_operator.type() == TokenKind::addition)
					next_operation_ptr = &parent.add_statement(StatementKind::append_operation_, rhs_token_it, end);
				else if (next_operator.type() == TokenKind::subtraction)
					next_operation_ptr = & parent.add_statement(StatementKind::range_operation_, rhs_token_it, end);
				else // Must be multiplication
					next_operation_ptr = &parent.add_statement(StatementKind::enumerate_operation_, rhs_token_it, end);

				//2.1.1.1.1. Add RHS token of this operation as a the LHS Statement of Next Operation.
				if (rhs_token.type() == TokenKind::number_literal)
					next_operation_ptr->add_statement(StatementKind::literal_number_, rhs_token_it, parser.next(rhs_token_it));
				else
					next_operation_ptr->add_statement(StatementKind::literal_alnumus_, rhs_token_it, parser.next(rhs_token_it));


				// Find the root of the binary identity expression.
				auto* found_parent = parent.find_parent(StatementKind::identifier_expression_);
				if (found_parent == nullptr) {
					return make_error(cursor, parser.get(cursor),
						"[ERROR] Invalid type identity expression. Parent of binary identity expression not found.");
				}
				auto& identity_expr_root = *found_parent;


				//2.1.1.1.4.Go to 2. with Next Operation as the parent statement.Start from Next Operator.Ending at the End of the Expression.
				auto parse_result = ParseIdentityExpression()(parser, *next_operation_ptr, next_operator_it, end);

				// Result is parsed, unless there was an error.
				if (!parse_result.valid) {
					return make_error(parse_result.cursor, parser.get(parse_result.cursor), parse_result.error_message_);
				}

				// Note parent maybe an invalid reference if the parent was deleted in the recursive call which is why we store the root.
				// update parent begin to the begin of the binary operation.
				identity_expr_root.body.back().beg_ = cursor - 1; // 
				//identity_expr_root.body.back().end_ = parse_result.cursor; // 

				return make_success(parse_result.cursor); // Full Binary Identity Expression.
			}
		}
		else { // Programmer logic error, body size of expression must be 0 or 1.
			return make_error(cursor,parser.get(cursor)
				,"[PROGRAMMER ERROR] ParseIdentityExpression must be called with a parent with a body size of 0 or 1.");
		}
	}

	ParseMethodDefinition(ParseIdentityList) {
		if (!(parser.get(cursor).type() == TokenKind::open_scope)) {
			return make_error(cursor, parser.get(cursor), "[PROGRAMMER ERROR] ParseIdentityList must be called with open_list token {.");
		}

		auto identity_list_scopes = parser.get_base_seperated_scope(cursor);
		auto& list_statement = parent.add_statement(StatementKind::identifier_list_, identity_list_scopes.front().contained_begin(), identity_list_scopes.back().contained_end());
		for (auto& scope : identity_list_scopes) {
			if (!scope.valid) {
				return this->make_result(cursor, false, "[ERROR] ParseIdentityList argument scope is invalid. Mismatched parentheses?");
			}

			// All scopes must contain a single argument of an alnumus.
			if (std::distance(scope.contained_begin(), scope.contained_end()) != 1) {
				return this->make_result(cursor, false, "[ERROR] ParseIdentityList scope must contain a singular alnumus.");
			}

			if(parser.get(scope.contained_begin()).type() != TokenKind::alnumus) {
				return this->make_result(cursor, false, "[ERROR] ParseIdentityList scope must contain a singular alnumus.");
			}

			list_statement.add_statement(StatementKind::literal_alnumus_,scope.contained_begin(), scope.contained_end());
		}

		return make_success(identity_list_scopes.back().scope_end());
	}

	ParseMethodDefinition(ParseTypeExpression) {
		assert(parent.kind() == StatementKind::type_expression_ && "Statement passed to TypeExpressionParser must be of type type_expression_.");
		// The next scope is the type expression, which must be a candil special or type directive token.
		if (parser.get(cursor).kind == TokenKind::candi) {
			auto candi_lit = parser.get(cursor).lit();
			// candi special must be one of the type literals
			if (std::any_of(SyntaxTraits::candi_special_literals.begin(),
				SyntaxTraits::candi_special_literals.end(),
				[s = candi_lit](auto match) { return match == s; })) {
				parent.add_statement(StatementKind::special_int_, cursor, end); // TODO: handle all special types
				return make_result(end, true);
			}
			else {
				return make_result(cursor, false, "[ERROR] Invalid candi special type literal: " + candi_lit);
			}
		}
		else if (parser.get(cursor).kind == TokenKind::directive && parser.get(cursor).lit() == SyntaxTraits::DirectiveLiteral::type) {
			// recursiveley call parse_directive_type until a single argument is returned.
			auto& type_expr = parent.add_statement(StatementKind::type_directive_, cursor, end);
			auto parse_result = ParseDirectiveType()(parser, type_expr, cursor, end);
			return make_result(end, true);
		}
		else {
			this->error() << "[ERROR] Invalid type expression. Must contain directive or candi special literal. Got:" << parser.get(cursor).lit() << std::endl;
			return make_result(cursor, false);
		}
	}

	ParseMethodDefinition(ParseDirectiveType){
		if (!(parser.get(cursor).type() == TokenKind::directive && parser.get(cursor).lit() == SyntaxTraits::DirectiveLiteral::type)) {
			return this->make_result(cursor, false, "[PROGRAMMER ERROR] ParseTypeExpression::perform() must be called with a #type directive.");
		}
		// 0. Advance past the directive
		parser.advance(cursor);
		// 1. Must be followed by a basic scope with 1 or 2 seperated arguments.
		auto directive_type_arg_scopes = parser.get_base_seperated_scope(cursor);
		if (directive_type_arg_scopes.size() != 2 && directive_type_arg_scopes.size() != 1) {
			return this->make_result(cursor, false, "[ERROR] #type directive must be followed by 1 or 2 arguments.");
		}
		// 2. Scopes must be valid. 
		for (auto& scope : directive_type_arg_scopes) {
			if (!scope.valid) {
				return this->make_result(cursor, false, "[ERROR] #type directive argument scope is invalid. Mismatched parentheses?");
			}
		}

		// 4. If scope has 1 argument, it is requesting a type from the type table. (Type Declaration)
		if (directive_type_arg_scopes.size() == 1) {
			// 3. Add the type decl to the parent statement.
			auto& type_directive = parent.add_statement(StatementKind::type_declaration_,
				directive_type_arg_scopes.front().scope_begin(), directive_type_arg_scopes.back().scope_end());

			auto& identity_expression_scope = directive_type_arg_scopes[0];
			if (identity_expression_scope.is_empty()) {
				return this->make_result(cursor, false, "[ERROR] Empty identity expression in #type directive.");
			}
			else {
				// Add the identity expression to the type directive
				auto identity_expression = type_directive.add_statement(StatementKind::identifier_expression_,
					identity_expression_scope.contained_begin(),
					identity_expression_scope.contained_end());
				// Parse it
				auto parse_result = ParseIdentityExpression()(parser, identity_expression,
					identity_expression_scope.contained_begin(),
					identity_expression_scope.contained_end());

				if (!parse_result.valid) {
					return this->make_result(cursor, false, parse_result.error_message_);
				}
				return this->make_result(end);
			}
		}
		// 4. If scope has 2 arguments, it is defining a new type. (Type Definition)
		else { // else must be 2 because we checked in step 1. and returned if it was not 1 or 2.
			// 3. Add the type decl to the parent statement.
			auto& type_directive = parent.add_statement(StatementKind::type_definition_,
				directive_type_arg_scopes.front().scope_begin(), directive_type_arg_scopes.back().scope_end());

			auto& identity_declaration_scope = directive_type_arg_scopes[0];
			auto& type_expression_scope = directive_type_arg_scopes[1];
			// 4.0. The first argument must evaluate to a type_identity_.
			if (identity_declaration_scope.is_empty()) {
				return make_result(cursor, false, "[WARNING] Empty identity declaration in #type directive.");
			}
			else {
				// Add the identity expression to the type directive
				auto& identity_expression = type_directive.add_statement(StatementKind::identifier_expression_,
					identity_declaration_scope.contained_begin(),
					identity_declaration_scope.contained_end());

				auto parse_result = ParseIdentityExpression()(parser, identity_expression,
					identity_declaration_scope.contained_begin(),
					identity_declaration_scope.contained_end());
				if (!parse_result.valid) {
					return this->make_result(cursor, false, parse_result.error_message_);
				} // Else continue parsing...
				
				auto& type_expression = type_directive.add_statement(StatementKind::type_expression_,
					type_expression_scope.contained_begin(),
					type_expression_scope.contained_end());

				auto type_expr_result = ParseTypeExpression()(parser,
					type_expression,
					type_expression_scope.contained_begin(),
					type_expression_scope.contained_end());
				if (!type_expr_result.valid) {
					return this->make_result(cursor, false, type_expr_result.error_message_);
				} // Else return the end of expression.

				return this->make_result(end);
			}
		}
		return this->make_result(cursor, false, "[PROGRAMMER ERROR] parse_directive_type() reached end of function without returning.");
	}







	// store a call to a function object
	//ProgramParser::parsing_method_t f_display_obj = ParseTypeExpression();
	// Map of all possible directives and their corresponding parsing functions.
	const ProgramParser::parsing_method_map_t ProgramParser::directive_map_ = {
		{SyntaxTraits::DirectiveLiteral::type , parsing_method_t(ParseDirectiveType())}
		//{SyntaxTraits::DirectiveLiteral::vari , parsing_method_t(&ProgramParser::parse_directive_vari)}
		//{SyntaxTraits::DirectiveLiteral::func , &ProgramParser::parse_directive_func},
		//{SyntaxTraits::DirectiveLiteral::class_ , &ProgramParser::parse_directive_class},
		//{SyntaxTraits::DirectiveLiteral::if_ , &ProgramParser::parse_directive_if},
		//{SyntaxTraits::DirectiveLiteral::else_ , &ProgramParser::parse_directive_else},
		//{SyntaxTraits::DirectiveLiteral::elif_ , &ProgramParser::parse_directive_elif},
		//{SyntaxTraits::DirectiveLiteral::while_ , &ProgramParser::parse_directive_while},
		//{SyntaxTraits::DirectiveLiteral::for_ , &ProgramParser::parse_directive_for},
		//{SyntaxTraits::DirectiveLiteral::forif_ , &ProgramParser::parse_directive_forif},
		//{SyntaxTraits::DirectiveLiteral::ret , &ProgramParser::parse_directive_ret},
		//{SyntaxTraits::DirectiveLiteral::break_ , &ProgramParser::parse_directive_break},
		//{SyntaxTraits::DirectiveLiteral::continue_ , &ProgramParser::parse_directive_continue},
		//{SyntaxTraits::DirectiveLiteral::into_ , &ProgramParser::parse_directive_into},
		//{SyntaxTraits::DirectiveLiteral::goto_ , &ProgramParser::parse_directive_goto},
		//{SyntaxTraits::DirectiveLiteral::out_ , &ProgramParser::parse_directive_out}
	};







}

//
//
//
//ProgramParser::token_iterator ProgramParser::parse_type_identity_expression(Statement& parent,
//	ProgramParser::token_iterator cursor, ProgramParser::token_iterator end) {
//	// Get the identity token	
//	auto identity = get(cursor);
//	if (identity.type() != TokenKind::alnumus) {
//		std::cerr << "[ERROR] Invalid type identity expression. Must evaluate to a single alnumus.Got:" << identity.lit() << std::endl;
//		return cursor; // TODO: error handling
//	}
//	else {
//		// 3.1. If the type is found, it is added to the type_directive_
//		// 3.2. If the type is not found, it is an error.
//		auto type_it = global_type_table_.find(identity.lit());
//		if (type_it != global_type_table_.end()) {
//			// Add the identity to the type expression
//			parent.add_statement(StatementKind::identifier_expression_, cursor, end);
//			// Advance cursor to the end of the scope and return.
//			advance_to(cursor, end);
//			return cursor; // TODO: return the type
//		}
//		else {
//			std::cerr << "[ERROR] Invalid type identity expression. Identity not found." << std::endl;
//			return cursor; // TODO: error handling
//		}
//	}
//}
//
//ProgramParser::token_iterator ProgramParser::parse_type_identity_declaration(Statement& parent,
//	ProgramParser::token_iterator cursor, ProgramParser::token_iterator end) {
//	// if the identity is not an alnumus, it is an error.
//	auto identity = get(cursor);
//	if (identity.type() != TokenKind::alnumus) {
//		std::cerr << "[ERROR] Invalid type identity declaration. Must evaluate to a single alnumus." << std::endl;
//		return cursor; // TODO: error handling
//	}
//	else {
//		// 3.1. If the type identity is found, it is an error. 
//		// 3.2. If the type identity is not found, it is added to the global type map added to the type_directive_.
//		auto type_it = global_type_table_.find(identity.lit());
//		if (type_it == global_type_table_.end()) {
//			// Add the identity to the type expression
//			parent.add_statement(StatementKind::identifier_expression_
//				, cursor, end);
//			// Advance cursor to the beginning of next scope.
//			advance_to(cursor, end);
//			return cursor;
//		}
//		else {
//			std::cerr << "[ERROR] Invalid type identity declaration. Identity already exists." << std::endl;
//			return cursor; // TODO: error handling
//		}
//	}
//}
//
//ProgramParser::token_iterator ProgramParser::parse_type_expression(Statement& parent,
//	ProgramParser::token_iterator cursor, ProgramParser::token_iterator end) {
//	assert(parent.kind() == StatementKind::type_expression_ && "Statement passed to TypeExpressionParser must be of type type_expression_.");
//	// The next scope is the type expression, which must be a candil special or type directive token.
//	if (get(cursor).kind == TokenKind::candi) {
//		auto candi_lit = get(cursor).lit();
//		// candi special must be one of the type literals
//		if (std::any_of(SyntaxTraits::candi_special_literals.begin(),
//			SyntaxTraits::candi_special_literals.end(),
//			[candi_lit](auto match) { return match == candi_lit; })) {
//			parent.add_statement(StatementKind::type_expression_, cursor, end);
//			advance_to(cursor, end);
//			return cursor;
//		}
//		else {
//			std::cerr << "[ERROR] Invalid candi special type literal: " << candi_lit << std::endl;
//			return cursor; // TODO: error handling
//		}
//	}
//	else if (get(cursor).kind == TokenKind::directive && get(cursor).lit() == SyntaxTraits::DirectiveLiteral::type) {
//		// recursiveley call parse_directive_type until a single argument is returned.
//		auto& type_expr = parent.add_statement(StatementKind::type_expression_, cursor, end);
//		auto type_expr_end = parse_directive_type(type_expr, cursor, end);
//		advance_to(cursor, type_expr_end);
//		return cursor;
//	}
//	else {
//		std::cerr << "[ERROR] Invalid type expression. Must contain directive or candi special literal. Got:" << get(cursor).lit() << std::endl;
//		return cursor; // TODO: error handling
//	}
//}
//
//// <@method:parse_value_expression>
//// The value expression may contain identities, literals, or operators.
//// Iterate over the expression. For each token, check if it is an identity, literal, or operator.
//// 1. If it is an identity, look up the value in the global variable table.
//
//// 1.1. Check for a following operator. If there is one. This identity becomes the left operand.
//// 1.2. Search for a right operand. If there is not one - it is an error.
//// 1.3. If there is a right operand, check for a following operator. If the following operator has a higher precedence - it must be solved first.
//// 1.3.0. Recursively call parse_value_expression() with the right operand as the beginning of the expression. Until the expression is solved.
//// 1.4. If the following operator has a lower precedence or equal precedence, solved the binary expression.
//// 1.4.0. The result becomes the left operand of the next operator.
//// 1.4.1. Continue until a literal or identifier is not followed by an operator.
//// 2. If it is a literal, evaluate the literal.
//// Repeat steps 1.1 - 1.4.
//ProgramParser::token_iterator ProgramParser::parse_value_expression(Statement& parent,
//	ProgramParser::token_iterator cursor, ProgramParser::token_iterator end) {
//	// Check if the value expression is a constant expression.
//	// Constant expressions must only contain literal values and literal operators.
//	//auto is_constexpr = std::all_of(cursor, end, [](auto& token) {
//	//	return token.get().is_literal_value() || token.get().is_literal_operator();
//	//	});
//
//	//if (is_constexpr) {
//	//	// If the expression is a constant expression, evaluate it and add it to the parent.
//	//	// 2 * 2 + 4 + 2 * 5 = 18
//	//	// 2 * (2 + 4 + 2) * 5 = 80
//	//	// 2 * (2 + (4 + 2) * 5) = 64
//
//	//	std::any result = 0;
//	//	std::type_index result_type{typeid(void)};
//	//	for(auto i = cursor; i < end; i++){
//	//		auto curr_token = i->get();
//	//		// If the first token is a literal value, it is an operand. It is the initial left operand and therefore the result start.
//	//		if (curr_token.kind == TokenKind::number_literal) {
//	//			result_type = typeid(int);
//	//			result = evaluate_number_literal(curr_token);
//	//		}
//	//		else if (curr_token.kind == TokenKind::real_literal) {
//	//			result_type = typeid(double);
//	//			result = evaluate_real_literal(curr_token);
//	//		}
//	//		else if (curr_token.is_literal_operator()) {
//	//			if (curr_token.operation_type() == TokenOperationType::binary) {
//	//				auto lhs = result;
//	//				auto rhs_token = std::next(i)->get();
//
//	//				if (rhs_token.is_literal_value()) {
//	//					auto following_token = peek(i,2);
//	//					if (following_token.is_literal_operator()) {
//	//						// If the literal operator's precidence is higher or equal, then we may solve the expression.
//	//						if (following_token.precidence() >= curr_token.precidence()){
//	//							if (curr_token.associativity() == TokenAssociativity::left) { // type is inherited from the associated operand.
//	//								result = solve_binary_operation(curr_token, lhs, rhs_token);
//	//							}
//	//							else {
//	//								// Cast the lhs operand back into a literal token. Convert the rhs operand to a any value.
//	//								// Solve the binary operation
//	//								result = solve_binary_operation(curr_token, rhs, lhs_token);
//	//							}
//	//						}
//	//						else { // Recursively call parse_value_expression() with the right operand as the beginning of the expression.
//
//	//							rhs = solve_value_expression(parent, i + 1, end);
//	//						}
//	//						// If the next operator has a lower precedence or equal precedence, solved the binary expression.
//	//						// The result becomes the left operand of the next operator.
//	//						// Continue until a literal or identifier is not followed by an operator.
//	//					}
//	//					else {
//	//						// If the next token is not a literal operator, it is an error.
//	//					}
//	//				}
//	//				else {
//	//					// If the next token is not a literal value, it is an error.
//	//				}
//	//			}
//	//		}
//	//		// If the first token is a scope, it must be a parenthetical expression - get the contained scope and evaluate that instead.
//	//	}
//
//	//}
//
//	while (cursor != end) {
//		auto token = get(cursor);
//		if (token.kind == TokenKind::alnumus) {
//			// 1. If it is an identity, look up the value in the global variable table.
//			auto identity = token.lit();
//			auto value_it = global_variable_table_.find(identity);
//			if (value_it != global_variable_table_.end()) {
//				auto lhs_operand = value_it->second;
//				// 1.1. Check for a following operator. If there is one. This identity becomes the left operand.
//				//		Else end the expression.
//				if (peek(cursor, 1).kind == TokenKind::addition) {
//					// 1.1. Check for a following operator. If there is one. This identity becomes the left operand.
//					// 1.2. Search for a right operand. If there is not one - it is an error.
//					// 1.3. If there is a right operand, check for a following operator. If the following operator has a higher
//				}
//				else {
//					// Convert the identifier to a literal value.
//
//					// Advance to end ,add the expression to the parent.
//					parent.add_statement(StatementKind::value_expression_, cursor, end);
//					advance(cursor);
//					return cursor;
//				}
//
//				// 1.2. Search for a right operand. If there is not one - it is an error.
//				// 1.3. If there is a right operand, check for a following operator. If the following operator has a higher precedence - it must be solved first.
//				// 1.3.0. Recursively call parse_value_expression() with the right operand as the beginning of the expression. Until the expression is solved.
//				// 1.4. If the following operator has a lower precedence or equal precedence, solved the binary expression.
//				// 1.4.0. The result becomes the left operand of the next operator.
//				// 1.4.1. Continue until a literal or identifier is not followed by an operator.
//			}
//			else {
//				std::cerr << "[ERROR] Invalid identity in value expression. Identity not found: " << identity << std::endl;
//				return cursor; // TODO: error handling
//			}
//		}
//		else if (token.kind == TokenKind::number_literal) {
//			// 2. If it is a literal, evaluate the literal.
//			// Repeat steps 1.1 - 1.4.
//		}
//		else if (token.kind == TokenKind::real_literal) {
//			// 2. If it is a literal, evaluate the literal.
//			// Repeat steps 1.1 - 1.4.
//		}
//		//else if (token.kind == TokenKind::operator_) {
//		//	// 1.1. Check for a following operator. If there is one. This identity becomes the left operand.
//		//	// 1.2. Search for a right operand. If there is not one - it is an error.
//		//	// 1.3. If there is a right operand, check for a following operator. If the following operator has a higher
//		//}
//	}
//}
//
//ProgramParser::token_iterator ProgramParser::parse_directive_type(Statement& parent,
//	ProgramParser::token_iterator cursor, ProgramParser::token_iterator end) {
//	assert(get(cursor).type() == TokenKind::directive && get(cursor).lit() == SyntaxTraits::DirectiveLiteral::type
//		&& "Token passed to parse_directive_type must be a #type directive.");
//	// 0. Advance past the directive
//	advance(cursor);
//
//	// 1. Must be followed by a basic scope with 1 or 2 seperated arguments.
//	auto directive_type_arg_scopes = get_base_seperated_scope(cursor);
//	if (directive_type_arg_scopes.size() != 2 && directive_type_arg_scopes.size() != 1) {
//		std::cerr << "[ERROR] #type directive must be followed by 1 or 2 arguments." << std::endl;
//		return cursor; // TODO: error handling
//	}
//
//	// 2. Scopes must be valid. 
//	for (auto& scope : directive_type_arg_scopes) {
//		if (!scope.valid) {
//			std::cerr << "[ERROR] #type directive argument scope is invalid. Mismatched parentheses?" << std::endl;
//			return cursor; // TODO: error handling
//		}
//	}
//
//	// Now we can add the type directive to the compiled program.
//	auto& type_directive = parent.add_statement(StatementKind::type_directive_,
//		directive_type_arg_scopes.front().scope_begin(), directive_type_arg_scopes.back().scope_end());
//
//	// 3. If scope has 1 argument, it is requesting a type from the type table.
//	if (directive_type_arg_scopes.size() == 1) {
//		// 3.0. The argument must evaluate to an identity_. If it does not, it is an error.
//		// TEMP: For now we will only accept a single alnumus as the identity.
//		auto& identity_expression_scope = directive_type_arg_scopes[0];
//		if (identity_expression_scope.is_empty()) {
//			std::cerr << "[WARNING] Empty identity expression in #type directive." << std::endl;
//			return cursor; // TODO: error handling
//		}
//		else {
//			parse_type_identity_expression(type_directive, identity_expression_scope.contained_begin(), identity_expression_scope.contained_end());
//			advance_to(cursor, identity_expression_scope.scope_end());
//			return cursor;
//		}
//	}
//	// 4. If scope has 2 arguments, it is defining a new type.
//	else { // else must be 2 because we checked in step 1. and returned if it was not 1 or 2.
//		// Store the first scope 
//		auto& identity_declaration_scope = directive_type_arg_scopes[0];
//		auto& type_expression_scope = directive_type_arg_scopes[1];
//		// 4.0. The first argument must evaluate to a type_identity_. If it does not, it is an error.
//		if (identity_declaration_scope.is_empty()) {
//			std::cerr << "[WARNING] Empty identity declaration in #type directive." << std::endl;
//			return cursor; // TODO: error handling
//		}
//		else {
//			// Advance cursor to begin of contained scope
//			parse_type_identity_declaration(type_directive, identity_declaration_scope.contained_begin(), identity_declaration_scope.contained_end());
//			advance_to(cursor, identity_declaration_scope.scope_end());
//
//			parse_type_expression(type_directive, type_expression_scope.contained_begin(), type_expression_scope.contained_end());
//			advance_to(cursor, type_expression_scope.scope_end());
//			return cursor;
//		}
//	}
//	std::cerr << "[PROGRAMMER ERROR] parse_directive_type() reached end of function without returning." << std::endl;
//	return cursor;
//}
//
//ProgramParser::token_iterator ProgramParser::parse_directive_vari(Statement& parent,
//	ProgramParser::token_iterator cursor, ProgramParser::token_iterator end) {
//	assert(get(cursor).type() == TokenKind::directive && get(cursor).lit() == SyntaxTraits::DirectiveLiteral::vari
//		&& "Token passed to parse_directive_vari must be a #vari directive.");
//	// 0. Advance past the directive
//	advance(cursor);
//
//	// 1. Must be followed by a basic scope with 2 or 3 seperated arguments.
//	auto directive_vari_arg_scopes = get_base_seperated_scope(cursor);
//	if (directive_vari_arg_scopes.size() != 2 && directive_vari_arg_scopes.size() != 3) {
//		std::cerr << "[ERROR] #vari directive must be followed by 2 or 3 arguments." << std::endl;
//		return cursor; // TODO: error handling
//	}
//
//	// 2. Scopes must be valid. 
//	for (auto& scope : directive_vari_arg_scopes) {
//		if (!scope.valid) {
//			std::cerr << "[ERROR] #vari directive argument scope is invalid. Mismatched parentheses?" << std::endl;
//			return cursor; // TODO: error handling
//		}
//	}
//
//	// Now we can add the type directive to the compiled program.
//	auto& vari_directive = parent.add_statement(StatementKind::vari_directive_,
//		directive_vari_arg_scopes.front().scope_begin(), directive_vari_arg_scopes.back().scope_end());
//
//	// 3. If scope has 2 arguments then we are defining a new variable with no initial value.
//	if (directive_vari_arg_scopes.size() == 2) {
//		auto& identity_expression_scope = directive_vari_arg_scopes[0];
//		auto& type_expression_scope = directive_vari_arg_scopes[1];
//		if (identity_expression_scope.is_empty()) {
//			std::cerr << "[WARNING] Empty identity expression in #vari directive." << std::endl;
//			return cursor; // TODO: error handling
//		}
//		else {
//			// Get the identity expression
//			parse_type_identity_expression(vari_directive, identity_expression_scope.contained_begin(), identity_expression_scope.contained_end());
//			advance_to(cursor, identity_expression_scope.scope_end());
//
//			if (type_expression_scope.is_empty()) {
//				std::cerr << "[WARNING] Empty type expression in #vari directive." << std::endl;
//				return cursor; // TODO: error handling
//			}
//			else {
//				// Get the type expression
//				parse_type_expression(vari_directive, type_expression_scope.contained_begin(), type_expression_scope.contained_end());
//				advance_to(cursor, type_expression_scope.scope_end());
//				return cursor;
//			}
//		}
//
//	}
//	else { // The scope has 3 arguments, we are defining a new variable with an initial value.
//		auto& identity_expression_scope = directive_vari_arg_scopes[0];
//		auto& type_expression_scope = directive_vari_arg_scopes[1];
//		auto& value_expression_scope = directive_vari_arg_scopes[2];
//		if (identity_expression_scope.is_empty()) {
//			std::cerr << "[WARNING] Empty identity expression in #vari directive." << std::endl;
//			return cursor; // TODO: error handling
//		}
//		else {
//			// Get the identity expression
//			parse_type_identity_expression(vari_directive, identity_expression_scope.contained_begin(), identity_expression_scope.contained_end());
//			advance_to(cursor, identity_expression_scope.scope_end());
//
//			if (type_expression_scope.is_empty()) {
//				std::cerr << "[WARNING] Empty type expression in #vari directive." << std::endl;
//				return cursor; // TODO: error handling
//			}
//			else {
//				// Get the type expression
//				parse_type_expression(vari_directive, type_expression_scope.contained_begin(), type_expression_scope.contained_end());
//				advance_to(cursor, type_expression_scope.scope_end());
//
//				if (value_expression_scope.is_empty()) {
//					std::cerr << "[WARNING] Empty value expression in #vari directive." << std::endl;
//					return cursor; // TODO: error handling
//				}
//				else {
//					// Get the value expression
//					parse_value_expression(vari_directive, value_expression_scope.contained_begin(), value_expression_scope.contained_end());
//					advance_to(cursor, value_expression_scope.scope_end());
//					return cursor;
//				}
//			}
//		}
//	}
//	std::cerr << "[PROGRAMMER ERROR] parse_directive_vari() reached end of function without returning." << std::endl;
//	return cursor;
//} // end parse_directive_vari()
//
///*
//* Evaluation methods will take a statement type and evaluate it to a literal value.
//* This is the terminating step in the recursion of the statement body.
//* Evluation methods will take a statement and evaluate it to a literal value.
//* The return type of the evaluation methods will be a variant of all possible literal types. FIXME: this is not implemented yet.
//* TEMPORARY: std::any will be used instead of a variant until the variant is implemented.
//*/
//
/////* Single token evaluation methods */
////value_eval_result evaluate_number_literal(const Token & number_token) {
////	assert(number_token.kind == TokenKind::number_literal
////		&& "Token passed to evaluate_number_literal must be of type number_literal_");
//
////	try {
////		return std::stoi(number_token.lit());
////	}
////	catch (std::invalid_argument& e) {
////		std::cerr << "Invalid argument passed to evaluate_number_literal: " << e.what() << std::endl;
////		return std::nullopt;
////	}
////	catch(std::out_of_range& e){
////		std::cerr << "Out of range argument passed to evaluate_number_literal: " << e.what() << std::endl;
////		return std::nullopt;
////	}
////}
////value_eval_result evaluate_real_literal(const Token & real_token) {
////	assert(real_token.kind == TokenKind::real_literal
////		&& "Token passed to evaluate_real_literal must be of type real_literal_");
//
////	try {
////		return std::stod(real_token.lit());
////	}
////	catch (std::invalid_argument& e) {
////		std::cerr << "Invalid argument passed to evaluate_number_literal: " << e.what() << std::endl;
////		return std::nullopt;
////	}
////	catch (std::out_of_range& e) {
////		std::cerr << "Out of range argument passed to evaluate_number_literal: " << e.what() << std::endl;
////		return std::nullopt;
////	}
////}
////type_eval_result evaluate_candispecial_type(const Token& candi_token) {
////	assert(candi_token.kind == TokenKind::candi
////		&& "Token passed to evaluate_candispecial_type must be of type candispecial_type_");
//
////	auto literal = candi_token.lit();
//
////	if (literal == SyntaxTraits::CandiSpecialLiteral::a_int) {
////		return typeid(int);
////	}
////	else if (literal == SyntaxTraits::CandiSpecialLiteral::a_unsigned) {
////		return typeid(unsigned);
////	}
////	else if (literal == SyntaxTraits::CandiSpecialLiteral::a_real) {
////		return typeid(double);
////	}
////	else if (literal == SyntaxTraits::CandiSpecialLiteral::a_char) {
////		return typeid(char);
////	}
////	else if (literal == SyntaxTraits::CandiSpecialLiteral::a_bool) {
////		return typeid(bool);
////	}
////	else if (literal == SyntaxTraits::CandiSpecialLiteral::a_void) {
////		return typeid(void);
////	}
////	else {
////		std::cerr << "Invalid argument passed to evaluate_candispecial_type: " << literal << std::endl;
////		return typeid(UndefinedType);
////	}
////}
//
///*
//* Parsing methods
//* All parsing methods will return the end of the parsed statement.
//* The cursor is passed by value, advancing to the new cursor is done by the parent parsing method.
//* Parsing methods will call eachother recursively until the statement is in its maximally simplified form.
//* The parsing methods will be called by the compiled_program() or interpreted_program() methods.
//*/
///*
//* Directive parsing methods
//* In order of least to most dependency, meaning one parser may only call parsers that are above it or equal in the list.
//* 1. parse_directive_type
//* 2. parse_directive_vari
//* 3. parse_directive_func
//* 4. parse_directive_class
//*/
//token_iterator parse_directive_type(Statement& parent, token_iterator cursor, token_iterator end);
//token_iterator parse_directive_vari(Statement& parent, token_iterator cursor, token_iterator end);
///*
//* Expression parsing methods
//* 1. parse_identitity_expression
//* 2. parse_type_expression
//* 3. parse_value_expression
//*/
//token_iterator parse_type_identity_expression(Statement& parent, token_iterator cursor, token_iterator end);
//token_iterator parse_type_identity_declaration(Statement& parent, token_iterator cursor, token_iterator end);
//token_iterator parse_type_expression(Statement& parent, token_iterator cursor, token_iterator end);
//token_iterator parse_value_expression(Statement& parent, token_iterator cursor, token_iterator end);
