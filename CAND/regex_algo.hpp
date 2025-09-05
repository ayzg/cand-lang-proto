#pragma once
#include <type_traits>
#include <ios>
#include <sstream>
#include <iostream>

//struct S2
//{
//	int foo(float x)
//	{
//		return 0;
//	}
//};
//
//template <typename T1, typename T2>
//using my_test_t = decltype(declval<T1>().foo(declval<T2>()));
//
//template <typename T1, typename T2, typename = void>
//struct is_formed : std::false_type
//{
//};
//
//template <typename T1, typename T2>
//struct is_formed<T1, T2, std::enable_if<std::is_same_v<T2,float>>> : std::true_type
//{
//};
//
struct S2
{
	int foo(float x)
	{
		return 0;
	}
};

template <typename T1, typename T2>
using my_test_t = decltype(declval<T1>().foo(declval<T2>()));


template <typename T1, typename T2, typename = std::enable_if<std::is_same<T2, float>::value>>
struct is_formed : std::true_type {};