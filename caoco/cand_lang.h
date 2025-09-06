//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: cand_lang
// File: cand_lang.h
//---------------------------------------------------------------------------//
// Brief: {description}
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_CAND_LANG_CAND_LANG_H
#define HEADER_GUARD_CAOCO_CAND_LANG_CAND_LANG_H
// Includes:
#include "cand_syntax.h"
#include "rt_val.h"
#include "dynamic_ptr.h"
#include "import_stl.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

using std::shared_ptr;
using std::size_t;
using std::string;
using std::weak_ptr;

template <class T, size_t Index>
struct CompileTimeTypeIndex {
  using type = T;
  static constexpr size_t idx = Index;
};

template <class T>
using ReferenceObject = shared_ptr<T>;

class CandObject;
class CandMethod;
class CandNone {
  bool operator==(const CandNone&) const { return true; }
};
class CandUndefined {
  bool operator==(const CandUndefined&) const { return true; }
};

struct RtVal {
  // Native types
  using Int = CompileTimeTypeIndex<int, 0>;
  using Unsigned = CompileTimeTypeIndex<unsigned, 1>;
  using Double = CompileTimeTypeIndex<double, 2>;
  using Bool = CompileTimeTypeIndex<bool, 3>;
  using Byte = CompileTimeTypeIndex<unsigned char, 4>;
  using Char = CompileTimeTypeIndex<char, 5>;
  using String = CompileTimeTypeIndex<ReferenceObject<string>, 6>;
  using None = CompileTimeTypeIndex<CandNone, 7>;
  using Undefined = CompileTimeTypeIndex<CandUndefined, 8>;
  using Method = CompileTimeTypeIndex<ReferenceObject<CandMethod>, 9>;
  using Object = CompileTimeTypeIndex<ReferenceObject<CandObject>, 10>;

  using IntT = typename Int::type;
  using UnsignedT = typename Unsigned::type;
  using DoubleT = typename Double::type;
  using BoolT = typename Bool::type;
  using ByteT = typename Byte::type;
  using CharT = typename Char::type;
  using StringT = typename String::type;
  using NoneT = typename None::type;
  using UndefinedT = typename Undefined::type;
  using MethodT = typename Method::type;
  using ObjectT = typename Object::type;

  static constexpr inline auto kInt = Int::idx;
  static constexpr inline auto kUnsigned = Unsigned::idx;
  static constexpr inline auto kDouble = Double::idx;
  static constexpr inline auto kBool = Bool::idx;
  static constexpr inline auto kByte = Byte::idx;
  static constexpr inline auto kChar = Char::idx;
  static constexpr inline auto kString = String::idx;
  static constexpr inline auto kNone = None::idx;
  static constexpr inline auto kUndefined = Undefined::idx;
  static constexpr inline auto kMethod = Method::idx;
  static constexpr inline auto kObject = Object::idx;

  using NativeVariant =
      std::variant<IntT, UnsignedT, DoubleT, BoolT, ByteT, CharT, StringT,
                   NoneT, UndefinedT, MethodT, ObjectT>;

  int type_ = kUndefined;
  NativeVariant value{UndefinedT()};

 private:
  // Get value as a copy.
  // Note that strings, objects and methods are returned as ref objects.
  // As such a copy is not made, only the reference count is incremented.
  template <class TYPE>
  auto InternalGetValue() {
    return std::get<TYPE>(value);
  }

 public:
  constexpr IntT GetInt() const { 
    return std::visit([](auto&& arg) -> IntT {
      using T = std::decay_t<decltype(arg)>;
      if constexpr (std::is_same_v<T, IntT>) {
        return arg;
      } else {
        throw "GetInt: Requested value is not an integer.";
      }
    }, value); 
  }
  UnsignedT GetUnsigned() const { return std::get<UnsignedT>(value); }
  DoubleT GetDouble() const { return std::get<DoubleT>(value); }
  BoolT GetBool() const { return std::get<BoolT>(value); }
  ByteT GetByte() const { return std::get<ByteT>(value); }
  CharT GetChar() const { return std::get<CharT>(value); }
  StringT GetString() const { return std::get<StringT>(value); }
  NoneT GetNone() const { return std::get<NoneT>(value); }
  UndefinedT GetUndefined() const { return std::get<UndefinedT>(value); }
  MethodT GetMethod() const { return std::get<MethodT>(value); }
  ObjectT GetObject() const { return std::get<ObjectT>(value); }

