#include <ranges>
#include "tokenizer.hpp"

namespace cil {
	class Sanitizer {
		const rvector<Token> & tokens;
		const rvector<TokenKind> REDUNDANT_TOKEN_KINDS = {

		};
	public:
		Sanitizer(const rvector<Token> & tokens) : tokens(tokens) {}

		auto sanitize(const rvector<Token>& tokens) {
			rvector<Tokenizer::token_iterator> sanitized;
			for (auto it = tokens.begin(); it != tokens.end(); ++it) {

				if (std::any_of(REDUNDANT_TOKEN_KINDS.begin(), REDUNDANT_TOKEN_KINDS.end(),
					[it](TokenKind match) { return match == it->type(); })) {
					continue;
				};

				sanitized.push_back(it);
			}

			// Create a non-owning view of iterators using std::ranges::subrange
			return std::ranges::subrange(sanitized);
		}

	};
}