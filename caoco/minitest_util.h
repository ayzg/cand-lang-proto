//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: minitest
// File: minitest_util.h
//---------------------------------------------------------------------------//
// Brief: {description}
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_MINITEST_MINITEST_UTIL_H
#define HEADER_GUARD_CAOCO_MINITEST_MINITEST_UTIL_H
// Includes:
#include "cand_syntax.h"
#include "minitest_pch.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

bool CompareAst(const Ast& node1, const Ast& node2) {
  // Compare node types
  if (node1.Type() != node2.Type()) {
    std::cout << "Node types are different: " << ToStr(node1.Type()) << " and "
              << ToStr(node2.Type()) << std::endl;
    return false;
  }

  // Compare node values
  if (node1.Literal() != node2.Literal()) {
    std::cout << "Node values are different: " << ToStr(node1.Type())
              << "with value:" << node1.Literal() << " and "
              << ToStr(node2.Type()) << "with value:" << node2.Literal()
              << std::endl;
    return false;
  }

  // Compare number of Children
  if (node1.Children().size() != node2.Children().size()) {
    std::cout << "Nodes contain diffrent number of Children: "
              << ToStr(node1.Type()) << "with " << node1.Children().size()
              << " Children and " << ToStr(node2.Type()) << "with "
              << node2.Children().size() << " Children" << std::endl;
    return false;
  }

  // Recursively compare Children
  for (size_t i = 0; i < node1.Children().size(); ++i) {
    if (!CompareAst(node1[i], node2[i])) return false;
  }

  // If all checks pass, the ASTs are equal
  return true;
}

// AstContains returns true if the given AST contains a node with the given type
// and value.
bool AstContains(const Ast& node, eAst type, const std::string& value) {
  if (node.Type() == type && node.Literal() == value) return true;
  for (const auto& child : node.Children()) {
    if (AstContains(child, type, value)) return true;
  }
  return false;
}

void PrintAst(const Ast& node, std::size_t depth = 0 ){


  std::string indent(depth, '  ');
  std::cout << indent << "|";
  /*for (int i = 0; i < depth; i++) */ std::cout << "________";
  std::cout << ">>";
  std::cout << "[T:" << ToStr(node.Type()) << "|L:" << node.Literal() << "]"
            << std::endl;
  for (const auto& child : node.Children())
    PrintAst(child, depth + 1);
}
//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: minitest
// File: minitest_util.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_MINITEST_MINITEST_UTIL_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//