  // Binary Operators
  constexpr RtVal& AddOp(const RtVal& other) {
    // Apply addition operation to equivalent types.
    RtVal&& result = std::visit(
        [&other](auto&& arg) -> RtVal&& {
        using T = std::decay_t<decltype(arg)>;
          try {
            if constexpr (std::is_same_v<T, IntT>) {
              arg += other.GetInt();
            } else if constexpr (std::is_same_v<T, UnsignedT>) {
              arg += other.GetUnsigned();
            } else if constexpr (std::is_same_v<T, DoubleT>) {
              arg += other.GetDouble();
            } else if constexpr (std::is_same_v<T, ByteT>) {
              arg += other.GetByte();
            } else if constexpr (std::is_same_v<T, CharT>) {
              arg += other.GetChar();
            } else if constexpr (std::is_same_v<T, StringT>) {
              arg = StringT(new string(*arg + *other.GetString()));
            } else {
              throw "Value type does not implement an addition operation.";
            }
          } catch (const std::exception& e) {
            string err = e.what();
            err += "\nInvalid argument type for addition operation.";
            throw err.c_str();
          }
        },
        value);

    // Move the result into this object. Type stays the same.
    value = std::move(result.value);

    // Return reference to this object.
    return *this;
  }

  RtVal& SubOp(const RtVal& other) {
    switch (type_) {
      case kInt:
        return RtVal(GetInt() - other.GetInt());
      case kUnsigned:
        return RtVal(GetUnsigned() - other.GetUnsigned());
      case kDouble:
        return RtVal(GetDouble() - other.GetDouble());
      case kByte:
        return RtVal(GetByte() - other.GetByte());
      case kChar:
        return RtVal(GetChar() - other.GetChar());
      default:
        throw "Invalid types for subtraction operation.";
    }
  }
  RtVal& MulOp(const RtVal& other) {
    switch (type_) {
      case kInt:
        return RtVal(GetInt() * other.GetInt());
      case kUnsigned:
        return RtVal(GetUnsigned() * other.GetUnsigned());
      case kDouble:
        return RtVal(GetDouble() * other.GetDouble());
      case kByte:
        return RtVal(GetByte() * other.GetByte());
      case kChar:
        return RtVal(GetChar() * other.GetChar());
      default:
        throw "Invalid types for multiplication operation.";
    }
  }
  RtVal& DivOp(const RtVal& other) {
    switch (type_) {
      case kInt:
        return RtVal(GetInt() / other.GetInt());
      case kUnsigned:
        return RtVal(GetUnsigned() / other.GetUnsigned());
      case kDouble:
        return RtVal(GetDouble() / other.GetDouble());
      case kByte:
        return RtVal(GetByte() / other.GetByte());
      case kChar:
        return RtVal(GetChar() / other.GetChar());
      default:
        throw "Invalid types for division operation.";
    }
  }
  RtVal& ModOp(const RtVal& other) const {
    switch (type_) {
      case kInt:
        return RtVal(GetInt() % other.GetInt());
      case kUnsigned:
        return RtVal(GetUnsigned() % other.GetUnsigned());
      case kByte:
        return RtVal(GetByte() % other.GetByte());
      case kChar:
        return RtVal(GetChar() % other.GetChar());
      default:
        throw "Invalid types for modulo operation.";
    }
  }

