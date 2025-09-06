//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: ast_frame.h
//---------------------------------------------------------------------------//
// Brief: Provides a template for AST types produced by the parser.
//        Allows for easier interface and building of the AST nodes.
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_COMMON_AST_FRAME_H
#define HEADER_GUARD_CAOCO_COMMON_AST_FRAME_H
// Includes:
#include "cand_syntax.h"
#include "import_stl.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

struct AstFrame {
  Ast root{eAst::kUndefined};
  const Ast & Build() { return root; };
};

struct FrameVarDef : AstFrame {
  Ast& modifiers;
  Ast& identifier;
  Ast& constraint;
  Ast& initializer;
};

struct FrameFunctionDef : AstFrame {
  Ast& modifiers;
  Ast& identifier;
  Ast& parameters;
  Ast& return_type;
  Ast& body;
};

struct FrameClassDef : AstFrame {
  Ast& modifiers;
  Ast& identifier;
  Ast& body;
};




//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: common
// File: ast_frame.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_COMMON_AST_FRAME_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
