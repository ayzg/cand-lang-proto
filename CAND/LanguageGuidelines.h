#pragma once

/*

*********************************************
 C& | CAND | Compiled and Interpreted Language
*********************************************

File names:
	Compiled and Interpreted Code: .cand
	Interpreted Code : .an .andi
	Compiled code : .comp
	Rule code : .ru .rule

How to begin a program / organize empty program

1. Pass the input and output directories to the transpiler
	-> The directories are parsed relative to the location of the transpiler executable
	-> Optionally pass a root directory to transpiler
	-> Optionally pass a filename for the program entry point, must be .cand file, 
2. Find the program entry point, this is the pointer to first character of input code.
	-> Scans for #enter a a reserved token, one whitespace after the token becomes a pointer to the
		begging of the compilation process



Language:

Symbols:
1. " # " : transpiler macro action, calls c++ action through transpiler class, used with reserved keywords only
	ex. 
	*================= enter.cand ==================*
	* int variable_name 
	* #enter 











**********************************************/