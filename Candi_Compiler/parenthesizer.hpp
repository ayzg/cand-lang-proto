#pragma once
#include "cand_syntax.hpp"
#include "token_iterator.hpp"

/// <Expression Rules>
/// 1. Access Operators . :: may only be followed by an identifier. Not a prefix!
/// </Expression Rules>
class closure;
using tk_list = sl_list<tk>;
using tk_list_it = tk_list::iterator;
using closure_list_it = sl_list<closure>::iterator;

ast parse_expression_impl(tk_vector_cit begin, tk_vector_cit end);
ast parse_expression(tk_vector_cit begin, tk_vector_cit end);

// A closure is a range of tokens representing a singular ast node.
// When a closure is a single token, the front and back are the same.
// Example: 1 + 2 -> Closure 2 is +, .+ is the front and +. is the back.
// Example: 1(abcd) + 2 -> Closure 2 is (abcd), .(abcd) is the front and (abcd). is the back.
class closure {
	tk_list_it front_;
	tk_list_it back_;
public:
	closure(tk_list_it front) : front_(front), back_(front) {}
	closure(tk_list_it front, tk_list_it back) : front_(front), back_(back) {}

	e_operation operation() const { 
		// Special case for (, [, {, and postfix operators.
		if(front_->type_is(e_tk::open_paren_) || front_->type_is(e_tk::open_brace_) || front_->type_is(e_tk::open_bracket_)) {
			return e_operation::postfix_;
		}
		else {
			return front_->operation();
		}
	}
	priority::e_priority priority() const { 
		// Special case for (, [, {, are postfix priority.
		if (front_->type_is(e_tk::open_paren_) || front_->type_is(e_tk::open_brace_) || front_->type_is(e_tk::open_bracket_)) {
			return priority::e_priority::postfix_;
		}
		else {
			return front_->priority();
		}
	}
	e_assoc assoc() const { return front_->assoc(); }
	tk_list_it front() const { return front_; }
	tk_list_it back() const { return back_; }
};

class closure_list {
	tk_list& output;
	//tk_list binary_dump_;
	sl_list<closure> closures_;
	closure_list_it find_backward(std::function<bool(closure&)>&& condition) {
		return std::find_if(closures_.rbegin(), closures_.rend(), condition).base();
	}
	sl_vector<closure_list_it> find_backward_consecutive(
		std::function<bool(closure_list_it, closure_list_it)>&& condition) {
		auto it = closures_.rbegin();
		auto end = closures_.rend();
		sl_vector<closure_list_it> accumulated_iterators;
		while (it != end) {
			auto next = std::next(it);
			if (condition(std::prev(it.base()), std::prev(next.base()))) {
				accumulated_iterators.push_back(std::prev(it.base()));
			}
			else {
				accumulated_iterators.push_back(std::prev(it.base()));
				break;
			}
			++it;
		}
		return accumulated_iterators;
	}
	sl_vector<closure_list_it> find_backward_consecutive_and_ignore(
		std::function<bool(closure_list_it, closure_list_it)>&& condition, std::function<bool(closure_list_it)>&& ignore_condition) {
		auto it = closures_.rbegin();
		auto end = closures_.rend();
		sl_vector<closure_list_it> accumulated_iterators;
		while (it != end) {
			auto next = std::next(it);
			if(ignore_condition(next.base())) {
				it++;
				continue;
			}
			else {
				if (condition(std::prev(it.base()), std::prev(next.base()))) {
					accumulated_iterators.push_back(std::prev(it.base()));
					it++;
				}
				else {
					accumulated_iterators.push_back(std::prev(it.base()));
					break;
				}
			}
		}
		return accumulated_iterators;
	}

