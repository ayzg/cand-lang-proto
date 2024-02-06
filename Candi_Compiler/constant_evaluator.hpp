#include "ast_node.hpp"
#include "parser.hpp"
#include <variant>
#include <list>
#include <map>

namespace caoco {

struct none_t {};

struct RTValue {
	enum eType {
		NUMBER = 0,
		REAL = 1,
		STRING = 2,
		BIT = 3,
		BYTE = 4,
		NONE = 5
	} type;

	std::variant<int, double, std::string, bool, char, none_t> value;

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


struct CEvalProcess {
	virtual RTValue eval(const Node & node) = 0;
	virtual ~CEvalProcess() = default;
public:
	RTValue operator()(const Node & node) {
		return eval(node);
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

#define caoco_def_eval_process(n) struct n : public CEvalProcess{\
	RTValue eval(const Node & node) override; \
};

#define caoco_impl_eval_process(n) RTValue n::eval(const Node & node)

#define caoco_def_env_eval_process(n) struct n : public EnvEvalProcess{\
	RTValue eval(const Node & node, rtenv& env) override; \
};

#define caoco_impl_env_eval_process(n) RTValue n::eval(const Node & node, rtenv& env)

// All tokens in the AST hold caoco::string_t, whitch is a utf-8 encoded string
// TODO: Move get_node_cstr into the Node class
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
caoco_def_eval_process(CNumberEval); // 1234
caoco_def_eval_process(CRealEval); // 1234.1234
caoco_def_eval_process(CStringEval); // 'Hello World'
caoco_def_eval_process(CBitEval); // 1b or 0b
caoco_def_eval_process(CByteEval); // 0B-255B
caoco_def_eval_process(CNoneEval); // #none directive
caoco_def_eval_process(CLiteralEval); // Dispatches to the above evaluators based on node type.

// Evironment-Sensitive Evaluators
caoco_def_env_eval_process(CIdentifierEval);
caoco_def_env_eval_process(COperationEval);
caoco_def_env_eval_process(CExpressionEval);

//----------------------------------------------------------------------------------------------------------------------------------------------------------//
// Constant Evaluator Processes Implementations
//----------------------------------------------------------------------------------------------------------------------------------------------------------//
/// !!!!!!!!!!!!! TODO: 
//			- Implement bit literals in tokenizer,parser 0b,1b
//			- Implement byte literals in tokenizer,parser 0B-255B also '_'B where _ is one of 255 chars
//			- Implement #node directive in tokenizer,parser #none
caoco_impl_eval_process(CNumberEval) {
	return get_node_rtvalue(node, RTValue::NUMBER,
		[](const std::string& literal) {
			return std::stoi(literal);
		}
	);
}

caoco_impl_eval_process(CRealEval) {
	return get_node_rtvalue(node, RTValue::REAL,
		[](const std::string& literal) {
			return std::stod(literal);
		}
	);
}

caoco_impl_eval_process(CStringEval) {
	// node is already a string, extract it and return
	return RTValue(RTValue::STRING, get_node_cstr(node));
}

caoco_impl_eval_process(CBitEval) {
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

caoco_impl_eval_process(CByteEval) {
	return get_node_rtvalue(node, RTValue::BYTE,
		[](const std::string& literal) {
		    // literal will be in the form [0-255]B
			if (literal.size() < 2) {
				throw std::runtime_error("byte literal invalid format" + literal);
			}
			if (literal.back() != 'B') {
				throw std::runtime_error("byte literal not followed by 'B'" + literal);
			}
			auto byte_str = literal.substr(0, literal.size() - 1);
			return static_cast<char>(std::stoi(byte_str));
		}
	);
}

caoco_impl_eval_process(CNoneEval) {
	return RTValue(RTValue::NONE, none_t{});
};

caoco_impl_env_eval_process(CIdentifierEval) {
	auto id = get_node_cstr(node);
	auto resolved_var = env.resolve_variable(id);
	if (resolved_var.valid()) {
		return resolved_var.value();
	}
	else {
		throw std::runtime_error("CIdentifierEval:Variable not found:" + id);
	}
}

caoco_impl_env_eval_process(COperationEval) {
	switch (node.type()) {
	case Node::eType::addition_:

	default:
		return make_rtval_none();
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------//



}; // namespace caoco