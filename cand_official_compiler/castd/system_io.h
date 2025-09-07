////=-------------------------------------------------------------------------=//
////---------------------------------------------------------------------------//
//// All Rights Reserved | Copyright 2024 NTONE INC.
//// Author: Anton Yashchenko
//// Email: ntondev@gmail.com
//// Website: https://www.bigteeny.com
////---------------------------------------------------------------------------//
//// Project: cand_official_compiler
//// Directory: castd
//// File: system_io.h
////---------------------------------------------------------------------------//
//// Brief: Filesystem I/O utilities.
////---------------------------------------------------------------------------//
//#ifndef HEADER_GUARD_CAND_OFFICIAL_COMPILER_CASTD_SYSTEM_IO_H
//#define HEADER_GUARD_CAND_OFFICIAL_COMPILER_CASTD_SYSTEM_IO_H
//// Includes:
//#include "import_stl.h"
//
////---------------------------------------------------------------------------//
////=-------------------------------------------------------------------------=//
//
////=---------------------------------=//
//// Method: LoadFileToU8Vec
//// Parameters:
////		1.name: The name of the file to load.
//// Returns:
////		Contents of the file as a vector of chars
////    If the last char is not a null terminator, adds one.
////=---------------------------------=//
//
////static std::vector<char> LoadFileToVec(std::string name) {
////  std::ifstream ifs(name, std::ios::binary | std::ios::ate);
////
////  if (!ifs) throw std::runtime_error(name 
////    + ": LoadFileToU8Vec : File Could not be opened.");
////
////  auto end = ifs.tellg();
////  ifs.seekg(0, std::ios::beg);
////
////  auto size = std::size_t(end - ifs.tellg());
////
////  if (size == 0)  // avoid undefined behavior due to zero size.
////    return {};
////
////  std::vector<std::byte> buffer(size);
////
////  if (!ifs.read((char*)buffer.data(), buffer.size()))
////    throw std::runtime_error(name 
////      + ": LoadFileToU8Vec : File Could not be read.");
////
////  std::vector<char> chars;
////  chars.reserve(buffer.size());
////  for (auto& c : buffer) {
////    chars.push_back(static_cast<char>(c));
////  }
////
////  if (chars.back() != '\0') chars.push_back('\0');
////
////  return chars;
////}
//
////=-------------------------------------------------------------------------=//
////---------------------------------------------------------------------------//
//// All Rights Reserved | Copyright 2024 NTONE INC.
//// Author: Anton Yashchenko
//// Email: ntondev@gmail.com
//// Website: https://www.bigteeny.com
////---------------------------------------------------------------------------//
//// Project: cand_official_compiler
//// Directory: castd
//// File: system_io.h
////---------------------------------------------------------------------------//
//#endif HEADER_GUARD_CAND_OFFICIAL_COMPILER_CASTD_SYSTEM_IO_H
////---------------------------------------------------------------------------//
////=-------------------------------------------------------------------------=//