	void output_insert_tk_before_closure(closure_list_it closure_it, tk token) {
		auto front = closure_it->front();
		output.insert(front, token);
	}
	void output_insert_tk_after_closure(closure_list_it closure_it, tk token) {
		auto back = closure_it->back();
		auto next = std::next(back);
		if (next == output.end()) {
			output.push_back(token);
		}
		else {
			output.insert(next, token);
		}
	}
public:
	closure_list(tk_list& output) : output(output) {}
public:
	void push_front(tk_list_it front) {
		closures_.push_front(closure(front));
	}
	void push_front(tk_list_it front, tk_list_it back) {
		closures_.push_front(closure(front, back));
	}

	void push_back(tk_list_it front) {
		closures_.push_back(closure(front));
	}
	void push_back(tk_list_it front, tk_list_it back) {
		closures_.push_back(closure(front, back));
	}
	void pop() {
		closures_.pop_back();
	}

	void pop_if(auto&& condition) {
		closures_.remove_if(std::move(condition));
	}

	auto size() const {
		return closures_.size();
	}	
	auto last_closure() {
		return std::prev(closures_.end());
	}
	tk& last_closure_front_tk() {
		return *closures_.back().front();
	}
	tk& last_closure_back_tk() {
		return *closures_.back().back();
	}

	// Upon resolution all closures of higher priority than the head are resolved.
	// When the head is at the end, the sentinel token is the head- which has the lowest priority.
	// After a resolution or series of resolutions, a closure store and head advance should be performed.
	
	// Resolves the last closure based on type-unchecked.
	void resolve_next() {
		if (last_closure_front_tk().operation()== e_operation::prefix_) {
			resolve_prefix();
		}
		else if (last_closure_front_tk().operation() == e_operation::postfix_ 
			|| last_closure_front_tk().type_is(e_tk::open_paren_)
			|| last_closure_front_tk().type_is(e_tk::open_brace_)
			|| last_closure_front_tk().type_is(e_tk::open_bracket_)) { // because ( [ { are postfix operators.
			resolve_postfix();
		}
		else if (last_closure_front_tk().operation() == e_operation::binary_) {
			resolve_binary();
		}
		else {
			throw "Invalid token type in closure.";
		}
	}

	void resolve_postfix() {
		// Find concecutive postfix closures.
		auto nclosures = find_backward_consecutive(
			[](closure_list_it a, closure_list_it b) {
				return a->priority() == b->priority();
			}).size();

		auto closure_count = nclosures;
		while(nclosures > 0) {
			output_insert_tk_after_closure(std::prev(last_closure(), static_cast<std::ptrdiff_t>(closure_count)), tk{ e_tk::open_paren_ ,u8"("});
			output_insert_tk_after_closure(std::prev(last_closure(), static_cast<std::ptrdiff_t>(nclosures -1)), tk{ e_tk::close_paren_ ,u8")"});
			nclosures--;
		}

		for (sl_size i = 0; i < closure_count; i++) {
			pop();
		}
	}

	void resolve_binary_left_associative() {
		auto binary_closures = find_backward_consecutive_and_ignore(
			[](closure_list_it a, closure_list_it b) {
				return a->priority() == b->priority();
			},
			[](closure_list_it a) {
				return ((a->priority() == priority::e_priority::postfix_) || (a->priority() == priority::e_priority::prefix_));
			}
			);
		priority::e_priority association_priority = binary_closures.back()->priority();
		// Resolve last
		auto open_paren_it = std::prev(binary_closures.back());
		output_insert_tk_after_closure(open_paren_it, tk{ e_tk::open_paren_ ,u8"("});
		output.push_back(tk{ e_tk::close_paren_ ,u8")"});

		auto close_paren_it = binary_closures.begin();
		// skip last in loop.
		while (close_paren_it != std::prev(binary_closures.end())) {
			output_insert_tk_after_closure(open_paren_it, tk{ e_tk::open_paren_ ,u8"("});
			output_insert_tk_before_closure(*close_paren_it, tk{ e_tk::close_paren_ ,u8")"});
			close_paren_it = std::next(close_paren_it);
		}

		// Pop all binary closures of the same priority.
		pop_if([&association_priority](closure& a) {
			return a.priority() == association_priority;
		});
		//for (auto i = 0; i < binary_closures.size(); i++) {
		//	pop();
		//}
	}