  // Fast Unary Operators
  RtVal& NegOp() {
    switch (type_) {
      case kInt:
        return RtVal(-GetInt());
      case kUnsigned:
        return RtVal(-GetUnsigned());
      case kDouble:
        return RtVal(-GetDouble());
      case kByte:
        return RtVal(-GetByte());
      case kChar:
        return RtVal(-GetChar());
      default:
        throw "Invalid types for negation operation.";
    }
  }
  RtVal& NotOp() {
    switch (type_) {
      case kBool:
        return RtVal(!GetBool());
      default:
        throw "Invalid types for negation operation.";
    }
  }
  RtVal& IncrementOp() const {
    switch (type_) {
      case kInt:
        return RtVal(GetInt() + 1);
      case kUnsigned:
        return RtVal(GetUnsigned() + 1);
      case kByte:
        return RtVal(GetByte() + 1);
      case kChar:
        return RtVal(GetChar() + 1);
      default:
        throw "Invalid types for increment operation.";
    }
  }
  RtVal& DecrementOp() const {
    switch (type_) {
      case kInt:
        return RtVal(GetInt() - 1);
      case kUnsigned:
        return RtVal(GetUnsigned() - 1);
      case kByte:
        return RtVal(GetByte() - 1);
      case kChar:
        return RtVal(GetChar() - 1);
      default:
        throw "Invalid types for decrement operation.";
    }
  }

  constexpr RtVal() = default;
  constexpr RtVal(size_t native_index) : type(type) {
    switch (type) {
      case kInt:
        value = IntT();
        break;
      case kUnsigned:
        value = UnsignedT();
        break;
      case kDouble:
        value = DoubleT();
        break;
      case kBool:
        value = BoolT();
        break;
      case kByte:
        value = ByteT();
        break;
      case kChar:
        value = CharT();
        break;
      case kString:
        value = StringT();
        break;
      case kNone:
        value = NoneT();
        break;
      case kUndefined:
        value = UndefinedT();
        break;
      case kMethod:
        value = MethodT();
        break;
      case kObject:
        value = ObjectT();
        break;
      default:
        throw "Invalid type.";
    }
  }
  constexpr RtVal(NativeVariant native_variant) {
    value = native_variant;
    switch (value.index()) {
      case kInt:
        type = kInt;
        break;
      case kUnsigned:
        type = kUnsigned;
        break;
      case kDouble:
        type = kDouble;
        break;
      case kBool:
        type = kBool;
        break;
      case kByte:
        type = kByte;
        break;
      case kChar:
        type = kChar;
        break;
      case kString:
        type = kString;
        break;
      case kNone:
        type = kNone;
        break;
      case kUndefined:
        type = kUndefined;
        break;
      case kMethod:
        type = kMethod;
        break;
      case kObject:
        type = kObject;
        break;
      default:
        break;
    }
  }

  // Create an undefined value.
  static constexpr inline auto NewUndefined() { return RtVal{}; }
};

enum class eNameCategory { kVar, kFunction, kClass };
static const RtVal kRuntimeUndefined = {RtVal::Undefined::idx};
// - Memory management of the C& Runtime.
class RuntimeEnv {
  using MemoryLocation = std::list<RtVal>::iterator;
  RuntimeEnv* parent_;
  std::list<RuntimeEnv> children_;
  std::list<RtVal> memory_;
  std::map<std::string, std::tuple<eNameCategory, MemoryLocation>> definitions_;

 public:
  void setParentEnv(RuntimeEnv* env) { parent_ = env; }
  RuntimeEnv& Subenv() {
    children_.push_back(RuntimeEnv{});
    children_.back().parent_ = this;
    return children_.back();
  }

  // Define a new variable in the current scope.
  void Define(const std::string& name, eNameCategory name_category,
              const RtVal& value) {
    memory_.push_back(value);
    definitions_[name] =
        std::make_tuple(name_category, std::prev(memory_.end()));
  }

  RtVal& RetrieveLocal(const std::string& name, eNameCategory name_category) {
    auto& [type, val] = definitions_[name];
    if (type == name_category) {
      return *val;
    } else {
      throw "Does not exist.";
    }
  }

  RtVal& RetrieveGlobal(const std::string& name, eNameCategory name_category) {
    if (ContainsLocal(name, name_category)) {
      auto& [type, val] = definitions_[name];
      return *val;
    } else if (ContainsGlobal(name, name_category)) {
      return parent_->RetrieveGlobal(name, name_category);
    } else {
      throw "Does not exist.";
    }
  }

  bool ContainsLocal(const std::string& name, eNameCategory name_category) {
    auto found = definitions_.contains(name);
    if (found) {
      if (std::get<0>(definitions_[name]) == name_category) {
        return true;
      }
    }
    return false;
  }

