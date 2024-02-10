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
			return_, into_, //goto_,
			
			statement_, expression_, declaration_, 
			program_, compiled_program_, interpreted_program_, 
			pragmatic_block_, functional_block_, 
			arguments_, type_constraints_, identifier_statement_, 
			function_call_, variable_assignment_, type_definition_, 
			anon_variable_definition_, anon_variable_definition_assingment_, 
			constrained_variable_definition_, constrained_variable_definition_assingment_, method_definition_,
			constrained_method_definition_, shorthand_void_method_definition_, 
			shorthand_constrained_void_method_definition_, 
			class_definition_, 

			conditional_statement_,conditional_block_,
			
			open_scope_, close_scope_, open_list_, close_list_, open_frame_, close_frame_, eof_,
		};
	private:
		e_type type_;
		sl_u8string literal_{u8""};
		astnode* parent_{ nullptr };
		sl_list<astnode> body_;
	public:
		astnode() : type_(e_type::eof_){}
		astnode(e_type type) : type_(type){}
		astnode(e_type type, tk_vector_cit beg, tk_vector_cit end) 
			: type_(type) {
			literal_ = u8"";
			for (auto it = beg; it != end; it++) {
				literal_ += it->literal();
			}
		}
		astnode(e_type type, tk_vector_cit beg, tk_vector_cit end, astnode& parent) 
			: type_(type), parent_(&parent) {
			literal_ = u8"";
			for (auto it = beg; it != end; it++) {
				literal_ += it->literal();
			}
		}
		
		e_type type() const { return type_; }
		const sl_list<astnode>& children() const { return body_; }
		SL_CX sl_u8string literal() const {
			return literal_;
		}
		SL_CX sl_string literal_str() const {
			return sl::to_str(literal_);
		}
		const astnode& operator[](int index) const {
			return [this, &index]()->const astnode& {auto it = body_.begin();
			for (auto i = 0; i < index; i++) { it++; } return *it;
			}();
		}

		// Internal parser use only.
		astnode& push_back(astnode stmt) { body_.push_back(stmt); return body_.back(); }
		astnode& push_front(astnode stmt) { body_.push_front(stmt); return body_.front(); }
		astnode& front() { return body_.front(); }
		astnode& back() { return body_.back(); }
	};
	using astnode_enum = astnode::e_type;

} // namespace caoco