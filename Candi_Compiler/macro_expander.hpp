#pragma once
#include "parser_utils.hpp"
namespace caoco {

	sl_tuple<tk_vector, bool, sl_string> macro_expand(tk_vector code, sl_string source_file) {
		tk_vector output;
		output.reserve(code.size()); // Code will probably be the same size or larger after preprocessing

		tk_cursor c = { code.begin(), code.end() };
		std::map<sl_u8string, std::vector<tk>> defined_macros;

		while (!c.at_end()) {
			if (c.type_is(tk_enum::macro_)) { // everything following the macro name until #endmacro is the macro.
				c.advance();
				if (!c.type_is(tk_enum::alnumus_)) {
					return std::make_tuple(output, false,
						sl_string("[C&][ERROR][macro-expander] file: "
							+ source_file
							+ "#macro directive not followed by a valid identifier.")
					);
				}
				auto macro_name = c.get().literal();

				if (defined_macros.find(macro_name) != defined_macros.end()) {
					return std::make_tuple(output, false,
						sl_string("[C&][ERROR][macro-expander] file: "
							+ source_file
							+ "Macro " + sl::to_str(macro_name) + " already defined.")
					);
				}
				c.advance();


				tk_vector macro_body;
				while (!c.type_is(tk_enum::endmacro_)) {
					macro_body.push_back(c.get());
					c.advance();
				}
				c.advance(); // skip #endmacro

				// Add macro to defined_macros
				defined_macros[macro_name] = macro_body;

			}
			else {
				// Check if a macro is being invoked.
				if (c.type_is(tk_enum::alnumus_)) {
					auto identifier_name = c.get().literal();
					if (defined_macros.find(identifier_name) != defined_macros.end()) {
						// Macro is being invoked
						auto macro_body = defined_macros[identifier_name];
						for (const auto& t : macro_body) {
							output.push_back(t);
						}
						c.advance();
					}
				}
				else {
					output.push_back(c.get());
					c.advance();
				}
			}
		}
		return std::make_tuple(output, true, "");
	}
}
