#include "ast_node.hpp"
#include "parser.hpp"
#include <variant>
#include <list>
#include <map>

namespace caoco {

struct none_t {
	bool operator==(const none_t&) const {
		return true;
	}
};

// string to unsigned int
unsigned stou(std::string const& str, size_t* idx = 0, int base = 10) {
	unsigned long result = std::stoul(str, idx, base);
	if (result > std::numeric_limits<unsigned>::max()) {
		throw std::out_of_range("stou");
	}
	return result;
}

struct RTValue {
	enum eType {
		NUMBER = 0,
		REAL = 1,
		STRING = 2,
		BIT = 3,
		OCTET = 4,
		NONE = 5,
		UNSIGNED = 6,
	} type;

	std::variant<int, double, std::string, bool, unsigned char, none_t,unsigned> value;

RTValue() : type(NONE), value(none_t{}) {}

template<typename T>
RTValue(eType t, T v) : type(t), value(v) {}

RTValue(const RTValue& other) {
	type = other.type;
	value = other.value;
}

RTValue(RTValue&& other) {
	type = other.type;
	value = std::move(other.value);
}

RTValue& operator=(RTValue&& other) {
	type = other.type;
	value = std::move(other.value);
	return *this;
}

RTValue& operator=(const RTValue& other) {
	type = other.type;
	value = other.value;
	return *this;
}

};

class rtenv {
	using string_t = std::string;
	using variable_map_t = std::map<std::string, RTValue>;
	using variable_map_iterator_t = variable_map_t::iterator;
	using variable_map_const_iterator_t = variable_map_t::const_iterator;

	using rtenv_list = std::list<rtenv>;
	using rtenv_iterator = rtenv_list::iterator;
	using rtenv_const_iterator = rtenv_list::const_iterator;

	// Purpose of this struct is to avoid testing for the end() iterator. Instead check if the returned iterator is valid. 
	// Which may be the end() of parent env. Such a comparison will result in an Incompatible Iterator error.
	struct local_variable_process_result {
		variable_map_iterator_t it;
		bool valid;

		const string_t& name() {
			return it->first;
		}
		RTValue& value() {
			return it->second;
		}
	};
	struct local_const_variable_process_result {
		variable_map_const_iterator_t it;
		bool valid;

		const string_t& name() const {
			return it->first;
		}

		const RTValue& value() const {
			return it->second;
		}
	};

	struct env_variable_process_result {
		local_variable_process_result local;
		rtenv& env;

		auto iterator() {
			return local.it;
		}

		const string_t& name() {
			return local.name();
		}

		RTValue& value() {
			return local.value();
		}

		bool valid() {
			return local.valid;
		}
	};
	struct env_const_variable_process_result {
		local_const_variable_process_result local;
		const rtenv& env;

		auto iterator() const {
			return local.it;
		}

		const string_t& name() const {
			return local.name();
		}

		const RTValue& value() const {
			return local.value();
		}

		bool valid() const {
			return local.valid;
		}
	};

	std::string name_;
	rtenv* parent_{nullptr};
	std::list<rtenv> children_;
	variable_map_t variables_;


	variable_map_iterator_t vari_end() {
		return variables_.end();
	}
	variable_map_iterator_t vari_begin() {
		return variables_.begin();
	}
	variable_map_const_iterator_t vari_cend() const {
		return variables_.end();
	}
	variable_map_const_iterator_t vari_cbegin() const {
		return variables_.begin();
	}
	
	variable_map_const_iterator_t find_variable(const std::string& name) const {
		// Note this only finds a local variable, returns end() if not found.
		return variables_.find(name);
	}