	void resolve_binary_right_associative() {
		auto binary_closures = find_backward_consecutive_and_ignore(
			[](closure_list_it a, closure_list_it b) {
			return a->priority() == b->priority();
		},
			[](closure_list_it a) {
			return ((a->priority() == priority::e_priority::postfix_) || (a->priority() == priority::e_priority::prefix_));
		}
		);
		priority::e_priority association_priority = binary_closures.back()->priority();
		// Resolve last
		output_insert_tk_after_closure(std::prev(binary_closures.back()), tk{ e_tk::open_paren_ ,u8"("});
		output.push_back(tk{ e_tk::close_paren_ ,u8")"});

		auto open_paren_it = std::next(binary_closures.begin());
		while (open_paren_it != binary_closures.end()) {
			output_insert_tk_after_closure(*open_paren_it, tk{ e_tk::open_paren_ ,u8"("});
			output.push_back(tk{ e_tk::close_paren_ ,u8")"});
			open_paren_it = std::next(open_paren_it);
		}

		// Pop all binary closures of the same priority.
		pop_if([&association_priority](closure& a) {
			return a.priority() == association_priority;
		});
		//for (auto i = 0; i < binary_closures.size(); i++) {
		//	pop();
		//}
	}

	void resolve_binary() {
		assert(closures_.size() >= 2 && closures_.front().front()->type_is(e_tk::none_)); // Sentinel begin closure must exist.

		// Resolve associative binary if there are repeated equivalent priority closures before the last.
		if (std::prev(last_closure())->priority() == last_closure_front_tk().priority()) {
			if (last_closure_front_tk().assoc() == e_assoc::left_) {
				resolve_binary_left_associative();
			}
			else if (last_closure_front_tk().assoc() == e_assoc::right_) {
				resolve_binary_right_associative();
			}
		}
		else { // Resolve Single Binary Closure
			// If there are postfix closures-> skip them when looking for the open paren location.
			auto postfix_it = std::prev(last_closure());
			while(postfix_it->priority() == priority::e_priority::postfix_) {
				postfix_it = std::prev(postfix_it);
			}

			auto prefix_it = postfix_it;
			if(postfix_it->priority() == priority::e_priority::prefix_) {
				while(prefix_it->priority() == priority::e_priority::prefix_) {
					prefix_it = std::prev(prefix_it);
				}
			}

			output_insert_tk_after_closure(prefix_it, tk{ e_tk::open_paren_ ,u8"("});
			output.push_back(tk{ e_tk::close_paren_ ,u8")"});
			pop();
		}
	}

	void resolve_prefix() {
		assert(closures_.size() >= 2 && closures_.front().front()->type_is(e_tk::none_)); // Sentinel begin closure must exist.
		output_insert_tk_before_closure(last_closure(), tk{ e_tk::open_paren_ ,u8"("});
		//binary_dump_.push_back(tk{ e_tk::close_paren_ ,u8")" });
		output.push_back(tk{ e_tk::close_paren_ ,u8")"});
		pop();
	}

};

