#pragma once
#include <unordered_map>
#include "global_typedef.hpp"

#include <any>
// We will transpile the ast to C++ code
// Sample code:
/*
#enter{
    #type(int,&int);
    #vari(a,int); // Variable of type int
    #vari(b@); // variable of type auto

    #method(add,#type(int),{lhs,rhs},{#type(int),#type(int)}); // Method of type int, with 2 int parameters

    #call(add,1,2); // Call method add with 2 parameters ERROR, cant call method returning int into void

    #vari(c@#call(add,1,2)); // ERROR, method body not defined

    // Able to give body to method at a later point.
    #method(add)(
        #return(lhs+rhs);
    );

    #vari(c@#call(add,1,2)); // OK, returns 3, type is the common type of lhs and rhs, which is int

    #class(car)(
		#vari(speed,int);
        #static#const#vari(max_speed,int,100);

        #static#method(get_max_speed@)(#return(max_speed);); // @ is the auto type, deduced from the return type which will be a const int 
                                                             // You could also return a #ref(max_speed) but it is not necessary for literal values. const int ref

		#method(speed_up)(
            #if(speed<max_speed)(
                #return(#ref(speed++))
            );

			#return(#ref(speed));
		);

        #method(turbo)(
           #while(speed<max_speed)(
            // When using call not in a variable definition and you wish to send the result to a variable
            // you must use the #into keyword, a special operator which tells the compiler to store the result in a memory location
			// whitespace is not necessary, but is used for readability
            // &self is a special keyword which is a pointer to the current most inner scope object, this being the return value of the speed_up method
            // which is a reference to the member variable speed, in case of pushing a reference into itself is the same as returning void.
            #call(speed_up) #into &self;
		);
    ) #type(car,#class(car));

    #vari(my_car,#type(car));

    #call(my_car,speed_up); // ERROR, cant return #ref(#type(int)) into void.
    #vari(curr_speed@#call(my_car,speed_up)); // OK, when first argument to #call is a non method variable, 
                                              // it is assumed to be the this pointer of a member method
    // Sample of using a static method.
    #vari(max_speed@#call(#type(car),get_max_speed)); // OK, returns 100
    #vari(max_speed@#call(#class(car),get_max_speed)); // Use a direct #class object call instead of a #type identity, same meaning (unless type car has other types)
    //#vari(max_speed@#call(my_car,get_max_speed)); // ERROR, cant call static method on non static object

    #print(curr_speed == max_speed); // Prints true because we used turbo to go to max speed.

}
#start{
}
*/


template<typename T = void>
struct ViperVari {
    std::any value_ptr_{};
    auto& value_ref() {
		return std::any_cast<T&>(value_ptr_);
	}

    const auto& value_ref() const {
        return std::any_cast<const T &>(value_ptr_);
    }

    ViperVari() {
        value_ptr_ = std::any(T{});
    }

    ViperVari(T value) {
		value_ptr_ = std::any(value);
	}
};

template<>
struct ViperVari<void> {
    std::any value_ptr_{};
};

using viper_type_int = int;

ViperVari<viper_type_int> add(ViperVari<viper_type_int> lhs, ViperVari<viper_type_int> rhs) {
	return (lhs.value_ref() + rhs.value_ref());
}

auto VIPER_MAIN() {
    ViperVari<viper_type_int> a; // int type
    ViperVari b;                 // auto type

    return 0;
}



ViperVari c = add(1, 2); // Okay auto type deduced to int

class car {
public: // Everything is public by default
    ViperVari<viper_type_int> speed;
    static const ViperVari<viper_type_int> max_speed; // according to C++ standard, static const variables must be defined outside of the class
													  // so we will define below.

    static ViperVari<viper_type_int> get_max_speed() {
		return max_speed;
	}

    ViperVari<viper_type_int>& speed_up() {
		if (speed.value_ref() < max_speed.value_ref()) {
			speed.value_ref()++;
		}
		return speed;
	}

	void turbo() {
		while (speed.value_ref() < max_speed.value_ref()) {
			speed_up(); // #into &self taking effect here
		}
	}
};
const ViperVari<viper_type_int> car::max_speed = 100;

// defining my car
using viper_type_car = car; // car registered as a type

ViperVari<viper_type_car> my_car;

ViperVari<viper_type_int> curr_speed = my_car.value_ref().speed_up(); 

// use turbo to go to max speed
auto void_call_turbo = []() {my_car.value_ref().turbo(); return NULL; }();

ViperVari<viper_type_int> max_speed = car::get_max_speed(); // static method call

// print curr_speed == max_speed
auto void_call_print = []() {printf("%d", curr_speed.value_ref() == max_speed.value_ref()); return NULL; }();

