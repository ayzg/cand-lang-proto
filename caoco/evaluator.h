#pragma once
#include "cand_lang.h"
#include "import_stl.h"
#include "ir_codegen.h"

// There will only be one instance of this class per C& program.
// Naming convention taken from llvm: "TheContext.h"
// It will be used to store global variables and settings.
class TheContext;

class Environment {
 public:
  using NameMap =
      std::unordered_map<std::string, std::list<NativeVariant>::iterator>;
  using MemoryList = std::list<NativeVariant>;
  using EnvironmentList = std::list<Environment>;
  using EnvironmentListIter = EnvironmentList::iterator;
  using MemoryIter = MemoryList::iterator;

  std::string_view name{"global"};  // Default name for the root environment.
  Environment& parent{*this};
  EnvironmentList subenvs;

  // Names are associated with a location in memory.
  NameMap variables;
  NameMap functions;
  NameMap types;

  // local static memory
  MemoryList local_memory{NativeVariant(
      NativeCaUndefined())};  // The first element is a placeholder.
                              // Sentinel none value. Variables
                              // Without a value will point to this.
  MemoryList& global_memory{this->local_memory};

  // Local Memory for unnamed values.
  MemoryList local_hot_memory{NativeVariant(NativeCaUndefined())};
  MemoryList& global_hot_memory{this->local_hot_memory};

  // Allocations/Dealloc
  MemoryIter GMAlloc(NativeVariant value);  // Global
  bool GMRelease(MemoryIter mem_location);
  MemoryIter LMAlloc(NativeVariant value);  // Local
  bool LMRelease(MemoryIter mem_location);
  MemoryIter HMAlloc(NativeVariant value);  // Hot
  bool HMRelease(MemoryIter mem_location);

  bool isRoot() const { return &parent == this; }
  EnvironmentListIter AddSubEnv(std::string_view name) {
    subenvs.emplace_back(*this, name);
    return std::prev(subenvs.end());
  }
  MemoryIter LastLocalAllocation() { return std::prev(local_memory.end()); }
  Environment() = default;  // Creates the root environment.
  Environment(Environment& parent, std::string_view name)
      : parent(parent), name(name) {}
};

class Evaluator {
  Environment& env;

  // Evaluates lines then places the result on the hot memory of the
  // environment.
  void EvaluateToLocal(std::list<IrLine>& lines,
                       std::list<IrLine>::iterator beg,
                       std::list<IrLine>::iterator end) {
    env.local_memory.push_back(Evaluate(lines, beg, end));
  }

 public:
  NativeVariant Evaluate(std::list<IrLine>& lines,
                         std::list<IrLine>::iterator beg,
                         std::list<IrLine>::iterator end) {
    for (std::list<IrLine>::iterator line = beg; line != end;line++) {
      switch (line->op) {
        case eIrOp::DECLARE_VARIABLE: {
          // Arg1: Type Constraint
          if (!std::holds_alternative<IrInt>(line->args[0])) {
            throw std::runtime_error("Expected IrInt for Type Constraint");
          }

          if (!std::holds_alternative<IrString>(line->args[1])) {
            throw std::runtime_error("Expected IrString for variable name");
          }
          // Check if the variable already exists in the current environment.
          auto& var_name = std::get<IrString>(line->args[1]);

          if (env.variables.find(var_name.data()) != env.variables.end()) {
            throw std::runtime_error("Variable already exists");
          }

          // Add the variable to the current environment.
          // Point to sentinel undefined.
          env.variables[var_name.data()] = env.local_memory.begin();
          // Check for an initializer.

          // Next argument is the lines which define the variable.
          if (!std::holds_alternative<IrInt>(line->args[2]) &&
              !std::holds_alternative<IrInt>(line->args[3])) {
            throw std::runtime_error(
                "Expected IrInt for start and end lines of variable "
                "definition.");
          }

          auto def_start_line =
              std::get<IrInt>(line->args[2]);  // Offset from this line.
          auto def_n_lines =
              std::get<IrInt>(line->args[3]);  // Number of lines to consume.

          auto def_it = std::next(line, def_start_line - line->index);
          auto def_it_end = std::next(def_it, def_n_lines);
          EvaluateToLocal(lines, def_it, def_it_end);

          // Point the variable name to the top of the local stack.
          env.variables.at(var_name.data()) = env.LastLocalAllocation();

        } break;
        case eIrOp::DEFINE_VARIABLE: {
          // Set the value of a global variable.
          // There should be 2 arguments.
          if (line->args.size() != 3) {
            throw std::runtime_error(
                "Expected 3 arguments for DEFINE_VARIABLE");
          }

          // Check that the 1st argument is a string.
          if (!std::holds_alternative<IrString>(line->args[0])) {
            throw std::runtime_error("Expected string for variable name");
          }

          // Check that the variable exists.
          auto& var_name = std::get<IrString>(line->args[0]);
          if (!env.variables.contains(var_name.data())) {
            throw std::runtime_error("Variable not found");
          }

          // Next argument is the lines which define the variable.
          if (!std::holds_alternative<IrInt>(line->args[1]) &&
              !std::holds_alternative<IrInt>(line->args[2])) {
            throw std::runtime_error(
                "Expected IrInt for start and end lines of variable "
                "definition.");
          }

          auto def_start_line =
              std::get<IrInt>(line->args[1]);  // Offset from this line.
          auto def_n_lines =
              std::get<IrInt>(line->args[2]);  // Number of lines to consume.

          auto def_it = std::next(line, def_start_line);
          auto def_it_end = std::next(def_it, def_n_lines);
          EvaluateToLocal(lines, def_it, def_it_end);

          // Point the variable name to the top of the local stack.
          env.variables.at(var_name.data()) = env.LastLocalAllocation();

          // Advance line
        } break;
        case eIrOp::ENTER_PROGRAM_DEFINITION:
          // Initialize env, for now do nothing.
          break;
        case eIrOp::ALLOCATE_LITERAL:
          // For now only int.
          // Arg1: Type Constraint
          if (!std::holds_alternative<IrInt>(line->args[0])) {
            throw std::runtime_error("Expected IrInt for Literal Value");
          }
          // Allocate literal value onto local env.
          env.local_memory.push_back(std::get<IrInt>(line->args[0]));
          break;

        case eIrOp::BINARY_ADD:

        default:
          throw std::runtime_error("Unknown operation");
      }
    }
  }

 public:
  Evaluator(Environment& env) : env(env) {}
};

class TheContext {
  Environment global_env;
  Evaluator evaluator{global_env};
};