class parenthesizer
{
	enum class e_expected_head { operative_, operator_ };
	std::vector<tk>::const_iterator head_; // [Head]
	std::vector<tk>::const_iterator end_;
	priority::e_priority last_bin_{ priority::e_priority::max_};
	tk_list output_{ e_tk::none_ }; // [Output]
	closure_list closures_{ output_ }; // [Closures]  
	tk sentinel_end_{ e_tk::none_ }; // [Sentinel End] When head is at the end, sentinel token is the head.
	e_expected_head expected_head = e_expected_head::operative_; // Expected type of head on next iteration. Always an operative to begin.
	bool first_pass = true; // Start has to be set after first insertion or else begin is the end.
	bool finished = false; // If the expression has been fully parenthesized.
	void first_pass_switch() {
		if (first_pass) {
			first_pass = false;
		}
	}
public:
	parenthesizer(std::vector<tk>::const_iterator beg, std::vector<tk>::const_iterator end) {
		head_ = beg;
		end_ = end;
		closures_.push_front(output_.begin()); // First closure is the sentinel begin created on the first pass. Do not pop.
	}
	std::vector<tk>::const_iterator head() {
		return head_;
	}
	std::vector<tk>::const_iterator next_head() {
		if(head_ != end_) return std::next(head_);
		else return end_;
	}
	std::vector<tk>::const_iterator buffer_end() {
		return end_;
	}

	std::vector<tk> parenthesized() {
		// Convert list to vector.
		output_.pop_front(); // remove none_ token.
		std::vector<tk> parenthesized_vector;
		parenthesized_vector.reserve(output_.size());
		for (auto& tk : output_) { parenthesized_vector.push_back(tk); }
		output_.push_front(e_tk::none_); // Reset output.
		return parenthesized_vector;
	}

	auto last_pushed_tk() {
		return std::prev(output_.end());
	}

	/// Push head to output, advance head.
	void action_skip(std::vector<tk>::const_iterator end) {
		// Check if this is a single token operator.
		if (head_->operation() == e_operation::none_) {
			output_.push_back(*head_);
			std::advance(head_, 1);
		}
		else {
			if(head_->type_is(e_tk::open_paren_)) {
				auto scope = tk_scope::find_paren(head_, end);
				if (!scope.valid()) {
					throw "Mismatched parentheses in operand.";
				}
				else {
					// Parenthesize the inside of the parentheses.
					auto parenthesized = parenthesizer(scope.contained_begin(), scope.contained_end()).parenthesize_expression();
					for (auto& tk : parenthesized) { output_.push_back(tk); }
					for (auto i = head_; i != end; i++) {
						std::advance(head_, 1);
					}
				}
			}
		}

		//for (auto i = head_; i != end; i++) {
		//	output_.push_back(*head_);
		//	std::advance(head_, 1);
		//}
	}

	/// Push head to output, add new closure from head, advance head.
	void action_store(std::vector<tk>::const_iterator end) {
		// Store first
		output_.push_back(*head_);
		auto start = std::prev(output_.end());
		std::advance(head_, 1);

		// Multi-token operator, store the rest.
		for (auto i = head_; i != end; i++) {
			output_.push_back(*head_);
			std::advance(head_, 1);
		}

		// Add closure
		closures_.push_back(start, std::prev(output_.end()));
	}

	/// Based on priority, resolve all higher priorty closures- then store or store new closure.
	/// If Head >= Last Closure -> Store
	/// If Head < Last Closure -> Resolve All Higher then Store
	void action_check(std::vector<tk>::const_iterator end) {
		if (head_ == end_) { // Resolve all the end.
			while (closures_.size() > 1) {
				closures_.resolve_next();
			}
		}
		else {
			if (head_->priority() >= closures_.last_closure()->priority()) {
				//If: (Head >= Last Closure) => Store
				action_store(end);
			}
			else {
				// While:( Head < Last Closure ) => Resolve Next
				while (head_->priority() < closures_.last_closure()->priority()) {
					closures_.resolve_next();
				}
				action_store(end); // Store head after resolutions.
			}
		}
	}

