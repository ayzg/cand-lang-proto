//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: cand_lang
// File: identifier_table.h
//---------------------------------------------------------------------------//
// Brief:
// IdentityTable: Reponsible for creating and storing identifiers.
//                Each identifier has:
//                  - a name
//                  - a scope index
//                  - an identity category
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_CAND_LANG_IDENTIFIER_TABLE_H
#define HEADER_GUARD_CAOCO_CAND_LANG_IDENTIFIER_TABLE_H
// Includes:
#include "cand_syntax.h"
#include "import_stl.h"
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

using String = std::string;

using IndexT = std::size_t;
using CeString = std::string;
using CeVoidPtr = void*;
using VoidPtr = void*;
using CeScopeIndex = std::size_t;
using CeTypeIndex = std::size_t;

struct CandTypes {
  using Str = std::string;
  class None;
  class Undefined;
  class Method;
  class Object;
};

template <class T, std::size_t Index>
struct TypeIndex {
  using type = T;
  static constexpr std::size_t index = Index;
};

namespace Native {
using Int = TypeIndex<int, 0>;
using Uint = TypeIndex<unsigned, 1>;
using Real = TypeIndex<double, 2>;
using Bool = TypeIndex<bool, 3>;
using Byte = TypeIndex<unsigned char, 4>;
using Str = TypeIndex<std::string, 5>;
using None = TypeIndex<CandTypes::None, 6>;
using Undefined = TypeIndex<CandTypes::Undefined, 7>;
using CandMethod = TypeIndex<CandTypes::Method, 8>;
using CandObject = TypeIndex<CandTypes::Object, 9>;

}  // namespace Native
using NativeVariant =
    std::variant<Native::Int::type, Native::Uint::type, Native::Real::type,
                 Native::Bool::type, Native::Byte::type, Native::Str::type,
                 Native::None::type, Native::Undefined::type,
                 Native::CandMethod::type, Native::CandObject::type>;


class CandTypes::None {
  bool operator==(const None&) const { return true; }
};
class CandTypes::Undefined {
  bool operator==(const Undefined&) const { return true; }
};
class CandTypes::Method {
  const String& name_;
  const CeScope& evalenv_;
  const CeIdentityTable& args_;
  const Ast & body_;

};
class CandTypes::Object {
  const String & name_;
  const CeScope & evalenv_;
  const CeIdentityTable & members_;
};



class UniqueVoidPtr {
  using StdVoidUptr = std::unique_ptr<void, void (*)(void*)>;
  template <class T>
  StdVoidUptr MakeStdVoidUptr(T* ptr) {
    return {ptr, [](void* ptr) { delete static_cast<T*>(ptr); }};
  }
  StdVoidUptr ptr_;

 public:
  UniqueVoidPtr() = default;
  UniqueVoidPtr(const UniqueVoidPtr&) = delete;
  UniqueVoidPtr(UniqueVoidPtr&&) = default;
  UniqueVoidPtr& operator=(const UniqueVoidPtr&) = delete;
  UniqueVoidPtr& operator=(UniqueVoidPtr&&) = default;

  template <class T>
  UniqueVoidPtr(T* ptr) : ptr_(MakeStdVoidUptr(ptr)) {}

  template <class T>
  void Reset(T* ptr) {
    ptr_.reset(ptr);
  }

  bool IsNull() {
    ptr_.get() == nullptr;
  }

  void Reset() { ptr_.reset(); }

  VoidPtr Ptr() const { return ptr_.get(); }

  template<class T>
  T* GetAs() {
    return static_cast<T*>(ptr_.get());
  }
};

struct CeValue {
  std::size_t type;
  NativeVariant value;

  CeValue() : type(Native::None::index), value(Native::None::type{}) {}

  template <typename T, std::size_t Index>
  CeValue(TypeIndex<T, Index> type_idx)
      : type(type_idx::index), value(type_idx::type()) {}

  CeValue(const CeValue& other) {
    type = other.type;
    value = other.value;
  }

  CeValue(CeValue&& other) noexcept {
    type = other.type;
    value = std::move(other.value);
  }

  CeValue& operator=(CeValue&& other) noexcept {
    type = other.type;
    value = other.value;
    return *this;
  }

  CeValue& operator=(const CeValue& other) {
    type = other.type;
    value = other.value;
    return *this;
  }
};

enum class eIdentityCategory : std::size_t {
  kUnknown,
  kVariable,
  kMethod,
  kObject,
  kNamespace,
  kLibrary,
  kMain,
};

struct CeIdentity {
  CeString name;
  eIdentityCategory category;
  CeScopeIndex scope_index;
};

class CeIdentityTable {
  using OptIdentityConstRef =
      std::optional<std::reference_wrapper<const CeIdentity>>;
  std::set<CeIdentity> table_;

  inline OptIdentityConstRef MakeIdRef(const std::string& name,
                                       eIdentityCategory category,
                                       std::size_t scope) const {
    return std::cref(*table_.find(CeIdentity{name, category, scope}));
  }

