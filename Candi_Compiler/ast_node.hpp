#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
#include "token.hpp"
namespace caoco {
	class astnode {
	public:
		enum class e_type : int {
			none_ = -1, invalid_, 
			string_literal_, number_literal_, real_literal_, none_literal_, 
			alnumus_, unsigned_literal_, octet_literal_, bit_literal_, 
			
			period_, simple_assignment_, addition_assignment_, subtraction_assignment_, 
			multiplication_assignment_, division_assignment_, remainder_assignment_, 
			bitwise_and_assignment_, bitwise_or_assignment_, bitwise_xor_assignment_, 
			left_shift_assignment_, right_shift_assignment_, 
			
			increment_, decrement_, 
			addition_, subtraction_, multiplication_, division_, remainder_, 
			bitwise_NOT_, bitwise_AND_, bitwise_OR_, bitwise_XOR_, 
			bitwise_left_shift_, bitwise_right_shift_, 
			negation_, logical_AND_, logical_OR_, 
			
			equal_, not_equal_, less_than_, greater_than_, 
			less_than_or_equal_, greater_than_or_equal_, three_way_comparison_, 
			
			atype_, aidentity_, avalue_, aint_, auint_, areal_, aoctet_, 
			abit_, aarray_, apointer_, amemory_, afunction_, 
			
			enter_, start_, type_, var_, class_, func_, 
			const_, static_, ref_, 
			if_, else_, elif_, while_, for_, 
			switch_, case_, default_, break_, continue_, 
			return_, into_, 
			
			statement_, expression_, declaration_, 
			program_, compiled_program_, interpreted_program_, 
			pragmatic_block_, functional_block_, 
			arguments_, type_constraints_, identifier_statement_, 
			function_call_, variable_assignment_, type_definition_, 
			anon_variable_definition_, anon_variable_definition_assingment_, 
			constrained_variable_definition_, method_definition_, 
			constrained_method_definition_, shorthand_void_method_definition_, 
			shorthand_constrained_void_method_definition_, 
			class_definition_, 
			
			open_scope_, close_scope_, open_list_, close_list_, open_frame_, close_frame_, eof_,
		};
	private:
		e_type type_;
		tk_vector_cit beg_;			
		tk_vector_cit end_;			// One token past the end of the statement.
		astnode* parent_{ nullptr };
		sl_list<astnode> body_;
	public:
		astnode() : type_(e_type::eof_){}
		astnode(e_type type) : type_(type){}
		astnode(e_type type, tk_vector_cit beg, tk_vector_cit end) 
			: type_(type), beg_(beg), end_(end) {}
		astnode(e_type type, tk_vector_cit beg, tk_vector_cit end, astnode& parent) 
			: type_(type), beg_(beg), end_(end), parent_(&parent) {}
		
		e_type type() const { return type_; }
		const sl_list<astnode>& body() const { return body_; }
		astnode& front() { return body_.front(); }
		astnode& back() { return body_.back(); }
		bool empty() const { return body_.empty(); }
		astnode& push_back(astnode stmt) { body_.push_back(stmt); return body_.back(); }
		astnode& push_front(astnode stmt) { body_.push_front(stmt); return body_.front(); }

		astnode& insert(astnode statement, sl_list<astnode>::iterator it) {
			return *body_.insert(it, statement);
		}

		void change_parent(astnode& new_parent) {
			parent_ = &new_parent;
		}
		void change_body(sl_list<astnode> new_body) {
			body_ = new_body;
		}

		// Tokens
		SL_CX bool has_tokens() { return not (beg_ == end_); }
		SL_CX tk_vector_cit token_begin() { return beg_; }
		SL_CX tk_vector_cit token_end() { return end_; }

		// Get the tokens that make up the statement in the source code.
		SL_CX tk_vector get_tokens() const {
			return tk_vector(beg_, end_);
		}

		// Get a string representation of the statement. NOTE: returns a char8_t string.
		SL_CX sl_u8string literal() const  {
			sl_u8string statement_string = u8"";
			for (auto& token : get_tokens()) {
				statement_string += token.literal();
			}
			return statement_string;
		}

		// Get a string representation of the statement.
		SL_CX sl_string literal_str() const {
			sl_u8string statement_string = u8"";
			for (auto& token : get_tokens()) {
				statement_string += token.literal();
			}
			return sl::to_str(statement_string);
		}

	};
	using astnode_enum = astnode::e_type;

} // namespace caoco