	// Based on the expected head, choose an action.
	void choose_action() {
		// Action will be skip , store, or check.
		// On first occurence of an operator- a store will be performed. All following operators will be checks.
		// A skip is performed when an operand is encountered. Multi-token operands trigger repeated skips.
		// A check is performed when an operator is encountered.
		// At the end, always do a check.
		if (head_ == end_) {
			action_check(next_head());
			finished = true;
		}
		else {
			if (expected_head == e_expected_head::operative_) {
				// Looking for an operand or prefix operator.Open scope(subexpr).Open Brace (generic list).
				// Open Paren -> Skip Entire Scope.
				if (head_->type_is(e_tk::open_paren_)) {
					auto scope = tk_scope::find_paren(head_, end_);
					if (!scope.valid()) {
						throw "Mismatched parentheses in operand.";
					}
					else {
						action_skip(scope.end());
						expected_head = e_expected_head::operator_;
					}
				}
				// Open Brace -> Skip Entire Scope.
				else if (head_->type_is(e_tk::open_brace_)) {
					auto scope = tk_scope::find_brace(head_, end_);
					if (!scope.valid()) {
						throw "Mismatched braces in operand.";
					}
					else {
						action_skip(scope.end());
						expected_head = e_expected_head::operator_;
					}
				}
				// Prefix Operator -> Check Single Prefix Operator.
				else if (head_->operation() == e_operation::prefix_) {
					if (first_pass) action_store(next_head());
					else action_check(next_head());
					first_pass_switch(); // Switch on first operator.
					expected_head = e_expected_head::operative_;
				}
				// Binary or Postfix -> Error, no operand for operator.
				else if (head_->operation() == e_operation::binary_ || head_->operation() == e_operation::postfix_) {
					// TODO: account for unary - which may be a prefix or binary operator.
					throw "No operand for operator.";
				}
				// Else must be some sort of operand. -> Skip Single Operand.
				else {
					action_skip(next_head());
					expected_head = e_expected_head::operator_;
				}
			}
			else if (expected_head == e_expected_head::operator_) {
				// Looking for a binary operator, postfix operator, or open paren(function call), or open brace(index operator) or open bracket(type constraint)
				// Open Paren -> Function Call -> Check Entire Function Call.
				if (head_->type_is(e_tk::open_paren_)) {
					auto scope = tk_scope::find_paren(head_, end_);
					if (!scope.valid()) {
						throw "Mismatched parentheses in operand.";
					}
					else {
						if (first_pass) action_store(scope.end());
						else action_check(scope.end());
						first_pass_switch(); // Switch on first operator.
						expected_head = e_expected_head::operator_;
					}
				}
				// Open Brace -> Index Operator -> Check Entire Index Operator.
				else if (head_->type_is(e_tk::open_brace_)) {
					auto scope = tk_scope::find_brace(head_, end_);
					if (!scope.valid()) {
						throw "Mismatched braces in operand.";
					}
					else {
						if (first_pass) action_store(scope.end());
						else action_check(scope.end());
						first_pass_switch(); // Switch on first operator.
						expected_head = e_expected_head::operator_;
					}
				}
				// Open Bracket -> Type Constraint -> Check Entire Type Constraint.
				else if (head_->type_is(e_tk::open_bracket_)) {
					auto scope = tk_scope::find_bracket(head_, end_);
					if (!scope.valid()) {
						throw "Mismatched brackets in operand.";
					}
					else {
						if (first_pass) action_store(scope.end());
						else action_check(scope.end());
						first_pass_switch(); // Switch on first operator.
						expected_head = e_expected_head::operator_;
					}
				}
				// Binary Operator -> Check Single Binary Operator.Followed by operative.
				else if (head_->operation() == e_operation::binary_) {
					if (first_pass) action_store(next_head());
					else action_check(next_head());
					first_pass_switch(); // Switch on first operator.
					expected_head = e_expected_head::operative_;
				}
				// Postfix Operator -> Check Single Postfix Operator.Followed by operator.
				else if (head_->operation() == e_operation::postfix_) {
					if (first_pass) action_store(next_head());
					else action_check(next_head());
					first_pass_switch(); // Switch on first operator.
					expected_head = e_expected_head::operator_;
				}
				// Prefix Operator -> Error,prefix after operand.
				else if (head_->operation() == e_operation::prefix_) {
					throw "Prefix following operand in expression.";
				}
				// Else must be some sort of operand. -> Error, operand after operand.
				else {
					throw "Operand following operand in expression.";
				}

			}
			else {
				throw "[Programmer Logic Error] expected_head in choose_action() is set to an invalid value.";
			}
		}
	}

