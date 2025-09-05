#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include "token.hpp"
#include <numeric>

template <typename T>
concept OutputStringStreamable_ = requires (T t,std::stringstream ss){
    { ss << t };
};


template <typename T>
concept InputStringStreamable_ = requires (T t,std::stringstream ss){
    { ss >> t };
};


static void to_stdio(const OutputStringStreamable_ auto & string_convertible){
    std::cout << string_convertible;
}

static void from_stdio(InputStringStreamable_ auto & string_convertible){
    std::cin >> string_convertible;
}

static void from_stdio(std::stringstream & input_stream){
    std::string tempstring;
    std::cin >> tempstring;
    input_stream << tempstring;
}



struct candil_console {
    std::string author_name;
    std::stringstream user_command_stream;
    std::istringstream input_stream;
    std::ostringstream output_stream;

    candil_console() {
        to_stdio( "Welcome to the CANDIL-Console ! \n Please enter your name(no spaces, alphanumeric, ?underscores):");
        from_stdio(author_name);
        to_stdio("[" + author_name + "]: ");
        from_stdio(user_command_stream);
        to_stdio(user_command_stream.str());

    }
};

void copy_and_advance(auto begin,auto end, auto & it,auto advance){
    std::copy(begin,end,it);
    std::advance(it,advance);
}

void move_and_advance(auto begin,auto end, auto & it,auto advance){
    std::move(begin,end,it);
    std::advance(it,advance);
}

template<typename T, std::size_t N1,std::size_t N2>
auto merge(const std::array<T,N1> & first,const std::array<T,N2> & second){
    std::array<T,N1+N2> ret_array;
    auto ret_it = ret_array.begin();
    std::copy(first.cbegin(),first.cend(),ret_it);
    std::advance(ret_it,N1);
    std::copy(second.cbegin(),second.cend(),ret_it);
    return ret_array;
}

template<typename T, std::size_t N1,std::size_t N2>
auto merge(std::array<T,N1> && first,std::array<T,N2> && second){
    std::array<T,N1+N2> ret_array;
    auto ret_it = ret_array.begin();
    std::move(first.cbegin(),first.cend(),ret_it);
    std::advance(ret_it,N1);
    std::move(second.cbegin(),second.cend(),ret_it);
    return ret_array;
}

template<typename T, std::size_t ...Ns>
auto merge_all(const std::array<T,Ns> & ...arrays){
    std::array<T,7> ret_array;
    auto ret_it = ret_array.begin();
    std::apply([&ret_it](auto&&... args) {
        ((copy_and_advance(args.cbegin(),args.cend(),ret_it,args.size()), ...));
    }, std::make_tuple(arrays...));

    return ret_array;
}

template<typename T, std::size_t ...Ns>
auto merge_all(std::array<T,Ns> && ...arrays){
    std::array<T,7> ret_array;
    auto ret_it = ret_array.begin();
    std::apply([&ret_it](auto&&... args) {
        ((move_and_advance(args.cbegin(),args.cend(),ret_it,args.size()), ...));
    }, std::make_tuple(arrays...));

    return ret_array;
}

template<typename T>
constexpr auto merge(const std::vector<T> & first,const std::vector<T> & second){
    std::vector<T> ret_array;
    ret_array.resize(first.size() + second.size());
    auto ret_it = ret_array.begin();
    std::copy(first.cbegin(),first.cend(),ret_it);
    std::advance(ret_it,first.size());
    std::copy(second.cbegin(),second.cend(),ret_it);
    return ret_array;
}

template<typename T>
constexpr auto merge(std::vector<T> && first,std::vector<T> && second){
    std::vector<T> ret_array;
    ret_array.resize(first.size() + second.size());
    auto ret_it = ret_array.begin();
    std::move(first.cbegin(),first.cend(),ret_it);
    std::advance(ret_it,first.size());
    std::move(second.cbegin(),second.cend(),ret_it);
    return ret_array;
}



template<typename T,template<typename V,typename ...Vs> class VectorT,template<typename U,typename ...Us> class ...VectorTs>
constexpr auto merge_all(const VectorT<T> & first_vector ,const VectorTs<T> & ...vectors){
    VectorT<T> ret_array;
    ret_array.resize(first_vector.size() + ({(vectors.size() + ...);}));
    auto ret_it = ret_array.begin();
    std::apply([&ret_it](auto&&... args) {
        ((copy_and_advance(args.cbegin(),args.cend(),ret_it,args.size()), ...));
    }, std::make_tuple(first_vector,vectors...));
    return ret_array;
}

template<typename T,template<typename U,typename ...Us> class VectorT,template<typename V,typename ...Vs> class ...VectorTs>
constexpr auto merge_all(VectorT<T> && first_vector ,VectorTs<T> && ...vectors) {
    VectorT<T> ret_array;
    ret_array.resize(first_vector.size() + ({ (vectors.size() + ...); }));
    auto ret_it = ret_array.begin();
    std::apply([&ret_it](auto &&... args) {
        ((move_and_advance(args.cbegin(), args.cend(), ret_it, args.size()), ...));
    }, std::make_tuple(first_vector, vectors...));
    return ret_array;
}

template<auto idx,auto N,typename T>
constexpr auto split(const std::array<T,N> & array){
    static_assert(idx < N, "Cannot split array beyond N size index.");
    std::array<T,idx> array1;
    std::array<T,N - idx> array2;
    auto idx_it = array.begin();
    std::advance(idx_it,idx);
    std::copy(array.begin(),idx_it,array1.begin());
    std::copy(idx_it,array.end(),array2.begin());
    return std::make_tuple(array1,array2);
}

template<typename T>
constexpr auto split(const std::vector<T> & vector,auto idx){
    static_assert(idx < vector.size(),"Cannot split array beyond N size index.");
    std::vector<T> array1;
    array1.resize(idx);
    std::vector<T> array2;
    array2.resize(vector.size() - idx);
    auto idx_it = vector.begin();
    std::advance(idx_it,idx);
    std::copy(vector.begin(),idx_it,array1.begin());
    std::copy(idx_it,vector.end(),array2.begin());
    return std::make_tuple(array1,array2);
}



int main() {
    /*
    std::array<float,3> array1{1,2,3};
    std::array<float,4> array2{5,6,7,8};
    std::array<float,6> array3;

    std::vector<float> vector1;
    std::vector<float> vector2;

    auto splited = split<1>(array1);

    auto merge_vec = merge(vector1,vector2);
    auto merge_vecmore = merge_all(vector1,vector2,vector2);
    auto merged = merge(array1,array2);
    auto merged2 = merge_all(array1,array2,array2);
    for(auto n : merged2){
        std::cout << n << std::endl;
    }
*/


    return 0;
}
