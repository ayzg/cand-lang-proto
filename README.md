# [Archive] C& Practical Programming Language
- Archived for https://github.com/zhymet/cand-lang. 
- Maintains pull request references to transferred issues from this repo.
- Pulicized for educational purpose, and post mortem reference.
- Copyright Anton Yashchenko 2025

## Grafted Commits
```
*   90f33c5 (HEAD -> master, origin/master-backup, origin/master, origin/HEAD, master-backup) -  resolve .gitignore conflic
|\
| *   848cbf8 Merge pull request #76 from zhymet/append-new-commits
| |\
| | * 8d9802e + major rework 1
| |/
| * 20178cd Create docs.md
| *   943ac29 Merge pull request #75 from zhymet/working
| |\
| | * a1030dd Unit Tests for Const Evaluator Literals/Operands/ArithmeticExprs/Structs
| * | b72e84d Merge pull request #71 from zhymet/working
| |\|
| | * 4c84e6a Preprocessor Unit Tests for Include directive and macros. Implemented simple macro expander. Declare macros with keyword 'macro' end with keyword 'endmacro'.
| * | fc8d747 Merge pull request #70 from zhymet/working
| |\|
| | * 61f0b6f Finished Adding new var and function syntax to parser and tokenizer.
| * | 8af8fd1 Merge pull request #69 from zhymet/working
| |\|
| | * 9794ec8 New Variable Decl And Type Alias Syntax
| * | 8f2fc31 Merge pull request #68 from zhymet/working
| |\|
| | * 2b8403a Constexpr Tokenizer
| | * f0a4030 Reworking tokenizer and parser tests to compare to an expected ast result(and making the tests more clean/useful).
| | * ebf9c1f updated expr tests
| | * cc74a34 closes #4
| * | 7d51cdc Update README.md
| * | 704bccb Update README.md
| * | 5e7415c Update README.md
| * | cfd5a9b Update README.md
| * | a1734df Update README.md
| * | 64e7026 Update README.md
| * | 118ccd4 Update README.md
| * | 6f72480 Update README.md
| * | db513d9 Update README.md
| * | b5af88a Merge pull request #46 from zhymet/working
| |\|
| | * 3ce268e Preprocessor Initial
| * | 3fcb11f Merge pull request #45 from zhymet/working
| |\|
| | * 6b5b198 If(Conditinals), While, For, Switch(#do) implemented in the parser.
| * | d9897bc Merge pull request #44 from zhymet/working
| |\|
| | * 9c10844 Removed all previous parser tests, parsing functors, and related utilities.
| | * 6143f82 Parser Refactor
| | * 9861291 Finished reworking the split parse function. Added extensive testing for value expressions.
| | * 5ac0a7a part1 parser rework.
| * | 4018a7b Merge pull request #41 from zhymet/working
| |\|
| | * 147f32e if, elif, else and while astnode parsers.
| * | a69df5a Merge pull request #39 from zhymet/working
| |\|
| | * b68087f Failing to load a file will now report a real error message.
| * | f47584a Merge pull request #38 from zhymet/working
| |\|
| | * f240cd8 Removed All Unecessary Method from astnode
| * | 3f105b6 Merge pull request #37 from zhymet/working
| |\|
| | * ec83d9e #include directive , ability to include scripts in other scripts.
| * | 837b361 Merge pull request #34 from zhymet/working
| |\|
| | * a0b2851 Clean up syntax_traits.hpp , created constants for 'priority'
| * | 4bb4d0c Merge pull request #33 from zhymet/working
| |\|
| | * e216593 closes #24 closes #23 closes #29
| | * ab90d35 closes #29
| * | 007dcb4 Merge pull request #32 from zhymet/working
| |\|
| | * a076c2c More Refactoring of ast_node.hpp, re-organizing and cleaning up the unit tests.
| | * aa7a1a8 Bit more cleaning up of typedefs/refactoring.
| * | b182fe4 Merge pull request #31 from zhymet/working
| |\|
| | * b5d4810 Major Refactor Part 1. Remove unnecessary files to scrapbook folder. closes #30
| * | 985a375 Update README.md
| * | 02fac4a Merge pull request #22 from zhymet/working
| |\|
| | * c7c942e ConstEvaluator Class Declarations with member variables.
| * | d6472e4 Merge pull request #21 from zhymet/working
| |\|
| | * ecf5318 Identifier and Anon variable Declaration Constant evaluators for variables and variable decl.
| * | 38d1b9d Merge pull request #20 from zhymet/working
| |\|
| | * 5cdfc4f Implementation of binary expr in constant evaluator + other minor syntax changes.
| * | e361cf0 Merge pull request #19 from zhymet/working
| |\|
| | * 6f5cdbc Unsigned,Bit and Octet literal tokens. 1. Added new Tk::eType enums:     -octet_literal, bit_literal, unsigned_literal 2.Added test for number following ellipsis. 3. Added unsigned, bit, octet lite
rals in tokenizer. And token test for them.     - 0b , 1b for bit     - 0-255c , and '_'c for octet     - 1234u for unsigned In tokenizer.hpp: lex_apostrophe() now checks for octet literal after string literal i
s lexed. lex_number handles u,c and b literal modifiers after the number. closes #7 closes #6
| * | 96c02d1 Merge pull request #18 from zhymet/working
| |\|
| | * 18b5918 closes #13 Removed ureal token from parser, tokenizer and tests.
| * | e43c8d3 Merge pull request #17 from zhymet/working
| |\|
| | * 861c481 closes #8 - Added the #none directive to the tokenizer and parser. - Added single token test for #none directive.
| * | eca6463 Merge pull request #16 from zhymet/working
| |\|
| | * 7c81548 - Added tokenizer test for single tokens - Fixed logic in lex_operator() in tokenizer.hpp to properly lex a three-way-comparison. - Fixed small error where lexing a #enter token returns a #type tok
en. - Changed peek() method in tokenizer.hpp to correctly check if iterator is out of bounds.
| * | c02c4ff Merge pull request #12 from zhymet/working
| |\|
| | * c49bf05 closes #11
| | *   a2ab1c0 Merge branch 'working' of https://github.com/zhymet/Candi_Compiler into working
| | |\
| | | * ff84397 Resolved issue  closes#11 Removed TODO: and FIXME: comments and turned them into GitHub issues.
| | |/
| |/|
| | * 844e8af Resolved issue  #11 Removed TODO: and FIXME: comments and turned them into GitHub issues.
| |/
| * 59e9361 Update README.md
| * 1ee79c3 First Commit
| * b11a745 Initial CommitAdd .gitattributes, .gitignore, and README.md.
* 38726d1 [gmash-lineage-merge] Deleting old repo, ready tp merge to new.
*   f76be30 Merge pull request #6 from ayzg/legacy.4.0
|\
| * 09acf65 + legacy.4.0 name:"caoco" first-edit:"2024/03/03" last-edit:"2024/04/07"
| * 33eedaf - legacy.3.0
|/
*   08b63ae Merge pull request #5 from ayzg/candi-lang-23-12-26
|\
| * 3547247 legacy.3.0 : 2023/12/26
|/
*   a1e0a2d Merge pull request #4 from ayzg/candil-23-12-19
|\
| * 24160ef legacy.0.3
|/
* 04da085 + old C& code samples
*   665a1a0 Merge pull request #3 from ayzg/cand-compiler-impl-old-phase-tokenizer
|\
| * 2d7133b cand-compiler-impl-old-phase-tokenizer
| * d99f93b candil-hundred-doors
|/
* 94cb6d2 - old
*   0bee3d1 Merge pull request #2 from ayzg/candi-hundred-doors
|\
| * ae0af42 - remove previous vers
| * 3512169 name:"candil-parser"
* |   24ca33b Merge pull request #1 from ayzg/cpp-sln-cand-v0-leg1
|\ \
| |/
|/|
| * ad20af2 name:"cpp-sln-cand-v0-leg1" brief:"" license:""
| * 8b62533 name:"cpp-sln-cand-v0-leg1" brief:"" license:""
|/
* 6b935de name:"cpp-sln-cand-v0-leg1-first-git" brief:"First idea I ever had about C&." license:"" ]]
* 1d0de61 Add project files.
* 8e912ab Add .gitignore and .gitattributes.
```

