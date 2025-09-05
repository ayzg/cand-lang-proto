//#pragma once
//#include <string>
//#include <functional>
//#include <type_traits>
//#include <memory>
//#include <stack>
//
//
//
//namespace CandiLanguage
//{
//	const enum operator_argsc { NONE = 0, UNARY, BINARY};
//	template <typename ChildT>
//	class tokenbasebase {
//	public:
//		using token_base_t = ChildT;
//		using token_base_pointer_t = token_base_t * ;
//		using token_base_reference_t = token_base_t & ;
//		using token_base_const_pointer_t = const token_base_pointer_t;
//		using token_base_const_refrerence_t = const token_base_reference_t;
//		using stack_base_t = std::stack<token_base_pointer_t>;
//
//
//		virtual std::string to_string() = 0;
//	};
//
//	template <typename ChildT, typename BaseChildT = ChildT>
//	class OperandToken : virtual public tokenbasebase<BaseChildT>
//	{
//	public:
//		using token_t = ChildT;
//		using token_pointer_t = token_t * ;
//		using token_reference_t = token_t & ;
//		using token_const_pointer_t = const token_pointer_t;
//		using token_const_refrerence_t = const token_reference_t;
//		using stack_t = std::stack<token_pointer_t>;
//		using parent = tokenbasebase<BaseChildT>;
//	};
//
//	template <typename ChildT, size_t nArgs, typename BaseChildT = ChildT>
//	class OperatorToken : virtual public tokenbasebase<BaseChildT>
//	{
//		using token_t = ChildT;
//		using token_pointer_t = token_t * ;
//		using token_reference_t = token_t & ;
//		using token_const_pointer_t = const token_pointer_t;
//		using token_const_refrerence_t = const token_reference_t;
//		using stack_t = std::stack<token_pointer_t>;
//		using parent = tokenbasebase<BaseChildT>;
//
//		const size_t num_args = nArgs;
//
//		virtual std::enable_if<(nArgs == operator_argsc::NONE), typename OperandToken<BaseChildT>::token_pointer_t>
//			perform() = 0;
//
//		virtual std::enable_if<(nArgs == operator_argsc::UNARY),typename OperandToken<BaseChildT>::token_pointer_t>
//			perform(typename OperandToken<BaseChildT>::token_pointer_t) = 0;
//
//		virtual std::enable_if<(nArgs == operator_argsc::BINARY), typename OperandToken<BaseChildT>::token_pointer_t>
//			perform(typename OperandToken<BaseChildT>::token_pointer_t left_op,
//					typename OperandToken<BaseChildT>::token_pointer_t right_op) = 0;
//	};
//
//	class AdditionToken : OperatorToken<AdditionToken,operator_argsc::BINARY>
//	{
//
//	};
//
//	template<typename DataT = void *>
//	class PrimaryDataToken : public OperandToken<PrimaryDataToken<DataT>>
//	{
//		using data_t = DataT;
//		using data_pointer_t = data_t * ;
//		using data_const_pointer_t = data_t const *;
//		using data_reference_t = data_t & ;
//		using data_const_reference_t = data_t const &;
//		using parent = OperandToken<PrimaryDataToken<DataT>>;
//	protected:
//		data_t data_;
//	public:
//		data_t get_data() const { return data_; }
//		void set_data(data_t v) { data_ = v; }
//		virtual std::string to_string() { return std::to_string(data_); };
//	};
//
//	class Integer : public PrimaryDataToken<int>
//	{
//		using parent = PrimaryDataToken<int>;
//	public:
//		Integer() : parent() {}
//		Integer(int value) : parent() { data_ = value; }
//	};
//
//
//}