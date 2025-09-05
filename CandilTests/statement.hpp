#pragma once
#include <list>
#include <any>
#include <concepts>
#include <type_traits>
#include "tokenizer.hpp"


namespace cil {
	enum class StatementKind {
		invalid_,				// An invalid statement is a statement that is not a valid statement.
		none_,					// A none statement is a statement that is not a statement.
		statement_,				// A statement is a single line of code.	
		expression_,			// An expression is a statement that returns a value.
		declaration_,			// A declaration is a statement that does not return a value.
		program_,				// A program is a collection of statements.
		compiled_program_,		// A compiled program is all the statemend within the #enter block.
		interpreted_program_,   // An interpreted program is all statement within the #start block.

		// Declarations
		method_declaration_,	// A method declaration creates a method identifier and signature. (not necesseraliy a definition)
		variable_declaration_,	// A variable declaration creates a variable identifier and type. (not necessarily a definition)
		// Candi Special Literals
		special_int_,
		// Literals
		literal_alnumus_,		// A literal alnumus is a declaration that defines a single literal alnumus value.
		literal_number_,
		// Directives
		type_directive_,		// A type directive is a declaration that defines a type.
			type_declaration_,	// A type declaration is a type directive with 1 argument.
			type_definition_,	// A type definition is a type directive with 2 arguments.
				type_expression_,	// Returns a type.
		vari_directive_,		// A vari directive is a declaration that defines a variable.
		identifier_expression_,	// Returns an identifier.
		    append_operation_,
			enumerate_operation_,
			range_operation_,
			identifier_list_,	// An identifier list is a declaration that defines a list of identifiers.
			literal_list_, // A literal list is a declaration that defines a list of literal values.
		value_expression_,		// Returns a value. Which may be one of variable or literal expression.
		variable_expression_,	// Returns a variable which cannot be determined at compile time.
		literal_expression_,	// Returns a literal value. Always determined at compile time.

		// Literals
		literal_value_,			// A literal value is a declaration that defines a literal value.
		integer_literal_,		// An integer literal is a declaration that defines an integer literal value.
		real_literal_,			// A float literal is a declaration that defines a float literal value.
		string_literal_,		// A string literal is a declaration that defines a string literal value.

		variable_definition_,	// A variable definition is a declaration that defines a variable.
		method_definition_,		// A method definition is a declaration that defines a method.

		// EOF
		eof_,
	};
	
		
		struct Statement {
		using statement_list = std::list<Statement>;

		StatementKind kind_;								// Type of statement.
		statement_list body;								// Body of the statement which recursively contains statements.
		TokenSanitizer::token_view_iterator beg_;			// Beginning of the statement.
		TokenSanitizer::token_view_iterator end_;			// One token past the end of the statement.

		Statement* parent{nullptr};									// Parent statement.

		Statement() : kind_(StatementKind::invalid_) {};
		Statement(StatementKind kind) :
			kind_(kind) {}
		Statement(StatementKind kind, TokenSanitizer::token_view_iterator beg, TokenSanitizer::token_view_iterator end) :
			kind_(kind), beg_(beg), end_(end) {}

		constexpr auto type() const { return kind_; }
		constexpr auto is_empty() { return beg_ == end_; }
		constexpr auto begin() { return beg_; }
		constexpr auto end() { return end_; }
		constexpr auto kind() { return kind_; }
		constexpr void morph(StatementKind kind, TokenSanitizer::token_view_iterator beg, TokenSanitizer::token_view_iterator end) { kind_ = kind; 
			beg_ = beg; end_ = end;
		}
		constexpr rvector<Token> get_tokens();		// Get the tokens that make up the statement in the source code.
		constexpr rvector<rstring> get_literals();	// Get the a vector of literals of the tokens that make up this statement.
		constexpr rstring to_string();				// Get a string representation of the statement.
		bool is_body_empty();															// Check if the body of the statement is empty.
		bool is_literal_value();													// Check if the body of the statement contains a singular literal_value declaration.
		Statement& add_statement(Statement statement);								// Adds a statement to the end of the body of the statement.
		Statement& add_statement(StatementKind kind, TokenSanitizer::token_view_iterator beg, TokenSanitizer::token_view_iterator end) {
			return add_statement(Statement(kind, beg, end));
		}
		Statement& emplace_statement(Statement statement);							// Adds a statement to the beginning of the body of the statement.
		Statement& insert_statement(Statement statement, statement_list::iterator it);	// Adds a statement to the body of the statement to the right of the specified iterator.
		
		Statement* find_parent(StatementKind kind) {
			if (parent == nullptr) return nullptr;
			if (parent->kind_ == kind) return parent;
			return parent->find_parent(kind);
		}
	    
	};


	// Get the tokens that make up the statement in the source code.
	constexpr rvector<Token> Statement::get_tokens() {
		return rvector<Token>(beg_, end_);
	}

	// Get the a vector of literals of the tokens that make up this statement.
	constexpr rvector<rstring> Statement::get_literals() {
		rvector<rstring> literals;
		for (auto& token : get_tokens()) {
			literals.push_back(token.lit());
		}
		return literals;
	}

	// Get a string representation of the statement.
	constexpr rstring Statement::to_string() {
		rstring statement_string = "";
		for (auto& token : get_tokens()) {
			statement_string += token.lit();
		}
		return statement_string;
	}

	// Check if the body of the statement is empty.
	bool Statement::is_body_empty() {
		return body.empty();
	}

	// Check if the body of the statement is a literal_value declaration.
	bool Statement::is_literal_value() {
		return !is_body_empty()
			&& body.size() == 1
			&& body.front().kind_ == StatementKind::literal_value_;
	}

	//  Adds a statement to the body of the statement.
	Statement& Statement::add_statement(Statement statement) {
		body.push_back(statement);
		body.back().parent = this;
		return body.back();
	}

	//  Adds a statement to the beginning of the body of the statement.
	Statement& Statement::emplace_statement(Statement statement) {
		body.emplace_front(statement);
		return body.front();
	}

	//  Adds a statement to the body of the statement to the left of the specified iterator.
	//  Returns an iterator to the newly inserted statement.
	// https://stackoverflow.com/questions/23038119/c-insertion-into-vector-after-or-before-iterator
	// https://en.cppreference.com/w/cpp/container/vector/insert
	Statement& Statement::insert_statement(Statement statement, statement_list::iterator it) {
		return *body.insert(it, statement);
	}
}