#pragma once
#include "global_dependencies.hpp"
#include "char_traits.hpp"
#include "token.hpp"
namespace caoco {
	class astnode {
	public:
		enum class e_type : int {
			none_ = -1, invalid_, eos_,
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

			unary_minus_,
			
			equal_, not_equal_, less_than_, greater_than_, 
			less_than_or_equal_, greater_than_or_equal_, three_way_comparison_, 
			
			atype_, aidentity_, avalue_, aint_, auint_, areal_, aoctet_, 
			abit_, aarray_, apointer_, amemory_, afunction_, cso_,

			aint_constrained_, auint_constrained_,
			
			enter_, start_, type_, var_, class_, func_, 
			const_, static_, ref_, 
			if_, else_, elif_, while_, for_, 
			switch_, case_, default_, break_, continue_, 
			return_, into_, //goto_,
			
			statement_, expression_, declaration_, operand_,
			program_, compiled_program_, interpreted_program_, 
			pragmatic_block_, functional_block_, 
			arguments_, type_constraints_, identifier_statement_, 
			function_call_, variable_assignment_, type_definition_, type_alias_,
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
		sl_list<astnode>& children_unsafe() { return body_; }
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
		astnode& pop_back() { body_.pop_back(); return body_.back(); }
		astnode& pop_front() { body_.pop_front(); return body_.front(); }
		astnode& front() { return body_.front(); }
		astnode& back() { return body_.back(); }