 public:
  bool Contains(const std::string& name, eIdentityCategory category,
                std::size_t scope) const {
    return table_.contains(CeIdentity{name, category, scope});
  }

  OptIdentityConstRef Get(const std::string& name, eIdentityCategory category,
                          std::size_t scope) const {
    if (not Contains(name, category, scope)) {
      // Element does not exist.
      return std::nullopt;
    } else {
      return MakeIdRef(name, category, scope);
    }
  }

  OptIdentityConstRef Add(const std::string& name, eIdentityCategory category,
                          std::size_t scope) {
    auto inserted = table_.insert(CeIdentity{name, category, scope});
    if (not inserted.second) {
      // Element already exists.
      return std::nullopt;
    } else {
      // Return inserted element reference
      return std::cref(*table_.find(CeIdentity{name, category, scope}));
    }
  }

  void Remove(const std::string& name, eIdentityCategory category,
              std::size_t scope) {
    table_.erase(CeIdentity{name, category, scope});
  }

  void Clear() { table_.clear(); }

  bool IsEmpty() { return table_.empty(); }
};

struct CeTypeDescriptor {
  CeString name;
  CeScopeIndex scope_index;
  CeTypeIndex type_index;

  bool operator<(const CeTypeDescriptor& other) const {
    return std::tie(name, scope_index) <
           std::tie(other.name, other.scope_index);
  }
};

class CeTypeTable {
  using OptTypeDescConstRef =
      std::optional<std::reference_wrapper<const CeTypeDescriptor>>;

 private:
  std::set<CeTypeDescriptor> table_;

  inline OptTypeDescConstRef MakeTypeConstRef(const std::string& name,
                                              CeScopeIndex scope) const {
    return std::cref(*table_.find(CeTypeDescriptor(name, scope)));
  }

 public:
  bool Contains(const std::string& name, CeScopeIndex scope) const {
    return table_.contains(CeTypeDescriptor(name, scope));
  }

  OptTypeDescConstRef Get(const std::string& name, CeScopeIndex scope) const {
    auto found = table_.find(CeTypeDescriptor{name, scope});
    if (found == table_.end()) {
      return std::nullopt;
    } else {
      return std::cref(*found);
    }
  }

  OptTypeDescConstRef Add(const std::string& name, CeScopeIndex scope,
                          CeTypeIndex type_index) {
    auto inserted = table_.insert(CeTypeDescriptor{name, scope, type_index});
    if (inserted.second == false) {
      return std::nullopt;
    } else {
      return MakeTypeConstRef(name, scope);
    }
  }

  OptTypeDescConstRef Replace(const std::string& name, CeScopeIndex scope,
                              CeTypeIndex type_index) {
    auto found = table_.find(CeTypeDescriptor{name, scope});
    if (found == table_.end()) {
      return std::nullopt;
    } else {
      auto extracted = table_.extract(found);
      extracted.value().type_index = type_index;
      table_.insert(std::move(extracted));
      return Get(name, scope);
    }
  }

  OptTypeDescConstRef Alias(const std::string& name, CeScopeIndex scope,
                            const std::string& alias_name,
                            CeScopeIndex alias_scope) {
    auto found = Get(name, scope);
    if (!found.has_value()) {
      return std::nullopt;
    } else {
      if (Contains(alias_name, alias_scope)) {
        return std::nullopt;
      } else {
        auto inserted = table_.insert(
            CeTypeDescriptor(alias_name, alias_scope, found->get().type_index));
        return std::cref(*(inserted.first));
      }
    }
  }

  void Remove(const std::string& name, CeScopeIndex scope) {
    table_.erase(CeTypeDescriptor{name, scope, 0});
  }

  void Clear() { table_.clear(); }

  bool IsEmpty() const { return table_.empty(); }
};

struct CeVariableInstance {
  CeString name;
  CeScopeIndex scope_index;
  CeTypeIndex type_index;
  std::list<UniqueVoidPtr>::iterator memory_location;
};

struct CeScope {
  CeString name;
  CeScopeIndex index;
  CeScopeIndex parent_index;
};

struct CeMethod {
  CeString name;
  CeScopeIndex scope_index;
  CeTypeIndex return_type_index;
  std::vector<CeTypeIndex> parameter_type_indices;
  std::vector<CeString> parameter_names;
  Ast body;
};

class CeEnvironment {
  CeIdentityTable identity_table_;
  CeTypeTable type_table_;
  std::list<CeScope> scopes_;
  std::list<CeVariableInstance> variables_;
  std::list<UniqueVoidPtr> memory_;
};

class ConstEvaluator {
  CeEnvironment Evaluate(const Ast& ast) {
    CeEnvironment env;

    return env;
  }
};

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
//  All Rights Reserved | Copyright 2024 NTONE INC.
//  Author: Anton Yashchenko
//  Email: ntondev@gmail.com
//  Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
//  Project: caoco
//  Directory: cand_lang
//  File: identifier_table.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_CAND_LANG_IDENTIFIER_TABLE_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//