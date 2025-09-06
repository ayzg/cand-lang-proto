//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: castd
// File: ref_count.h
//---------------------------------------------------------------------------//
// Brief:
//
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_CASTD_DYNAMIC_PTR_H
#define HEADER_GUARD_CAOCO_CASTD_DYNAMIC_PTR_H
// Includes:
#include "import_stl.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

using VoidPtr = void*;

template <class T>
class DynPtr {
  T* ptr_{nullptr};

 public:
  constexpr DynPtr() = default;
  constexpr DynPtr(const DynPtr&) = delete;
  constexpr DynPtr(DynPtr&&) = default;
  constexpr DynPtr& operator=(const DynPtr&) = delete;
  constexpr DynPtr& operator=(DynPtr&&) = default;

  DynPtr(T* ptr) : ptr_(ptr) {}

  T& operator->() { return *ptr_; }
  T& operator*() { return *ptr_; }
  T* Get() { return ptr_; }

  void Reset(T* ptr) { ptr_ = ptr; }
  void Reset() { ptr_ = nullptr; }
};

class UniqueVoidPtr {
 public:
  using void_unique_ptr = std::unique_ptr<void, void (*)(void*)>;
  struct UnknownType {};  // default type for unique_ptr
 private:
  template <class T>
  static constexpr std::unique_ptr<void, void (*)(void*)> MakeStdVoidUptr(
      T* ptr) {
    return {ptr, [](void* ptr) { delete static_cast<T*>(ptr); }};
  }

  void_unique_ptr ptr_{MakeStdVoidUptr(new UnknownType())};

 public:
  constexpr UniqueVoidPtr() = default;
  constexpr UniqueVoidPtr(const UniqueVoidPtr&) = delete;
  constexpr UniqueVoidPtr& operator=(const UniqueVoidPtr&) = delete;
  constexpr UniqueVoidPtr& operator=(UniqueVoidPtr&&) = default;

  constexpr UniqueVoidPtr(UniqueVoidPtr&& other) {
    ptr_ = MakeStdVoidUptr(std::move(other.ptr_.get()));
  }

  template <class T>
  constexpr UniqueVoidPtr(T* ptr) : ptr_(MakeStdVoidUptr(ptr)) {}

  template <class T>
  constexpr UniqueVoidPtr(T&& val)
      : ptr_(MakeStdVoidUptr(new T(std::forward<T>(val)))) {}

  template <class T>
  void Reset(T* ptr) {
    ptr_.reset(ptr);
  }

  bool IsNull() { return ptr_.get() == nullptr; }

  void Reset() { ptr_.reset(); }

  VoidPtr Ptr() const { return ptr_.get(); }

  template <class T>
  DynPtr<T> GetAs() {
    return static_cast<T*>(ptr_.get());
  }
};

//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: cand_official_compiler
// Directory: castd
// File: ref_count.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_CASTD_DYNAMIC_PTR_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
