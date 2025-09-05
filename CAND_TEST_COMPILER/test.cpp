#include "pch.h"
#include "../CAND/candi_token.h"
#include "../CAND/regex_algo.hpp"
TEST( Operand_Integer , Tokens ) {
	int test_val = 42;

	EXPECT_FALSE(is_formed<S2, double>::value);
	EXPECT_FALSE(is_formed<S2, double>::value);
	EXPECT_TRUE(is_formed<S2, float>::value);


	////Initialize empty candi integer
	//CandiLanguage::Integer i;	

	//i.set_data(42); // Set value
	//EXPECT_EQ(test_val, i.get_data() ); //Get value

	////Initialize to value
	//CandiLanguage::Integer i_v(test_val);
	//EXPECT_EQ(test_val, i_v.get_data()); 

	////Copy constructor
	//CandiLanguage::Integer i_c(i);
	//EXPECT_EQ(test_val, i_c.get_data());

	////Move constructor
	//CandiLanguage::Integer i_m(std::move(i));
	//EXPECT_EQ(test_val, i_m.get_data());
}