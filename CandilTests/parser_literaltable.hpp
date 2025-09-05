#pragma once
#include "global_typedef.hpp"
#include "tokenizer.hpp"


namespace cil {

	// A candil type contains a type identifier and a list of types associated with it.
	struct CandilType {
		rstring identity;
		rvector<SyntaxTraits::CandilTypeKind> types;
	};

	// A candil type table contains a list of currently declared candil types
	class ParserTypeTable {
		rvector<CandilType> types_;
	public:
		auto operator[](size_t index) {
			return types_[index];
		}
		auto operator[](rstring identity) {
			for (auto type : types_) {
				if (type.identity == identity) {
					return type;
				}
			}
			return CandilType{ identity,{SyntaxTraits::CandilTypeKind::undefined_} };
		}
		auto push(rstring identity, rvector<SyntaxTraits::CandilTypeKind> types) {
			assert(types.size() > 0 && "[PROGRAMMER LOGIC ERROR][ParserTypeTable][push] Cannot pass empty types vector.");

			// Check if the type already exists.
			for (auto type : types_) {
				if (type.identity == identity) {
					return types_.cend() - 1;
				}
			}

			// Else continue...
			types_.push_back(CandilType{ identity,types });
			return types_.cend() - 1;
		}
		auto begin() {
			return types_.cbegin();
		}
		auto end() {
			return types_.cend();
		}
		auto cbegin() {
			return types_.cbegin();
		}
		auto cend() {
			return types_.cend();
		}
		auto size() {
			return types_.size();
		}
		auto empty() {
			return types_.empty();
		}
		auto back() {
			return types_.back();
		}
		auto front() {
			return types_.front();
		}
		auto clear() {
			types_.clear();
		}
	};

	// A candil variable contains a type identifier, a value, and an identity.
	struct CandilVariable {
		rstring identity;
		CandilType type;
		std::any value;

		// Based on the CandilType enum, cast the std::any value to the appropriate type.
		template<typename T>
		T get_value() {
			assert(type.types.size() == 1 && "[PROGRAMMER LOGIC ERROR][CandilVariable][get_value] CandilType must have singular underlying CandilTypeKind, multiple type type-identities are not yet implemented.");

			switch (type.types.back()) {
			case SyntaxTraits::CandilTypeKind::int_:
				return std::any_cast<int>(value);
			case SyntaxTraits::CandilTypeKind::unsigned_:
				return std::any_cast<unsigned>(value);
			case SyntaxTraits::CandilTypeKind::real_:
				return std::any_cast<double>(value);
			case SyntaxTraits::CandilTypeKind::char_:
				return std::any_cast<char>(value);
			case SyntaxTraits::CandilTypeKind::bool_:
				return std::any_cast<bool>(value);
			default:
				std::cerr << "[PROGRAMMER LOGIC ERROR][CandilVariable][get_value] Cannot get value of undefined type.";
			}
		}
	};

	CandilVariable c;
	//auto test = c.get_value();

	class ParserVariableTable {

	};



}