  bool ContainsGlobal(const std::string& name, eNameCategory name_category) {
    if (ContainsLocal(name, name_category)) {
      return true;
    } else if (parent_ != nullptr) {
      return parent_->ContainsGlobal(name, name_category);
    } else {
      return false;
    }
  }

  void Flush() {
    memory_.clear();
    definitions_.clear();
    children_.clear();
  }
};

struct CandObject {
  RuntimeEnv local_env;  // local encapsulated env for object containing member
                         // vars and methods.
  RuntimeEnv& object_env;  // static object env, created once per object type
                           // linked to the global env.
  ReferenceObject<CandMethod> default_constructor;
  ReferenceObject<CandMethod> default_destructor;

  // Default constructor. Will create an empty object.
  CandObject(RuntimeEnv& this_object_env) : object_env(this_object_env) {}

  // Constructor with constructor and destructor. This will create a new
  // instance of a 'type'. Static members will not be initialized here. The
  // actual construct is not called here. Use 'Construct' method to create an
  // instance.
  CandObject(RuntimeEnv& this_object_env, ReferenceObject<CandMethod> ctor,
             ReferenceObject<CandMethod> dtor)
      : object_env(this_object_env),
        default_constructor(ctor),
        default_destructor(dtor) {}

  // Construct a new instance of the object using the default constructor.
  void Construct() const {
    if (default_constructor != nullptr) {
      // for now, constructors do not return anything.
      [mabye_unused] RtVal none_result =
          default_constructor->call({}, &local_env).GetResultAndFlush();
    }
    // Else object is empty.
  }

  // Destruct the object.
  void Destruct() const {
    if (default_destructor != nullptr) {
      // for now, destructors do not return anything.
      [mabye_unused] RtVal none_result =
          default_destructor->call({}, &object_env).GetResultAndFlush();
    }
  }

  // Call a static method of the object.
  RtVal CallStaticMethod(const std::string& method_name,
                         std::vector<RtVal*> var_args) {
    auto& method =
        object_env.RetrieveLocal(method_name, eNameCategory::kFunction);
    return method.Get<RtVal::MethodT>()
        ->call(var_args, &object_env)
        .GetResultAndFlush();
  }

  // Get a static member variable of the object.
  RtVal GetStaticMember(const std::string& member_name) {
    return object_env.RetrieveLocal(member_name, eNameCategory::kVar);
  }

  // Set a member variable of the object.
  void AddStaticMember(const std::string& member_name, const RtVal& value) {
    object_env.Define(member_name, eNameCategory::kVar, value);
  }

  // Add a static method to the object.
  void AddStaticMethod(const std::string& method_name,
                       ReferenceObject<CandMethod> method) {
    object_env.Define(method_name, eNameCategory::kFunction, RtVal{method});
  }
};

class CandMethod {
  RuntimeEnv local_env_;
  std::vector<std::string> args_;
  Ast body_;
  RtVal result;

 public:
  CandMethod(std::vector<std::string> args) : args_(args) {}

  RtVal&& GetResultAndFlush() {
    RtVal&& val = std::move(result);
    local_env_.Flush();
    return std::move(val);
  }

  CandMethod& call(std::vector<RtVal*> var_args, RuntimeEnv* context) {
    // set parent of local env.
    local_env_.setParentEnv(context);

    // Based on args_, create local vars, get value from args...
    for (auto index = 0; const auto& a : args_) {
      local_env_.Define(a, eNameCategory::kVar, *var_args[index]);
      index++;
    }

    // Evaluate body of func and store in result.
    // result = Runtime::Evaluate(body_, local_env_);
    // TEMP: function is returning 1 arg passed to it and adding 1.
    auto& arg1 = local_env_.RetrieveLocal(args_[0], eNameCategory::kVar);
    // add op
    if (arg1.type == RtVal::Int::idx) {
      auto& v = arg1.Get<RtVal::IntT>();
      v += 100;
      arg1.value = v;
    }

    // return op
    result = arg1;
    return *this;
  }
};

// C& Intermediate Language
//

// C& Runtime: Responsible for
//             - executing op codes,
class Runtime {
 public:
  static RtVal Evaluate(const Ast& ast, RuntimeEnv& env) {
    return RtVal();  // temp
  }
};

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: cand_lang
// File: cand_lang.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_CAND_LANG_CAND_LANG_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//