## Transferred Issues
```
$ transfer-all-issues.sh zhymet/cand-lang
Transferring issues from zhymet/C-And-Lang to zhymet/cand-lang
======================================================
Fetching issues from zhymet/C-And-Lang...
Found 37 issues to transfer
[1/37] Transferring issue #73...
https://github.com/zhymet/cand-lang/issues/33
✓ Successfully transferred issue #73
---
[2/37] Transferring issue #72...
https://github.com/zhymet/cand-lang/issues/34
✓ Successfully transferred issue #72
---
[3/37] Transferring issue #67...
https://github.com/zhymet/cand-lang/issues/35
✓ Successfully transferred issue #67
---
[4/37] Transferring issue #65...
https://github.com/zhymet/cand-lang/issues/36
✓ Successfully transferred issue #65
---
[5/37] Transferring issue #63...
https://github.com/zhymet/cand-lang/issues/37
✓ Successfully transferred issue #63
---
[6/37] Transferring issue #57...
https://github.com/zhymet/cand-lang/issues/38
✓ Successfully transferred issue #57
---
[7/37] Transferring issue #54...
https://github.com/zhymet/cand-lang/issues/39
✓ Successfully transferred issue #54
---
[8/37] Transferring issue #53...
https://github.com/zhymet/cand-lang/issues/40
✓ Successfully transferred issue #53
---
[9/37] Transferring issue #52...
https://github.com/zhymet/cand-lang/issues/41
✓ Successfully transferred issue #52
---
[10/37] Transferring issue #48...
https://github.com/zhymet/cand-lang/issues/42
✓ Successfully transferred issue #48
---
[11/37] Transferring issue #47...
https://github.com/zhymet/cand-lang/issues/43
✓ Successfully transferred issue #47
---
[12/37] Transferring issue #43...
https://github.com/zhymet/cand-lang/issues/44
✓ Successfully transferred issue #43
---
[13/37] Transferring issue #42...
https://github.com/zhymet/cand-lang/issues/45
✓ Successfully transferred issue #42
---
[14/37] Transferring issue #40...
https://github.com/zhymet/cand-lang/issues/46
✓ Successfully transferred issue #40
---
[15/37] Transferring issue #36...
https://github.com/zhymet/cand-lang/issues/47
✓ Successfully transferred issue #36
---
[16/37] Transferring issue #35...
https://github.com/zhymet/cand-lang/issues/48
✓ Successfully transferred issue #35
---
[17/37] Transferring issue #30...
https://github.com/zhymet/cand-lang/issues/49
✓ Successfully transferred issue #30
---
[18/37] Transferring issue #29...
https://github.com/zhymet/cand-lang/issues/50
✓ Successfully transferred issue #29
---
[19/37] Transferring issue #28...
https://github.com/zhymet/cand-lang/issues/51
✓ Successfully transferred issue #28
---
[20/37] Transferring issue #27...
https://github.com/zhymet/cand-lang/issues/52
✓ Successfully transferred issue #27
---
[21/37] Transferring issue #26...
https://github.com/zhymet/cand-lang/issues/53
✓ Successfully transferred issue #26
---
[22/37] Transferring issue #25...
https://github.com/zhymet/cand-lang/issues/54
✓ Successfully transferred issue #25
---
[23/37] Transferring issue #24...
https://github.com/zhymet/cand-lang/issues/55
✓ Successfully transferred issue #24
---
[24/37] Transferring issue #23...
https://github.com/zhymet/cand-lang/issues/56
✓ Successfully transferred issue #23
---
[25/37] Transferring issue #15...
https://github.com/zhymet/cand-lang/issues/57
✓ Successfully transferred issue #15
---
[26/37] Transferring issue #14...
https://github.com/zhymet/cand-lang/issues/58
✓ Successfully transferred issue #14
---
[27/37] Transferring issue #13...
https://github.com/zhymet/cand-lang/issues/59
✓ Successfully transferred issue #13
---
[28/37] Transferring issue #11...
https://github.com/zhymet/cand-lang/issues/60
✓ Successfully transferred issue #11
---
[29/37] Transferring issue #10...
https://github.com/zhymet/cand-lang/issues/61
✓ Successfully transferred issue #10
---
[30/37] Transferring issue #9...
https://github.com/zhymet/cand-lang/issues/62
✓ Successfully transferred issue #9
---
[31/37] Transferring issue #8...
https://github.com/zhymet/cand-lang/issues/63
✓ Successfully transferred issue #8
---
[32/37] Transferring issue #7...
https://github.com/zhymet/cand-lang/issues/64
✓ Successfully transferred issue #7
---
[33/37] Transferring issue #6...
https://github.com/zhymet/cand-lang/issues/65
✓ Successfully transferred issue #6
---
[34/37] Transferring issue #5...
https://github.com/zhymet/cand-lang/issues/66
✓ Successfully transferred issue #5
---
[35/37] Transferring issue #4...
https://github.com/zhymet/cand-lang/issues/67
✓ Successfully transferred issue #4
---
[36/37] Transferring issue #3...
https://github.com/zhymet/cand-lang/issues/68
✓ Successfully transferred issue #3
---
[37/37] Transferring issue #2...
https://github.com/zhymet/cand-lang/issues/69
✓ Successfully transferred issue #2
---
======================================================
Issue transfer completed!
Transferred 37 issues from zhymet/C-And-Lang to zhymet/cand-lang
```
