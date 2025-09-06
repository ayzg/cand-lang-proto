
//=-------------------------------------------------------------------------=//
//---------------------------------------------------------------------------//
// All Rights Reserved | Copyright 2024 NTONE INC.
// Author: Anton Yashchenko
// Email: ntondev@gmail.com
// Website: https://www.bigteeny.com
//---------------------------------------------------------------------------//
// Project: caoco
// Directory: castd
// File: expected.h
//---------------------------------------------------------------------------//
// Brief: Structures for handling expected values and errors.
//---------------------------------------------------------------------------//
#ifndef HEADER_GUARD_CAOCO_CASTD_EXPECTED_H
#define HEADER_GUARD_CAOCO_CASTD_EXPECTED_H
// Includes:
#include "import_stl.h"

//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//

class BoolError {
  bool valid_;
  std::string error_;

 public:
  BoolError(std::string error_message)
      : valid_(false), error_(error_message) {}
  BoolError(const char* error_message)
      : valid_(false), error_(error_message) {}
  BoolError() : valid_(true), error_("") {}
  BoolError(bool value) : valid_(value) {
    if (value)
      error_ = "";
    else
      error_ = "BoolError: Unspecified false error.";
  }
  constexpr bool Valid() const { return valid_; }
  constexpr const std::string& Error() const { return error_; }
  constexpr operator bool() const { return valid_; }
};

template <typename T>
class Expected {
  std::optional<T> expected_{std::nullopt};
  std::string error_{""};

  constexpr Expected(T expected) : expected_(expected) {}
  template <typename T>
  constexpr Expected(T&& expected) : expected_(expected) {}
  template <typename T>
  constexpr Expected(const T& expected) : expected_(expected) {}

  #pragma warning(disable : 4100) // Disable unused parameter warning for std::nullopt_t
                                  // Cannot be instantiated directly from type.
  constexpr Expected(const std::nullopt_t& none, std::string error_message)
      : expected_(std::nullopt), error_(error_message) {}
  #pragma warning(default : 4100) 
 public:
  constexpr bool Valid() const { return expected_.has_value(); }
  constexpr T Extract() {
    T val = std::move(expected_.value());
    expected_.reset();
    error_ = "Value has been moved out of the expected object.";
    return val;
  }
  constexpr const T& Value() const { return expected_.value(); }
  constexpr const auto& Error() const { return error_; }
  constexpr operator bool() const { return expected_.has_value(); }
  static constexpr Expected<T> Success(const T& expected) {
    return Expected<T>(std::forward<const T>(expected));
  }
  static constexpr Expected<T> Success(T&& expected) {
    return Expected<T>(std::forward<T>(expected));
  }
  static constexpr Expected<T> Failure(const std::string& error_message) {
    return Expected<T>(std::nullopt,
                       std::forward<const std::string>(error_message));
  }
  static constexpr Expected<T> Failure(std::string&& error_message) {
    return Expected<T>(std::nullopt, std::forward<std::string>(error_message));
  }

  constexpr Expected<T> ChainFailure(std::string error_message) {
    return Expected::ChainFailure(*this, error_message);
  }
  static constexpr Expected<T> ChainFailure(const Expected<T>& other,
                                            const std::string& error_message) {
    auto ret = Expected<T>::Failure(other.error_ + "\n" + error_message);
    return ret;
  }
};

template <typename T, typename AlwaysT>
class PartialExpected {
  AlwaysT always_;
  std::optional<T> value_{std::nullopt};
  std::string error_{""};

  template <typename T, typename AlwaysT>
  constexpr PartialExpected(const AlwaysT& always, T expected)
      : always_(always), value_(expected) {}

  template <typename AlwaysT>
  constexpr PartialExpected(const AlwaysT& always) : always_(always) {}

 public:
  constexpr bool Valid() const { return value_.has_value(); }

  constexpr const T& Value() const { return value_.value(); }

  constexpr const AlwaysT& Always() const { return always_; }

  constexpr const std::string& Error() const { return error_; }
  constexpr operator bool() const { return value_.has_value(); }
  constexpr T Extract() {
    T val = std::move(value_.value());
    value_.reset();
    error_ = "Value has been moved out of the expected object.";
    return val;
  }

  constexpr PartialExpected<T, AlwaysT> ChainFailure(
      std::string error_message) {
    return PartialExpected::ChainFailure(*this, error_message);
  }

  static constexpr PartialExpected<T, AlwaysT> Success(const AlwaysT& always,
                                                       T expected) {
    return PartialExpected(always, expected);
  }

  static constexpr PartialExpected<T, AlwaysT> Failure(
      const AlwaysT& always, const std::string& error_message) {
    auto ret = PartialExpected(always);
    ret.error_ = error_message;
    return ret;
  }

  static constexpr PartialExpected<T, AlwaysT> ChainFailure(
      const PartialExpected& other, const AlwaysT& new_always,
      const std::string& error_message) {
    auto ret = PartialExpected::Failure(new_always,
                                        other.error_ + "\n" + error_message);
    return ret;
  }

  static constexpr PartialExpected<T, AlwaysT> ChainFailure(
      const PartialExpected& other, const std::string& error_message) {
    auto ret = PartialExpected::Failure(other.Always(),
                                        other.error_ + "\n" + error_message);
    return ret;
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
// Directory: castd
// File: expected.h
//---------------------------------------------------------------------------//
#endif HEADER_GUARD_CAOCO_CASTD_EXPECTED_H
//---------------------------------------------------------------------------//
//=-------------------------------------------------------------------------=//
