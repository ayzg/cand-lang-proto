#pragma once
#include <string>
#include <functional>
#include <type_traits>
#include <memory>
#include <stack>

///* stack type helper */
//template<typename T>
//using stack_t = std::stack<T>;
//
//using precedence_t = enum {
//			MIN = 0,
//			ASSIGNMENT,
//			LOGOR,
//			LOGXOR,
//			LOGAND,
//			BITOR,
//			BITXOR,
//			BITAND,
//			EQUALITY,
//			RELATIONAL,
//			BITSHIFT,
//			ADDITIVE,
//			MULTIPLICATIVE,
//			POWER,
//			UNARY,
//			POSTFIX,
//			MAX 
//};
//
//#define META_TOKENBASE(tokentype)					 \
//public:												 \
//	using token_type = tokentype;					 \
//	using pointer_type = std::shared_ptr<token_type>;\
//	using stack_type = stack_t<pointer_type>;		 \
//
//
//// Token Base Class
//// All tokens have:
//// 1. Type traits meta data
//// 2. A Name : Name of Class
//// 3. A precedence value
//// 3. To String Method
//class TokenBase {
//	META_TOKENBASE(TokenBase)
//public:
//	using string_type = std::string;
//	using value_type = void*;
//
//
//	virtual precedence_t get_precedence() const = 0;	
//	virtual ~TokenBase() = 0;
//	virtual string_type	to_string() const;
//};
//
//// Operand base
//// 1. type of a value | void * default
//class OperandBase : virtual public TokenBase
//{
//	META_TOKENBASE(TokenBase)
//public:
//	using value_type = void *;
//	virtual ~OperandBase() = 0;
//};
//
////Recieves and outputs a token
//class OperationTokenBase : virtual public TokenBase
//{
//	META_TOKENBASE(TokenBase)
//public:
//	virtual ~OperationTokenBase() = 0;
//	virtual unsigned number_of_args() const = 0;
//	virtual TokenBase::pointer_type perform(const TokenBase::stack_type & input_operands) = 0;
//
//
//};
//
////Recives operands and outputs an operand
//class OperatorTokenBase : virtual public OperationTokenBase
//{
//	META_TOKENBASE(TokenBase)
//public:
	//	virtual OperandBase::pointer_type perform(const OperandBase::stack_type & currentOperands) = 0;
//	virtual unsigned number_of_args() const = 0;
//	virtual ~OperatorTokenBase() = 0;
//};
//

template <typename class ChildT>
class tokenbase {
public:
	using token_t = ChildT;
	using pointer_t = std::shared_ptr<token_t>;
	using stack_t = std::stack<pointer_t>;
	using string_t = std::string;

	virtual std::string to_string() = 0;
};

template <typename class ChildT, typename BaseChildT = ChildT>
class OperandBase : virtual public tokenbase<BaseChildT>
{
	virtual std::string to_string() = 0;
};


template<typename ValueT>
class variable : public OperandBase<variable<ValueT>>
{
	using  par_t = OperandBase<variable<ValueT>>;
	using value_t = ValueT;
	using value_pointer_t = value_t * ;
	using value_const_pointer_t = value_t const *;
	using value_reference_t = value_t & ;
	using value_pointer_t = value_t const &;
protected:
	value_t value_;
public:
	value_t get_value() const { return value_; }
	void set_value(value_t& v) { value_ = v; } 	
	virtual par_t::string_t to_string() { return std::to_string(value_); };

};


//template <typename V>
//struct child : public parent<child, V> {
//	void do_something(V argument);
//};

