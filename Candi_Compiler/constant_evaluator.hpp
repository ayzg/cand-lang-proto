#include "ast_node.hpp"
#include "parser.hpp"
#include <functional>
#include <variant>
#include <list>
#include <map>
#include <memory>

namespace caoco {
class object_t;
class function_t;
struct RTValue;
class rtenv;
struct CBinopEval;
struct none_t {
	bool operator==(const none_t&) const {
		return true;
	}
};

struct undefined_t {
	bool operator==(const undefined_t&) const {
		return true;
	}
};

// string to unsigned int
unsigned stou(sl_string const& str, size_t* idx = 0, int base = 10) {
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
		OBJECT = 7,
		FUNCTION = 8
	} type;

	std::variant<int, double, sl_string, bool, unsigned char, none_t,unsigned, std::shared_ptr<object_t>,std::shared_ptr<function_t>> value;

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
	//using string_t = sl_string;
	using variable_map_t = std::map<sl_string, RTValue>;
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

		const sl_string& name() {
			return it->first;
		}
		RTValue& value() {
			return it->second;
		}
	};
	struct local_const_variable_process_result {
		variable_map_const_iterator_t it;
		bool valid;

		const sl_string& name() const {
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

		const sl_string& name() {
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

		const sl_string& name() const {
			return local.name();
		}

		const RTValue& value() const {
			return local.value();
		}

		bool valid() const {
			return local.valid;
		}
	};

	sl_string name_{};
	rtenv* parent_{nullptr};
	std::list<rtenv> children_{};
	variable_map_t variables_{};


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
	
	variable_map_const_iterator_t find_variable(const sl_string& name) const {
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
	rtenv(const sl_string& name) : name_(name) {}
	rtenv(const sl_string& name, rtenv& parent) : name_(name), parent_(&parent) {}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Local Environment Operations
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	
	// <method:add_subenv> Add a sub environment to the current environment
	rtenv& add_subenv(const sl_string& name) {
		children_.emplace_back(name, *this);
		return children_.back();
	}

	// <@method:create_variable> Create a variable in the current environment
	local_variable_process_result create_variable(const sl_string& name, const RTValue& value) {
		// local vars will shadow parent vars
		auto inserted = variables_.insert({ name, value });
		return { inserted.first, inserted.second };
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------//
	// Environment Sensitive Operations
	//----------------------------------------------------------------------------------------------------------------------------------------------------------//

	// <@method:resolve_variable> Find the variable in the current environment or in the parent environment(if exists)
	env_const_variable_process_result resolve_variable(const sl_string& name) const {
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
	bool delete_variable(const sl_string & name) {
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
	env_variable_process_result get_variable(const sl_string& name) {
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
	env_variable_process_result set_variable(const sl_string& name, const RTValue& value) {
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

// The C& class object type.
class object_t {
	sl_string name_;
	rtenv& scope_;

	std::map<sl_string, RTValue> members_;
public:
	object_t(const sl_string& name,rtenv& scope) : name_(name), scope_(scope) {}

	object_t(const object_t& other) : scope_(other.scope_) {
		name_ = other.name_;
		members_ = other.members_;
		scope_ = other.scope_;
	}

	object_t(object_t&& other) : scope_(other.scope_) {
		name_ = std::move(other.name_);
		members_ = std::move(other.members_);
		scope_ = std::move(other.scope_);
	}

	object_t& operator=(const object_t& other) {
		name_ = other.name_;
		members_ = other.members_;
		scope_ = other.scope_;
		return *this;
	}

	object_t& operator=(object_t&& other) {
		name_ = std::move(other.name_);
		members_ = std::move(other.members_);
		scope_ = std::move(other.scope_);
		return *this;
	}

	RTValue& get_member(sl_string name) {
		return scope_.get_variable(name).value();
	};

	//auto& add_member(const sl_string& name, const RTValue& value) {
	//	auto inserted = members_.insert({ name, value });
	//	return inserted.first;
	//}

	const auto& name() const {
		return name_;
	}

	auto& scope() {
		return scope_;
	}

	auto& set_scope(rtenv& scope) {
		scope_ = scope;
		return scope;
	}
};

class function_t {
	sl_string name_;
	rtenv& scope_;
	sl_vector<sl_string> args_;
	astnode body_;

public:
	function_t(const sl_string& name, rtenv& scope, const sl_vector<sl_string>& args, const astnode& body) 
		: name_(name), scope_(scope), args_(args), body_(body) {}

	function_t(const function_t& other) : scope_(other.scope_) {
		name_ = other.name_;
		args_ = other.args_;
		body_ = other.body_;
		scope_ = other.scope_;
	}

	function_t(function_t&& other) : scope_(other.scope_) {
		name_ = std::move(other.name_);
		args_ = std::move(other.args_);
		body_ = std::move(other.body_);
		scope_ = std::move(other.scope_);
	}

	function_t& operator=(const function_t& other) {
		name_ = other.name_;
		args_ = other.args_;
		body_ = other.body_;
		scope_ = other.scope_;
		return *this;
	}

	function_t& operator=(function_t&& other) {
		name_ = std::move(other.name_);
		args_ = std::move(other.args_);
		body_ = std::move(other.body_);
		scope_ = std::move(other.scope_);
		return *this;
	}

	const auto& name() const {
		return name_;
	}

	auto& scope() {
		return scope_;
	}

	auto& set_scope(rtenv& scope) {
		scope_ = scope;
		return scope;
	}

	const auto& args() const {
		return args_;
	}

	const auto& body() const {
		return body_;
	}
};
struct EnvEvalProcess {
	virtual RTValue eval(const astnode & node, rtenv& env) = 0;
	virtual ~EnvEvalProcess() = default;
public:
	RTValue operator()(const astnode& node, rtenv& env) {
		return eval(node, env);
	};
};

#define caoco_def_env_eval_process(n) struct n : public EnvEvalProcess{\
	RTValue eval(const caoco::astnode & node, caoco::rtenv& env) override; \
};

#define caoco_impl_env_eval_process(n) RTValue n::eval(const caoco::astnode & node, caoco::rtenv& env)

//// All tokens in the AST hold caoco::sl_u8string, whitch is a utf-8 encoded string
//inline auto get_node_cstr(const astnode & node) {
//	return node.literal_str();
//}

// Util method for single node evaluators which use the same pattern.
template<typename RetrieverT> requires std::invocable<RetrieverT, const sl_string&>
inline RTValue get_node_rtvalue(const astnode& node, RTValue::eType rtval_type,RetrieverT && retrival_policy) {
	auto literal = node.literal_str();
	try {
		return RTValue{ rtval_type, retrival_policy(literal) };
	}
	catch (std::exception e) {
		sl_string msg = e.what();
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
caoco_def_env_eval_process(CVariableEval);
caoco_def_env_eval_process(CLiteralEval); // Dispatches to the above evaluators based on node type.


caoco_def_env_eval_process(CAddOpEval); // +
caoco_def_env_eval_process(CSubOpEval); // -
caoco_def_env_eval_process(CMultOpEval); // *
caoco_def_env_eval_process(CDivOpEval); // /
caoco_def_env_eval_process(CModOpEval); // %
caoco_def_env_eval_process(CBinopEval); // Dispatches to binary ops


caoco_def_env_eval_process(CVarDeclEval); // anon var decl <#var><alnumus><=><expression><;> (for now)
caoco_def_env_eval_process(CClassDeclEval); // class decl <#class><alnumus><{><...><}>
caoco_def_env_eval_process(CFunctionDeclEval); // function decl <#function><alnumus><{><...><}>
caoco_def_env_eval_process(CFunctionCallEval);
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Constant Evaluator Processes Implementations
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
caoco_impl_env_eval_process(CNumberEval) {
	return get_node_rtvalue(node, RTValue::NUMBER,
		[](const sl_string& literal) {
			return std::stoi(literal);
		}
	);
}

caoco_impl_env_eval_process(CRealEval) {
	return get_node_rtvalue(node, RTValue::REAL,
		[](const sl_string& literal) {
			return std::stod(literal);
		}
	);
}

caoco_impl_env_eval_process(CStringEval) {
	// Get the cstr of the node
	auto literal = node.literal_str();

	// Remove the quotes
	literal = literal.substr(1, literal.size() - 2);

	// Check for escape characters.
	// If there are escape characters, we need to replace them with their actual values.
	bool no_escape = false;
	size_t current_pos = 0;
	while (!no_escape) {
		auto escape_pos = literal.find('\\', current_pos);
		if (escape_pos == sl_string::npos) {
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
		[](const sl_string& literal) {
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
		[](const sl_string& literal) {
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
		[](const sl_string& literal) {
			// literal will be in the form [0-255]c or '[character]'c
			if (literal.back() != 'c') {
				throw std::runtime_error("COctetEval:Octet literal not followed by 'c'" + literal);
			}

			//if the literal is a character
			if (literal[0] == '\'') {
				auto octet_str = literal.substr(1, literal.size() - 3); // remove the quotes and the c
				// Check for escape characters.
				auto escape_pos = octet_str.find('\\');
				if (escape_pos != sl_string::npos) {
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

caoco_impl_env_eval_process(CVariableEval) {
	auto var_name = node.literal_str();
	auto resolved_var = env.resolve_variable(var_name);
	if (resolved_var.valid()) {
		return resolved_var.value();
	}
	else {
		throw std::runtime_error("CVariableEval:Variable not found:" + var_name);
	}
}

caoco_impl_env_eval_process(CLiteralEval) {
	auto node_type = node.type();
	switch (node_type) {
	case astnode_enum::number_literal_:
		return CNumberEval{}(node, env);
	case astnode_enum::real_literal_:
		return CRealEval{}(node, env);
	case astnode_enum::string_literal_:
		return CStringEval{}(node, env);
	case astnode_enum::bit_literal_:
		return CBitEval{}(node, env);
	case astnode_enum::unsigned_literal_:
		return CUnsignedEval{}(node, env);
	case astnode_enum::octet_literal_:
		return COctetEval{}(node, env);
	case astnode_enum::none_literal_:
		return CNoneEval{}(node, env);
	case astnode_enum::alnumus_:
		return CVariableEval{}(node, env);
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
	if (get_node_operation(left.type()) == Operation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (get_node_operation(left.type()) == Operation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (get_node_operation(right.type()) == Operation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (get_node_operation(right.type()) == Operation::none_) {
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
		return RTValue{ RTValue::STRING, std::get<sl_string>(left_val.value) + std::get<sl_string>(right_val.value) };
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
	if (get_node_operation(left.type()) == Operation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (get_node_operation(left.type()) == Operation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (get_node_operation(right.type()) == Operation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (get_node_operation(right.type()) == Operation::none_) {
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
	if (get_node_operation(left.type()) == Operation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (get_node_operation(left.type()) == Operation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (get_node_operation(right.type()) == Operation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (get_node_operation(right.type()) == Operation::none_) {
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
	if (get_node_operation(left.type()) == Operation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (get_node_operation(left.type()) == Operation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (get_node_operation(right.type()) == Operation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (get_node_operation(right.type()) == Operation::none_) {
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
	if (get_node_operation(left.type()) == Operation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (get_node_operation(left.type()) == Operation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (get_node_operation(right.type()) == Operation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (get_node_operation(right.type()) == Operation::none_) {
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
	if(node.body().size()==0) { // If the node has no child, it is a literal
		return CLiteralEval{}(node, env);
	}
	// If lhs node is binop,process it.
	auto left = node.body().front();
	auto right = node.body().back();
	RTValue left_val;
	RTValue right_val;
	// Left
	if(get_node_operation(left.type()) == Operation::binary_) {
		left_val = CBinopEval{}(left, env);
	}
	else if (get_node_operation(left.type()) == Operation::none_) {
		left_val = CLiteralEval{}(left, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Right
	if (get_node_operation(right.type()) == Operation::binary_) {
		right_val = CBinopEval{}(right, env);
	}
	else if (get_node_operation(right.type()) == Operation::none_) {
		right_val = CLiteralEval{}(right, env);
	}
	else {
		throw "NOT IMPLEMENTED";
	}

	// Get the type of the operation
	auto op = node.type();
	switch (op) {
	case astnode_enum::addition_:
		return CAddOpEval{}(node, env);
	case astnode_enum::subtraction_:
		return CSubOpEval{}(node, env);
	case astnode_enum::multiplication_:
	return CMultOpEval{}(node, env);
	case astnode_enum::division_:
	return CDivOpEval{}(node, env);
	case astnode_enum::remainder_:
	return CModOpEval{}(node, env);
	default:
		throw "NOT IMPLEMENTED";
	}

}

caoco_impl_env_eval_process(CVarDeclEval) {
	auto var_name = node.body().front().literal_str();

	// Check if the variable has been declared
	if(env.resolve_variable(var_name).valid()) {
		throw std::runtime_error("CAssignOpEval:Variable already declared:" + var_name);
	}

	// Format of the node is <var name>,<expression> where expression is one of the assingment operators.(for now only simple assingment)
	// lhs of the assingment operator is the variable name, thats why we use node.body().back().body().back() to get the rhs.
	//auto what = node.body().back().body().back();
	auto new_value = CBinopEval()(node.body().back().body().back(), env);

	// Create the variable

	auto created_var = env.create_variable(var_name, new_value);

	return created_var.value(); // return ref to the created variable
}

caoco_impl_env_eval_process(CClassDeclEval) {
	/* Format of incoming node:
		<class_definition>
			-> <alnumus> The name of the class
			-> <pragmatic_block_> The body of the class
				-> ...statements...
	*/
	auto class_name = node.body().front().literal_str();
	auto new_class = std::make_shared<object_t>(object_t(class_name, env.add_subenv(class_name)));
	auto created_class = env.create_variable(class_name, RTValue(RTValue::OBJECT, new_class));

	// Process the class body
	for (auto& statement : node.body().back().body()) {
		// only anonvardeclfor now
		CVarDeclEval{}(statement, new_class->scope());
	}

	return created_class.value(); // return ref to the created class
}

caoco_impl_env_eval_process(CFunctionDeclEval) {
	/* Format of incoming node:
		<function_definition>
			-> <alnumus> The name of the function
			-> <arguments?> The arguments of the function
			-> <functional_block_> The body of the function
				-> ...statements...
	*/
	auto function_name = node.body().front().literal_str();
	auto arguments = [&node]() {
		sl_vector<sl_string> args;
		for (auto& arg : (*(node.body().begin()++)).body()) {
			args.push_back(arg.literal_str());
		}
		return args;
	}();
	auto new_function = std::make_shared<function_t>(function_t(function_name, env.add_subenv(function_name), arguments, node.body().back()));
	auto created_function = env.create_variable(function_name, RTValue(RTValue::FUNCTION, new_function));

	return created_function.value(); // return ref to the created function
};

caoco_impl_env_eval_process(CFunctionCallEval) {
	// front will be the function name, back will be the arguments
	auto function_name = node.body().front().literal_str();

	// Get the function from the env
	auto resolved_function = env.resolve_variable(function_name);

	// Check if the function exists
	if(!resolved_function.valid()) {
		throw std::runtime_error("CFunctionCallEval:Function not found:" + function_name);
	}

	// Bind the arguments to the function
	auto& function = std::get<std::shared_ptr<function_t>>(resolved_function.value().value);
	function->scope().create_variable(function->args().front(),CBinopEval()(node.body().back(), env));

	// Evaluate the function body(for now only 1 return statement)
	auto result = CBinopEval()(function->body().body().front().body().back(), function->scope());

	// destroy the variables created in the function scope
	for (auto& arg : function->args()) {
		function->scope().delete_variable(arg);
	}

	return result;
}

}; // namespace caoco