//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_runtime.h
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_RUNTIME_H
#define HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_RUNTIME_H
// Includes:
#include "lexer.h"
#include "lark_parser.h"
#include "cand_lang.h"
#include "ir_codegen.h"
#include "evaluator.h"
#include "minitest_flags.h"  // Flags to enable or disable the unit tests
#include "minitest_pch.h"    // All pre includes for each unit test
#include "minitest_util.h"   // Utility methods shared among the all unit tests
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

MINITEST(ut0_runtime, Basic) {
  // 0. Runtime environment. Only one global environment is created per program.
  RuntimeEnv global_env;

  // 1. Define a global variable.
  global_env.Define("GFoo", eNameCategory::kVar, {RtVal::kInt, 10});

  // 2. Define a global object.
  // 2.0. Define a sub-environment for the object.
  RuntimeEnv& subenv = global_env.Subenv();
  // 2.1. Pass the current environment to the
  // CandObject constructor.
  subenv.Define("Foobar", eNameCategory::kClass,
                {RtVal::kObject,
                 std::shared_ptr<CandObject>(new CandObject(subenv))});

  // 3. Retrieve the class object from the global environment.
  RtVal& local_Foobar = subenv.RetrieveLocal("Foobar", eNameCategory::kClass);

  // Able to retrieve the global variable from the object context.
  RtVal& global_GFoo = subenv.RetrieveGlobal("GFoo", eNameCategory::kVar);
  // 4. Print the global variable.
  std::cout << "global_GFoo: " << global_GFoo.GetAs<NativeInt>() << std::endl;
  
  // 5. Define a static variable in the object context.
  NativeCaObject obj = local_Foobar.GetAs<NativeCaObject>();
  obj->object_env.Define("Member", eNameCategory::kVar, {RtVal::kInt, 100});

  // 6. Retrieve the static variable from the object context.
  RtVal& foobar_object_member =
      obj->object_env.RetrieveLocal("Member", eNameCategory::kVar);
  std::cout << "Member: " << foobar_object_member.GetAs<RtVal::IntT>()
            << std::endl;

  // 7. Define a static method in the object context.
  obj->object_env.Define(
      "Method", eNameCategory::kFunction,
      {RtVal::kMethod, std::shared_ptr<CandMethod>(new CandMethod({"x"}))});
  auto& method =
      obj->object_env.RetrieveLocal("Method", eNameCategory::kFunction);
  std::vector<RtVal*> args;
  args.push_back(new RtVal{RtVal::kInt, 10});
  auto method_ptr = method.GetAs<RtVal::MethodT>();
  method_ptr->call(args, obj->object_env);
  // Get the result as an int.
  RtVal result = method_ptr->GetResultAndFlush();
  std::cout << "Method result: " << result.GetAs<RtVal::IntT>() << std::endl;
  // Call method again. Result should be the same. Pass by value.

  result = method_ptr->call(args, obj->object_env).GetResultAndFlush();
  std::cout << "Method result: " << result.GetAs<RtVal::IntT>() << std::endl;
}
END_MINITEST;

MINITEST(ut0_runtime, IrCodeBasic) {
  auto source_code = LoadFileToVec("animal_sounds1.cand");
  auto tokens = Lexer::Lex(source_code);
  if (!tokens) {
    std::cout << "Lexer error:" << tokens.Error() << std::endl;
    return;
  }

  auto parser = LarkParser::Parse(tokens.Value());

  if (!parser) {
    std::cout << "Parser error:" << parser.Error() << std::endl;
    return;
  }

  PrintAst(parser.Value());

  //IrGen irgen;
  //irgen.GenerateIr(parser.Value());

}
END_MINITEST;


MINITEST(ut0_runtime, IrCodeVariableDecl) {
  auto source_code = LoadFileToVec("variable_decl.cand");
  auto tokens = Lexer::Lex(source_code);
  if (!tokens) {
    std::cout << "Lexer error:" << tokens.Error() << std::endl;
    return;
  }

  auto parser = LarkParser::Parse(tokens.Value());

  if (!parser) {
    std::cout << "Parser error:" << parser.Error() << std::endl;
    return;
  }

  PrintAst(parser.Value());

  IrGen irgen;
  IrCode ircode = irgen.GenerateIr(parser.Value());

  ircode.PrintDisassembly();

  Environment env;
  Evaluator eval{env};

  eval.Evaluate(ircode.GetLines(), ircode.GetLines().begin(), ircode.GetLines().end());


}
END_MINITEST;
//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: unit_tests
// File: ut0_expected.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_UNIT_TESTS_UT0_RUNTIME_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//