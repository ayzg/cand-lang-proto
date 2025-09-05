#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "global_typedef.hpp"

namespace cil {
	auto load_source_file(rstring name) {
        rvector<char> bytes;                                // vector to store the bytes
        std::ifstream file(name, std::ios::binary);         // file to read from
        file.seekg(0, std::ios::end);                       // move the file pointer to the end
        std::streamsize size = file.tellg();                // get the file size
        file.seekg(0, std::ios::beg);                       // move the file pointer to the beginning
        bytes.resize(size);                                 // resize the vector to fit the file size
        file.read(bytes.data(), size);                      // read the file into the vector
        file.close();                                       // close the file

        // if the last char is not a null terminator, add one
        if (bytes.back() != '\0')
            bytes.push_back('\0');

        return bytes;
	}
}