	std::vector<tk> parenthesize_expression() {
		while(!finished) {
			choose_action();
		}

		return parenthesized();
	}

};

ast parse_expression_impl(tk_vector_cit begin, tk_vector_cit end) {
	tk_iterator it(begin, end);
	ast node;
	enum class e_expected { operand_, operator_, any_ } expected = e_expected::any_;

	// Open Paren -> Subexpr or Redundant Parentheses.
	if (it.type_is(e_tk::open_paren_)) {
		auto scope = tk_scope::find_paren(it.it(), it.end());
		if (!scope.valid()) {
			throw "Mismatched parentheses in operand.";
		}
		else {
			// Check for redundant parentheses.
			if (scope.end() == it.end()) {
				// Parse the inside of the parentheses.
				return parse_expression_impl(scope.contained_begin(), scope.contained_end());
			}
			else {
				// Scope is an operand contained in a subexpression.
				ast operand = parse_expression_impl(scope.contained_begin(), scope.contained_end());
				it.advance(scope.end());

				// Operand may be followed by a postfix, or a binary operator.
				if (it.operation() == e_operation::postfix_) {
					// Postfix () -> Function Call
					if (it.type_is(e_tk::open_paren_)) {
						auto scope = tk_scope::find_paren(it.it(), it.end());
						if (!scope.valid()) {
							throw "Mismatched parentheses in function call.";
						}
						else {
							// Parse the inside of the parentheses.
							node = ast{ e_ast::function_call_ };
							node.push_back(operand);
							node.push_back(ast{ e_ast::arguments_ }); // TODO: parse_args
							it.advance(scope.end());
						}
					}
					// Postfix {} -> Index Operator
					else if (it.type_is(e_tk::open_brace_)) {
						auto scope = tk_scope::find_brace(it.it(), it.end());
						if (!scope.valid()) {
							throw "Mismatched braces in index operator.";
						}
						else {
							// Parse the inside of the parentheses.
							node = ast{ e_ast::index_operator_ };
							node.push_back(operand);
							node.push_back(e_ast::index_arguments_); // TODO: parse_index_args
							it.advance(scope.end());
						}
					}
					// Postfix [] -> Type Call
					else if (it.type_is(e_tk::open_bracket_)) {
						auto scope = tk_scope::find_bracket(it.it(), it.end());
						if (!scope.valid()) {
							throw "Mismatched brackets in type call.";
						}
						else {
							// Parse the inside of the parentheses.
							node = ast{ e_ast::type_call_ };
							node.push_back(operand);
							node.push_back(e_ast::type_arguments_); // TODO: parse_type_args
							it.advance(scope.end());
						}
					}
					// Postfix Single Token Operator.
					else {
						node = ast{ it.get() };
						node.push_back(operand);
						it.advance();
					}
				}
				else if (it.operation() == e_operation::binary_) {
					node = ast{ it.get() };
					node.push_back(operand);
					it.advance();

					// Expecting an operand after a binary operator.
					// Open Paren -> Subexpr
					if (it.type_is(e_tk::open_paren_)) {
						auto scope = tk_scope::find_paren(it.it(), it.end());
						if (!scope.valid()) {
							throw "Mismatched parentheses in operand.";
						}
						else {
							// Parse the inside of the parentheses.
							node.push_back(parse_expression_impl(scope.contained_begin(), scope.contained_end()));
							it.advance(scope.end());
						}
					}
					else if (it.operation() == e_operation::none_) {
						node.push_back(ast{ it.get() });
						it.advance();
					}
					else
						throw "Invalid token following operand in subexpression.";
				}
				else {
					throw "Invalid token following operand in subexpression.";
				}

			}
		}

	}
	else if (it.operation() == e_operation::none_) {
		// Single Operand
		ast operand = ast{ it.get() };
		it.advance();

		if (it.at_end()) {
			// Single operand expression.
			node = operand;
			return node;
		}

		// Operand may be followed by a postfix, or a binary operator.
		if (it.operation() == e_operation::postfix_) {
			if (it.type_is(e_tk::open_paren_)) {
				auto scope = tk_scope::find_paren(it.it(), it.end());
				if (!scope.valid()) {
					throw "Mismatched parentheses in function call.";
				}
				else {
					// Parse the inside of the parentheses.
					node = ast{ e_ast::function_call_ };
					node.push_back(operand);
					node.push_back(ast{ e_ast::arguments_ });
					it.advance(scope.end());
				}
			}
			// Postfix {} -> Index Operator
			else if (it.type_is(e_tk::open_brace_)) {
				auto scope = tk_scope::find_brace(it.it(), it.end());
				if (!scope.valid()) {
					throw "Mismatched braces in index operator.";
				}
				else {
					// Parse the inside of the parentheses.
					node = ast{ e_ast::index_operator_ };
					node.push_back(operand);
					node.push_back(e_ast::index_arguments_); // TODO: parse_index_args
					it.advance(scope.end());
				}
			}
			// Postfix [] -> Type Call
			else if (it.type_is(e_tk::open_bracket_)) {
				auto scope = tk_scope::find_bracket(it.it(), it.end());
				if (!scope.valid()) {
					throw "Mismatched brackets in type call.";
				}
				else {
					// Parse the inside of the parentheses.
					node = ast{ e_ast::type_call_ };
					node.push_back(operand);
					node.push_back(e_ast::type_arguments_); // TODO: parse_type_args
					it.advance(scope.end());
				}
			}
			else {
				node = ast{ it.get() };
				node.push_back(operand);
				it.advance();
			}
		}
		else if (it.operation() == e_operation::binary_) {
			node = ast{ it.get() };
			node.push_back(operand);
			it.advance();

			// Expecting an operand after a binary operator.
			// Open Paren -> Subexpr
			if (it.type_is(e_tk::open_paren_)) {
				auto scope = tk_scope::find_paren(it.it(), it.end());
				if (!scope.valid()) {
					throw "Mismatched parentheses in operand.";
				}
				else {
					// Parse the inside of the parentheses.
					node.push_back(parse_expression_impl(scope.contained_begin(), scope.contained_end()));
					it.advance(scope.end());
				}
			}
			else if (it.operation() == e_operation::none_) {
				node.push_back(ast{ it.get() });
				it.advance();
			}
			else
				throw "Invalid token following operand in subexpression.";
		}
		else {
			throw "Invalid token following operand in subexpression.";
		}

	}
	else if (it.operation() == e_operation::prefix_) {
		// Prefix Operator, must be followed by an operand.
		node = ast{ it.get() };
		it.advance();
		// Open Paren -> Subexpr
		if (it.type_is(e_tk::open_paren_)) {
			auto scope = tk_scope::find_paren(it.it(), it.end());
			if (!scope.valid()) {
				throw "Mismatched parentheses in operand.";
			}
			else {
				// Parse the inside of the parentheses.
				node.push_back(parse_expression_impl(scope.contained_begin(), scope.contained_end()));
				it.advance(scope.end());
			}
		}
		else if (it.operation() == e_operation::none_) {
			node.push_back(ast{ it.get() });
			it.advance();
		}
		else
			throw "Invalid token following operand in subexpression.";
	}
	else
		throw "Invalid token at start of expression.";

	return node;
}

ast parse_expression(tk_vector_cit begin, tk_vector_cit end) {
	parenthesizer p(begin, end);
	auto parenthesized = p.parenthesize_expression();
	auto node = parse_expression_impl(parenthesized.begin(), parenthesized.end());
	return node;
}