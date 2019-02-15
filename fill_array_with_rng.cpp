/*
Requires C++17 because of std::void_t
But you can define your own void_t, see: https://en.cppreference.com/w/cpp/types/void_t

This file presents a `fillRnd(container, type_of_container_element min, type_of_container_element max, int seed)`
function which can fill any itereble numerical container with random elements using mt19937
See mersenne twister engine: https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine
*/

#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <random>
#include <type_traits>
#include <vector>

template <typename T, typename = void>
struct is_iterable : std::false_type {};
template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>> : std::true_type{};

/**
  Fills the given container with random values generated with mt19937
  
  @param arr Any iterable and numerical container (checked with is_iterable and std::is_arithmetic).
  @param min Minimum random value, type is deducted from passed container.
  @param max Maximum random value, type is deducted from passed container.
  @param seed (int) Seed for rng.
*/
template<typename T, typename = std::enable_if<
    std::is_arithmetic<typename T::value_type>::value &&
    is_iterable<T>::value, 
    void>>
void fillRnd(T &arr, typename T::value_type min, typename T::value_type max, int seed=1337) {
    
    std::mt19937 generator(seed);
    
    using value_type = typename T::value_type;
    
    //disType should be uniform_int_distribution if our container contains only integral types
    //and uniform_real_distribution for floating_point types
    using disType = typename std::conditional<
                                std::is_integral<value_type>::value, 
                                std::uniform_int_distribution<value_type>, 
                                std::uniform_real_distribution<value_type>
                            >::type;
    
    disType dis(min, max);

    std::generate(arr.begin(), arr.end(), [&](){ return dis(generator); } );
    
    return;
}

//Nice output for vectors
template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
    if ( !v.empty() ) {
        out << '[';
        std::copy (v.begin(), v.end() -1, std::ostream_iterator<T>(out, ", "));
        out << v.back() << "]";
    }
    return out;
}

//Nice output for arrays
template <typename T, size_t SIZE>
std::ostream& operator<< (std::ostream& out, const std::array<T, SIZE> &a) {
    if ( !a.empty() ) {
        out << '[';
        std::copy (a.begin(), a.end() -1, std::ostream_iterator<T>(out, ", "));
        out << a.back() << "]";
    }
    return out;
}

int main() {
    //testing vector with int
    std::vector<int> v(10);
    fillRnd(v, 1, 10, 42);
    std::cout << v << std::endl;
    
    //testing array with int
    std::array<int, 10> a;
    fillRnd(a, 1, 10, 43);
    std::cout << a << std::endl;
    
    //testing array with 1 element and float
    std::array<float, 1> b;
    fillRnd(b, 1, 100);
    std::cout << b << std::endl;
}