	inline bool has_parent() const {
		return parent_ != nullptr;
	}
	rtenv_const_iterator subenv_end() const {
		return children_.end();
	}
	rtenv_const_iterator subenv_begin() const {
		return children_.begin();
	}
	public:
	rtenv() = default;
	rtenv(const std::string& name) : name_(name) {}
	rtenv(const std::string& name, rtenv& parent) : name_(name), parent_(&parent) {}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Local Environment Operations
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	
	// <@method:create_variable> Create a variable in the current environment
	local_variable_process_result create_variable(const std::string& name, const RTValue& value) {
		// local vars will shadow parent vars
		auto inserted = variables_.insert({ name, value });
		return { inserted.first, inserted.second };
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Environment Sensitive Operations
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//

	// <@method:resolve_variable> Find the variable in the current environment or in the parent environment(if exists)
	env_const_variable_process_result resolve_variable(const std::string& name) const {
		auto it = find_variable(name);
		if (it != vari_cend()) {
			return { { it,true },*this };
		}
		if (has_parent()) {
			return parent_->resolve_variable(name);
		}
		return {{ vari_cend(),false },*this };
	}

	// <@method:delete_variable> Delete a variable in the current or parent environment
	bool delete_variable(const std::string & name) {
		auto resolved_var = get_variable(name);
		// The env which owns the var must delete it!!!
		if (resolved_var.valid()) {
			resolved_var.env.variables_.erase(resolved_var.iterator());
			return true; // Deleted Successfully
		}
		else { // cant delete a non-existing var
			return false;
		}
	}

	// <@method:get_variable> Get the variable in the current environment or in the parent environment(if exists)
	// Same as <@method:resolve_variable> but returns a modifyable iterator
	env_variable_process_result get_variable(const std::string& name) {
		auto it = variables_.find(name);
		if (it != variables_.end()) { // Found locally
			return { { it,true },*this };
		}
		else {
			if (has_parent()) { // Find in the parent
				return parent_->get_variable(name);
			}
			else { // No parent
				return { { variables_.end(),false },*this };
			}
		}
	}

	// <@method:set_variable> Set the variable in the current environment or in the parent environment(if exists)
	env_variable_process_result set_variable(const std::string& name, const RTValue& value) {
		auto resolved_var = get_variable(name);
		if (resolved_var.valid()) {
			resolved_var.value() = value;
			return resolved_var;
		}
		else { // Could not find var in the current env or in the parent env
			return { {vari_end(),false},*this };
		}
	}

};


struct EnvEvalProcess {
	virtual RTValue eval(const Node & node, rtenv& env) = 0;
	virtual ~EnvEvalProcess() = default;
public:
	RTValue operator()(const Node& node, rtenv& env) {
		return eval(node, env);
	};
};

#define caoco_def_env_eval_process(n) struct n : public EnvEvalProcess{\
	RTValue eval(const caoco::Node & node, caoco::rtenv& env) override; \
};

#define caoco_impl_env_eval_process(n) RTValue n::eval(const caoco::Node & node, caoco::rtenv& env)

// All tokens in the AST hold caoco::string_t, whitch is a utf-8 encoded string
inline auto get_node_cstr(const Node & node) {
	return to_std_string(node.to_string());
}

// Util method for single node evaluators which use the same pattern.
template<typename RetrieverT> requires std::invocable<RetrieverT, const std::string&>
inline RTValue get_node_rtvalue(const Node& node, RTValue::eType rtval_type,RetrieverT && retrival_policy) {
	auto literal = get_node_cstr(node);
	try {
		return RTValue{ rtval_type, retrival_policy(literal) };
	}
	catch (std::exception e) {
		std::string msg = e.what();
		throw std::runtime_error("get_node_rtvalue:Invalid literal:" + literal + ":Error:" + msg);
	}
}

inline RTValue make_rtval_none() {
	return RTValue(RTValue::NONE, none_t{});
}

// Literal Evaluators
caoco_def_env_eval_process(CNumberEval); // 1234
caoco_def_env_eval_process(CRealEval); // 1234.1234
caoco_def_env_eval_process(CStringEval); // 'Hello World'
caoco_def_env_eval_process(CBitEval); // 1b or 0b
caoco_def_env_eval_process(CUnsignedEval); // 1234u
caoco_def_env_eval_process(COctetEval); // 0c-255c and ' 'c where ' ' is a character from the ascii table
caoco_def_env_eval_process(CNoneEval); // #none directive
caoco_def_env_eval_process(CLiteralEval); // Dispatches to the above evaluators based on node type.

// Evironment-Sensitive Evaluators
caoco_def_env_eval_process(CAddOpEval); // +
caoco_def_env_eval_process(CSubOpEval); // -
caoco_def_env_eval_process(CMultOpEval); // *
caoco_def_env_eval_process(CDivOpEval); // /
caoco_def_env_eval_process(CModOpEval); // %

caoco_def_env_eval_process(CBinopEval); // Dispatches to binary ops

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Constant Evaluator Processes Implementations
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
caoco_impl_env_eval_process(CNumberEval) {
	return get_node_rtvalue(node, RTValue::NUMBER,
		[](const std::string& literal) {
			return std::stoi(literal);
		}
	);
}

caoco_impl_env_eval_process(CRealEval) {
	return get_node_rtvalue(node, RTValue::REAL,
		[](const std::string& literal) {
			return std::stod(literal);
		}
	);
}

caoco_impl_env_eval_process(CStringEval) {
	// Get the cstr of the node
	auto literal = get_node_cstr(node);

	// Remove the quotes
	literal = literal.substr(1, literal.size() - 2);

	// Check for escape characters.
	// If there are escape characters, we need to replace them with their actual values.
	bool no_escape = false;
	size_t current_pos = 0;
	while (!no_escape) {
		auto escape_pos = literal.find('\\', current_pos);
		if (escape_pos == std::string::npos) {
			no_escape = true;
		}
		else {
			// Replace the escape characters
			switch (literal[escape_pos + 1]) {
			case 'n':
				literal.replace(escape_pos, 2, "\n");
				current_pos = escape_pos + 1;
				break;
			case 't':
				literal.replace(escape_pos, 2, "\t");
				current_pos = escape_pos + 1;
				break;
			case 'r':
				literal.replace(escape_pos, 2, "\r");
				current_pos = escape_pos + 1;
				break;
			case '0':
				literal.replace(escape_pos, 2, "\0");
				current_pos = escape_pos + 1;
				break;
			case '\\':
				literal.replace(escape_pos, 2, "\\");
				current_pos = escape_pos + 1;
				break;
			case '\'':
				literal.replace(escape_pos, 2, "'");
				current_pos = escape_pos + 1;
				break;
			case '\"':
				literal.replace(escape_pos, 2, "\"");
				current_pos = escape_pos + 1;
				break;
			default:
				throw std::runtime_error("CStringEval:Invalid escape character:" + literal);
			}
		}
	
	}
	// node is already a string, extract it and return
	return RTValue(RTValue::STRING, literal);
}
//
caoco_impl_env_eval_process(CBitEval) {
	return get_node_rtvalue(node, RTValue::BIT,
		[](const std::string& literal) {
			if (literal == "1b") {
				return true;
			}
			else if (literal == "0b") {
				return false;
			}
			else {
				throw std::runtime_error("CBitEval:Invalid literal:" + literal);
			}
		}
	);
}

caoco_impl_env_eval_process(CUnsignedEval) {
	return get_node_rtvalue(node, RTValue::UNSIGNED,
		[](const std::string& literal) {
			if (literal.back() != 'u') {
				throw std::runtime_error("CUnsignedEval:Unsigned literal not followed by 'u':" + literal);
			}
			auto unsigned_str = literal.substr(0, literal.size() - 1);
			return stou(unsigned_str);
		}
	);
}

caoco_impl_env_eval_process(COctetEval) {
	return get_node_rtvalue(node, RTValue::OCTET,
		[](const std::string& literal) {
			// literal will be in the form [0-255]c or '[character]'c
			if (literal.back() != 'c') {
				throw std::runtime_error("COctetEval:Octet literal not followed by 'c'" + literal);
			}

			//if the literal is a character
			if (literal[0] == '\'') {
				auto octet_str = literal.substr(1, literal.size() - 3); // remove the quotes and the c
				// Check for escape characters.
				auto escape_pos = octet_str.find('\\');
				if (escape_pos != std::string::npos) {
					// Replace the escape characters
					switch (literal[escape_pos + 1]) {
					case 'n':
						octet_str.replace(escape_pos, 2, "\n");
						break;
					case 't':
						octet_str.replace(escape_pos, 2, "\t");
						break;
					case 'r':
						octet_str.replace(escape_pos, 2, "\r");
						break;
					case '0':
						octet_str.replace(escape_pos, 2, "\0");
						break;
					case '\\':
						octet_str.replace(escape_pos, 2, "\\");
						break;
					case '\'':
						octet_str.replace(escape_pos, 2, "'");
						break;
					case '\"':
						octet_str.replace(escape_pos, 2, "\"");
						break;
					default:
						throw std::runtime_error("COctetEval:Invalid escape character:" + literal);
					}
				}

				if (octet_str.size() != 1) {
					throw std::runtime_error("COctetEval:Invalid character literal:" + literal);
				}

				return static_cast<unsigned char>(octet_str.at(0));
			}
			else { // Has to be a number
				auto octet_str = literal.substr(0, literal.size() - 1);
				return static_cast<unsigned char>(std::stoi(octet_str));
			}
		}
	);
}

caoco_impl_env_eval_process(CNoneEval) {
	return RTValue(RTValue::NONE, none_t{});
};

caoco_impl_env_eval_process(CLiteralEval) {
	auto node_type = node.type();
	switch (node_type) {
	case Node::eType::number_literal_:
		return CNumberEval{}(node, env);
	case Node::eType::real_literal_:
		return CRealEval{}(node, env);
	case Node::eType::string_literal_:
		return CStringEval{}(node, env);
	case Node::eType::bit_literal_:
		return CBitEval{}(node, env);
	case Node::eType::unsigned_literal_:
		return CUnsignedEval{}(node, env);
	case Node::eType::octet_literal_:
		return COctetEval{}(node, env);
	case Node::eType::none_literal_:
		return CNoneEval{}(node, env);
	default:
		throw std::runtime_error("CLiteralEval:Invalid node type:" + std::to_string(static_cast<int>(node_type)));
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
caoco_impl_env_eval_process(CAddOpEval) {
	auto left = node.body().front();
	auto right = node.body().back();

	RTValue left_val;
	RTValue right_val;
	// Left
	if (left.operation() == Node::eOperation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (left.operation() == Node::eOperation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (right.operation() == Node::eOperation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (right.operation() == Node::eOperation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}



	if (left_val.type == RTValue::NUMBER && right_val.type == RTValue::NUMBER) {
		return RTValue{ RTValue::NUMBER, std::get<int>(left_val.value) + std::get<int>(right_val.value) };
	}
	else if (left_val.type == RTValue::REAL && right_val.type == RTValue::REAL) {
		return RTValue{ RTValue::REAL, std::get<double>(left_val.value) + std::get<double>(right_val.value) };
	}
	else if (left_val.type == RTValue::STRING && right_val.type == RTValue::STRING) {
		return RTValue{ RTValue::STRING, std::get<std::string>(left_val.value) + std::get<std::string>(right_val.value) };
	}
	else if (left_val.type == RTValue::BIT && right_val.type == RTValue::BIT) {
		return RTValue{ RTValue::BIT, std::get<bool>(left_val.value) + std::get<bool>(right_val.value) };
	}
	else if (left_val.type == RTValue::OCTET && right_val.type == RTValue::OCTET) {
		return RTValue{ RTValue::OCTET, static_cast<unsigned char>(std::get<unsigned char>(left_val.value) + std::get<unsigned char>(right_val.value)) };
	}
	else if (left_val.type == RTValue::UNSIGNED && right_val.type == RTValue::UNSIGNED) {
		return RTValue{ RTValue::UNSIGNED, std::get<unsigned>(left_val.value) + std::get<unsigned>(right_val.value) };
	}
	else {
		throw std::runtime_error("CAddOpEval:Invalid types for addition, implicit conversion is disabled.");
	}
}

caoco_impl_env_eval_process(CSubOpEval) {
	auto left = node.body().front();
	auto right = node.body().back();

	RTValue left_val;
	RTValue right_val;
	// Left
	if (left.operation() == Node::eOperation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (left.operation() == Node::eOperation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (right.operation() == Node::eOperation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (right.operation() == Node::eOperation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	if (left_val.type == RTValue::NUMBER && right_val.type == RTValue::NUMBER) {
		return RTValue{ RTValue::NUMBER, std::get<int>(left_val.value) - std::get<int>(right_val.value) };
	}
	else if (left_val.type == RTValue::REAL && right_val.type == RTValue::REAL) {
		return RTValue{ RTValue::REAL, std::get<double>(left_val.value) - std::get<double>(right_val.value) };
	}
	else if (left_val.type == RTValue::OCTET && right_val.type == RTValue::OCTET) {
		return RTValue{ RTValue::OCTET, static_cast<unsigned char>(std::get<unsigned char>(left_val.value) - std::get<unsigned char>(right_val.value)) };
	}
	else if (left_val.type == RTValue::UNSIGNED && right_val.type == RTValue::UNSIGNED) {
		return RTValue{ RTValue::UNSIGNED, std::get<unsigned>(left_val.value) - std::get<unsigned>(right_val.value) };
	}
	else {
		throw std::runtime_error("CSubOpEval:Invalid types for subtraction, implicit conversion is disabled.");
	}

}

caoco_impl_env_eval_process(CMultOpEval) {
	auto left = node.body().front();
	auto right = node.body().back();

	RTValue left_val;
	RTValue right_val;
	// Left
	if (left.operation() == Node::eOperation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (left.operation() == Node::eOperation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (right.operation() == Node::eOperation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (right.operation() == Node::eOperation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	if (left_val.type == RTValue::NUMBER && right_val.type == RTValue::NUMBER) {
		return RTValue{ RTValue::NUMBER, std::get<int>(left_val.value) * std::get<int>(right_val.value) };
	}
	else if (left_val.type == RTValue::REAL && right_val.type == RTValue::REAL) {
		return RTValue{ RTValue::REAL, std::get<double>(left_val.value) * std::get<double>(right_val.value) };
	}
	else if (left_val.type == RTValue::OCTET && right_val.type == RTValue::OCTET) {
		return RTValue{ RTValue::OCTET, static_cast<unsigned char>(std::get<unsigned char>(left_val.value) * std::get<unsigned char>(right_val.value)) };
	}
	else if (left_val.type == RTValue::UNSIGNED && right_val.type == RTValue::UNSIGNED) {
		return RTValue{ RTValue::UNSIGNED, std::get<unsigned>(left_val.value) * std::get<unsigned>(right_val.value) };
	}
	else {
		throw std::runtime_error("CMultOpEval:Invalid types for multiplication, implicit conversion is disabled.");
	}
}

caoco_impl_env_eval_process(CDivOpEval) {
	auto left = node.body().front();
	auto right = node.body().back();

	RTValue left_val;
	RTValue right_val;
	// Left
	if (left.operation() == Node::eOperation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (left.operation() == Node::eOperation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (right.operation() == Node::eOperation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (right.operation() == Node::eOperation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	if (left_val.type == RTValue::NUMBER && right_val.type == RTValue::NUMBER) {
		return RTValue{ RTValue::NUMBER, std::get<int>(left_val.value) / std::get<int>(right_val.value) };
	}
	else if (left_val.type == RTValue::REAL && right_val.type == RTValue::REAL) {
		return RTValue{ RTValue::REAL, std::get<double>(left_val.value) / std::get<double>(right_val.value) };
	}
	else if (left_val.type == RTValue::OCTET && right_val.type == RTValue::OCTET) {
		return RTValue{ RTValue::OCTET, static_cast<unsigned char>(std::get<unsigned char>(left_val.value) / std::get<unsigned char>(right_val.value)) };
	}
	else if (left_val.type == RTValue::UNSIGNED && right_val.type == RTValue::UNSIGNED) {
		return RTValue{ RTValue::UNSIGNED, std::get<unsigned>(left_val.value) / std::get<unsigned>(right_val.value) };
	}
	else {
		throw std::runtime_error("CDivOpEval:Invalid types for division, implicit conversion is disabled.");
	}
}

caoco_impl_env_eval_process(CModOpEval) {
	auto left = node.body().front();
	auto right = node.body().back();

	RTValue left_val;
	RTValue right_val;
	// Left
	if (left.operation() == Node::eOperation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (left.operation() == Node::eOperation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (right.operation() == Node::eOperation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (right.operation() == Node::eOperation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	if (left_val.type == RTValue::NUMBER && right_val.type == RTValue::NUMBER) {
		return RTValue{ RTValue::NUMBER, std::get<int>(left_val.value) % std::get<int>(right_val.value) };
	}
	else if (left_val.type == RTValue::OCTET && right_val.type == RTValue::OCTET) {
		return RTValue{ RTValue::OCTET, static_cast<unsigned char>(std::get<unsigned char>(left_val.value) % std::get<unsigned char>(right_val.value)) };
	}
	else if (left_val.type == RTValue::UNSIGNED && right_val.type == RTValue::UNSIGNED) {
		return RTValue{ RTValue::UNSIGNED, std::get<unsigned>(left_val.value) % std::get<unsigned>(right_val.value) };
	}
	else {
		throw std::runtime_error("CModOpEval:Invalid types for modulo, implicit conversion is disabled.");
	}
}

caoco_impl_env_eval_process(CBinopEval) {
	// If lhs node is binop,process it.
	auto left = node.body().front();
	auto right = node.body().back();
	RTValue left_val;
	RTValue right_val;
	// Left
	if(left.operation() == Node::eOperation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (left.operation() == Node::eOperation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (right.operation() == Node::eOperation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (right.operation() == Node::eOperation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Get the type of the operation
	auto op = node.type();
	switch (op) {
	case Node::eType::addition_:
		return CAddOpEval{}(node, env);
	case Node::eType::subtraction_:
		return CSubOpEval{}(node, env);
	case Node::eType::multiplication_:
	return CMultOpEval{}(node, env);
	case Node::eType::division_:
	return CDivOpEval{}(node, env);
	case Node::eType::remainder_:
	return CModOpEval{}(node, env);
	default:
		throw "NOT IMPLEMENTED";
	}

}
}; // namespace caoco