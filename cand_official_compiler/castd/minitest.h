//#include "import_stl.h"
//// CRITICAL: must undefine min and max macros after including Windows.h,
//// or else you cannot use std::min and std::max
//// or std::numeric_limits<T>::min() and std::numeric_limits<T>::max().
//namespace minitest {
//template <auto TestName, auto TestCaseName, auto TestImpl>
//struct Test {
//  static constexpr auto test_name = TestName();
//  static constexpr auto test_case_name = TestCaseName();
//  static inline void Run() {
//    std::cout << "Mini[Test]" << test_name << "[Case]" << test_case_name
//              << std::endl;
//    TestImpl();
//  }
//};
//
//static inline void EXPECT_TRUE(bool b) {
//  if (!b) {
//    std::cout << "[FAIL] Expected TRUE but got FALSE." << std::endl;
//  }
//}
//
//static inline void EXPECT_FALSE(bool b) {
//  if (b) {
//    std::cout << "[FAIL] Expected FALSE but got TRUE." << std::endl;
//  }
//}
//
//template <typename LeT, typename ReT> requires std::equality_comparable_with<LeT, ReT>
//static inline void EXPECT_EQ(LeT lhs, ReT rhs) {
//  if (lhs == rhs) {
//    return;
//  } else {
//    std::cout << "[FAIL] Expected Equality: ";
//    std::cout << lhs << " Got: " << rhs << std::endl;
//  }
//}
//
//template <typename LT, typename RT>
//  requires std::equality_comparable_with<LT, RT>
//static inline void EXPECT_NE(LT a, RT b) {
//  if (a != b) {
//    return;
//  } else {
//    std::cout << "[FAIL] Expected Inequality ";
//    std::cout << a << " and: " << b << std::endl;
//  }
//}
//
//static inline void EXPECT_ANY_THROW(auto&& f) {
//  try {
//    f();
//  } catch (...) {
//    return;
//  }
//  std::cout << "[FAIL] Expected Exception." << std::endl;
//}
//
//}  // namespace minitest
//#define MINITEST(TestName, TestCaseName) \
//  constexpr auto MINITEST_TEST_TestName_TestCaseName = []() -> bool {                       \
//  minitest::Test < []() consteval -> const char* { return #TestName; }, \
//      []() consteval -> const char* { return #TestCaseName; }, []() -> void { using namespace minitest;
//#define END_MINITEST \
//  }                  \
//  > ::Run();         \
//  return true;};
//
//#define RUN_MINITEST(TestName, TestCaseName) \
//  MINITEST_TEST_TestName_TestCaseName();
//
//MINITEST(oo,aa)
//{
//  EXPECT_TRUE(1 == 1);
//  EXPECT_FALSE(1 == 2);
//  EXPECT_EQ(1, 1);
//  EXPECT_NE(1, 2);
//  EXPECT_ANY_THROW([]() { throw 1; });
//  EXPECT_ANY_THROW([]() {});
//}
//END_MINITEST
//