		// error handling only
		static constexpr auto type_to_string(e_type node) {
			std::string debug_string = "[";
			switch (node) {
			case e_type::none_: debug_string += "none_"; break;
			case e_type::invalid_: debug_string += "invalid_"; break;
			case e_type::string_literal_: debug_string += "string_literal_"; break;
			case e_type::number_literal_: debug_string += "number_literal_"; break;
			case e_type::real_literal_: debug_string += "real_literal_"; break;
			case e_type::alnumus_: debug_string += "alnumus_"; break;
			case e_type::simple_assignment_: debug_string += "simple_assignment_"; break;
			case e_type::addition_assignment_: debug_string += "addition_assignment_"; break;
			case e_type::subtraction_assignment_: debug_string += "subtraction_assignment_"; break;
			case e_type::multiplication_assignment_: debug_string += "multiplication_assignment_"; break;
			case e_type::division_assignment_: debug_string += "division_assignment_"; break;
			case e_type::remainder_assignment_: debug_string += "remainder_assignment_"; break;
			case e_type::bitwise_and_assignment_: debug_string += "bitwise_and_assignment_"; break;
			case e_type::bitwise_or_assignment_: debug_string += "bitwise_or_assignment_"; break;
			case e_type::bitwise_xor_assignment_: debug_string += "bitwise_xor_assignment_"; break;
			case e_type::left_shift_assignment_: debug_string += "left_shift_assignment_"; break;
			case e_type::right_shift_assignment_: debug_string += "right_shift_assignment_"; break;
			case e_type::increment_: debug_string += "increment_"; break;
			case e_type::decrement_: debug_string += "decrement_"; break;
			case e_type::addition_: debug_string += "addition_"; break;
			case e_type::subtraction_: debug_string += "subtraction_"; break;
			case e_type::multiplication_: debug_string += "multiplication_"; break;
			case e_type::division_: debug_string += "division_"; break;
			case e_type::remainder_: debug_string += "remainder_"; break;
			case e_type::bitwise_NOT_: debug_string += "bitwise_NOT_"; break;
			case e_type::bitwise_AND_: debug_string += "bitwise_AND_"; break;
			case e_type::bitwise_OR_: debug_string += "bitwise_OR_"; break;
			case e_type::bitwise_XOR_: debug_string += "bitwise_XOR_"; break;
			case e_type::bitwise_left_shift_: debug_string += "bitwise_left_shift_"; break;
			case e_type::bitwise_right_shift_: debug_string += "bitwise_right_shift_"; break;
			case e_type::negation_: debug_string += "negation_"; break;
			case e_type::logical_AND_: debug_string += "logical_AND_"; break;
			case e_type::logical_OR_: debug_string += "logical_OR_"; break;
			case e_type::equal_: debug_string += "equal_"; break;
			case e_type::not_equal_: debug_string += "not_equal_"; break;
			case e_type::less_than_: debug_string += "less_than_"; break;
			case e_type::greater_than_: debug_string += "greater_than_"; break;
			case e_type::less_than_or_equal_: debug_string += "less_than_or_equal_"; break;
			case e_type::greater_than_or_equal_: debug_string += "greater_than_or_equal_"; break;
			case e_type::three_way_comparison_: debug_string += "three_way_comparison_"; break;
			case e_type::atype_: debug_string += "atype_"; break;
			case e_type::aidentity_: debug_string += "aidentity_"; break;
			case e_type::avalue_: debug_string += "avalue_"; break;
			case e_type::aint_: debug_string += "aint_"; break;
			case e_type::auint_: debug_string += "auint_"; break;
			case e_type::areal_: debug_string += "areal_"; break;
				//case e_type::aureal_: debug_string += "aureal_"; break;
			case e_type::aoctet_: debug_string += "aoctet_"; break;
			case e_type::abit_: debug_string += "abit_"; break;
			case e_type::aarray_: debug_string += "aarray_"; break;
			case e_type::apointer_: debug_string += "apointer_"; break;
			case e_type::amemory_: debug_string += "amemory_"; break;
			case e_type::afunction_: debug_string += "afunction_"; break;
			case e_type::enter_: debug_string += "enter_"; break;
			case e_type::start_: debug_string += "start_"; break;
			case e_type::type_: debug_string += "type_"; break;
			case e_type::var_: debug_string += "var_"; break;
			case e_type::class_: debug_string += "class_"; break;
			case e_type::func_: debug_string += "func_"; break;
			case e_type::const_: debug_string += "const_"; break;
			case e_type::static_: debug_string += "static_"; break;
			case e_type::ref_: debug_string += "ref_"; break;
			case e_type::if_: debug_string += "if_"; break;
			case e_type::else_: debug_string += "else_"; break;
			case e_type::elif_: debug_string += "elif_"; break;
			case e_type::while_: debug_string += "while_"; break;
			case e_type::for_: debug_string += "for_"; break;
			case e_type::switch_: debug_string += "switch_"; break;
			case e_type::case_: debug_string += "case_"; break;
			case e_type::default_: debug_string += "default_"; break;
			case e_type::break_: debug_string += "break_"; break;
			case e_type::continue_: debug_string += "continue_"; break;
			case e_type::return_: debug_string += "return_"; break;
			case e_type::into_: debug_string += "into_"; break;
			case e_type::statement_: debug_string += "statement_"; break;
			case e_type::expression_: debug_string += "expression_"; break;
			case e_type::declaration_: debug_string += "declaration_"; break;
			case e_type::program_: debug_string += "program_"; break;
			case e_type::compiled_program_: debug_string += "compiled_program_"; break;
			case e_type::interpreted_program_: debug_string += "interpreted_program_"; break;
			case e_type::pragmatic_block_: debug_string += "pragmatic_block_"; break;
			case e_type::functional_block_: debug_string += "functional_block_"; break;
			case e_type::identifier_statement_: debug_string += "identifier_statement_"; break;
			case e_type::variable_assignment_: debug_string += "variable_assignment_"; break;
			case e_type::type_definition_: debug_string += "type_definition_"; break;
			case e_type::anon_variable_definition_: debug_string += "anon_variable_definition_"; break;
			case e_type::anon_variable_definition_assingment_: debug_string += "anon_variable_definition_assingment_"; break;
			case e_type::constrained_variable_definition_: debug_string += "constrained_variable_definition_"; break;
			case e_type::type_constraints_: debug_string += "type_constraints_"; break;
			case e_type::class_definition_: debug_string += "class_definition_"; break;
			case e_type::shorthand_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
			case e_type::shorthand_constrained_void_method_definition_: debug_string += "shorhand_void_method_definition_"; break;
			case e_type::arguments_: debug_string += "arguments_"; break;
			case e_type::function_call_: debug_string += "function_call_"; break;
			case e_type::method_definition_: debug_string += "method_definition_"; break;
			case e_type::eof_: debug_string += "eof_"; break;
			case e_type::period_: debug_string += "period_"; break;
			case e_type::conditional_block_: debug_string += "conditional_block_"; break;
			case e_type::conditional_statement_: debug_string += "conditional_statement_"; break;
			case e_type::operand_: debug_string += "operand_"; break;
			case e_type::unary_minus_: debug_string += "unary_minus_"; break;
			case e_type::type_alias_: debug_string += "type_alias_"; break;
			default: debug_string += "This node type is not string-convertible. Please implement a string conversion for this node type in the node_debug_string function in test.cpp.";
			}
			debug_string += "]";
			return debug_string;
		}
	};
	using astnode_enum = astnode::e_type;

} // namespace caoco