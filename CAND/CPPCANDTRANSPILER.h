//#pragma once
//
//#include <string>
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <limits>
//#include <string_view>
//#include <vector>
//#include <memory>
//
//namespace CandiCompiler
//{
//	namespace util
//	{
//		// Reads in user written code and removes unnecessary characters to optimize next compiler step : tokenization
//		class lexer_raw_optimizer
//		{
//			std::ifstream infile;
//			
//			std::string load_and_clean_candi_raw(const std::string_view candi_path, 
//				const std::ios::openmode openmode = std::ios::in)
//			{
//
//				infile.open(candi_path.data(), openmode);
//
//				infile.ignore(std::numeric_limits<std::streamsize>::max());
//
//				std::string candi_raw_lexed;
//
//				candi_raw_lexed.reserve(infile.gcount());
//				
//				//infile.clear();
//
//				infile.read(candi_raw_lexed.data(), candi_raw_lexed.size());
//			}
//		};
//		
//	}
//
//	//constexpr const bool load_and_clean_candi_raw(const std::string_view candi_path,
//	//	const std::string & target, const std::ios::openmode & openmode = std::ios::in)
//	//{
//	//	infile.set_rdbuf(candi_path.data(), openmode);
//
//	//	infile.ignore(std::numeric_limits<std::streamsize>::max());
//
//	//	candi_raw_input = string_type(infile.gcount(), 0); //use reserve??
//
//	//	infile.clear();
//
//	//	infile.read(candi_raw_input.data(), candi_raw_input.size());
//	//}
//	using string_type = std::string;
//	using string_type_ref = string_type &;
//	using string_type_ptr = string_type *;
//	using string_type_const_ref = string_type const &;
//	using string_type_const_ptr = string_type const *;
//
//	using string_type_iterator = string_type::iterator;
//	using string_type_const_iterator = string_type::const_iterator;
//	using string_type_reverse_iterator = string_type::reverse_iterator;
//	using string_type_const_reverse_iterator = string_type::const_reverse_iterator;
//
//	class CPPCANDTRANSPILER_PROCESS
//	{
//		string_type candi_raw_input;
//	public:
//		explicit CPPCANDTRANSPILER_PROCESS(const std::string_view candi_path) : candi_raw_input(candi_path)
//		{
//
//			/*
//			
//			tellg() is not guaranteed to return the offset in bytes since the beginning of the file. Instead, as Puzomor Croatia pointed out, it's more of a token which can be used within the fstream calls. gcount() however does return the amount of unformatted bytes last extracted. We therefore open the file, extract and discard all of its contents with ignore() to get the size of the file, and construct the output string based on that.
//
//Secondly, we avoid having to copy the data of the file from a std::vector<char> to a std::string by writing to the string directly.
//
//In terms of performance, this should be the absolute fastest, allocating the appropriate sized string ahead of time and calling read() once. As an interesting fact, using ignore() and countg() instead of ate and tellg() on gcc compiles down to almost the same thing, bit by bit.
//*/
//			std::ios::openmode openmode = std::ios::in;
//
//			std::ifstream infile(candi_path.data(), openmode);
//			
//			infile.ignore(std::numeric_limits<std::streamsize>::max());
//
//			candi_raw_input = string_type(infile.gcount(), 0); //use reserve??
//
//			infile.clear();
//
//			infile.read(candi_raw_input.data(), candi_raw_input.size());
//
//			
//			// Usercode Compiler Optimization Lexer
//			// Omit all unnecessary whitespace and comment characters in the input
//		}
//
//		~CPPCANDTRANSPILER_PROCESS()
//		{
//		}
//
//	private:
//
//	};
//
//	class CPPCANDTRANSPILER
//	{
//		explicit CPPCANDTRANSPILER() {}
//
//	};
//
//}