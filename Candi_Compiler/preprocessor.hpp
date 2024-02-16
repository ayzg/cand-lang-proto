#pragma once
#include "parser_utils.hpp"
namespace caoco {
	sl_tuple<tk_vector, bool, sl_string> preprocess(tk_vector code,sl_string source_file, sl_vector<sl_u8string> already_included_files = {}) {
		tk_vector output;
		output.reserve(code.size()); // Code will probably be the same size or larger after preprocessing
		sl_vector<sl_u8string> included_files = already_included_files;
		// Include directives

		tk_cursor c = { code.begin(), code.end() };

		while (!c.at_end()){
			if (c.type_is(tk_enum::include_)) {
				c.advance();
				if(!c.type_is(tk_enum::string_literal_)){
					return std::make_tuple(output,false,
						sl_string("[C&][ERROR][pre-processor] file: " 
							+ source_file 
							+ "#include directive not followed by a string literal file name.")
					);
				}
				auto file_name = c.get().literal();
				// Remove quotes
				file_name = file_name.substr(1, file_name.size() - 2);

				if (sl::to_str(file_name) == source_file) {
					auto error_message = 
						sl_string("[C&][ERROR][pre-processor] file: " + source_file
						+ '\n' + "File cannot recursiveley include self.");
					return std::make_tuple(output, false, error_message.c_str());
				}

				if(included_files.end() != std::find(included_files.begin(), included_files.end(), file_name)){
					c.advance();
					std::cout << "[C&][WARNING][pre-processor] File " << sl::to_str(file_name) 
						<< " already included. Inclusion will be ignored. Consider removing duplicate include directive." 
						<< std::endl;
					continue;
				}
				else {
					included_files.push_back(file_name);
				}

				try {
					auto source_file = sl::load_file_to_char8_vector(sl::to_str(file_name));
					try {
						auto tokenized_file = tokenizer(source_file.cbegin(), source_file.cend())();
						auto preprocess_result = preprocess(tokenized_file, sl::to_str(file_name));
						if (!sl::get<1>(preprocess_result)) {
							auto error_message =
								sl_string("[C&][ERROR][pre-processor] file: ") + sl::get<2>(preprocess_result);
							return std::make_tuple(output, false, error_message.c_str());
						}
						for (const auto& t : sl::get<0>(preprocess_result)) {
							output.push_back(t);
						}
						c.advance();

						for (auto a : output) {
							std::cout << a.literal_str() << std::endl;
						}
					}
					catch (const std::exception& e) {
						auto error_message =
							sl_string("[C&][ERROR][pre-processor] file: ") + sl::to_str(file_name) + ": " + e.what();
						return std::make_tuple(output, false, error_message.c_str());
					}
				}
				catch (const std::exception& e) {
					auto error_message = 
						sl_string("[C&][ERROR][pre-processor] could not find or load file: ") + sl::to_str(file_name) + ": " + e.what();
					return std::make_tuple(output, false, error_message.c_str());
				}
			}
			else {
				output.push_back(c.get());
				c.advance();
			}
		}
		return std::make_tuple(output, true, "");
	}



}

