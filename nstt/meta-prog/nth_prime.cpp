#include <cmath>
#include <iostream>
#include <type_traits>

using std::bool_constant;
using std::conditional_t;
using std::cout;
using std::endl;
using std::integral_constant;

template <int x, int i>
struct Div : bool_constant<(x % i == 0 || Div<x, i - 1>{})> {};

template <int x>
struct Div<x, 1> : bool_constant<false> {};

template <int x>
struct Div<x, 0> : bool_constant<false> {};

template <int x>
struct IsPrime : bool_constant<(x > 1 && !Div<x, x - 1>{})> {};

template <int N, int acc>
struct _NthPrime
    : integral_constant<
          int,
          conditional_t<IsPrime<acc>{},
                        conditional_t<(N == 1), integral_constant<int, acc>,
                                      _NthPrime<N - 1, acc + 1>>,
                        _NthPrime<N, acc + 1>>{}> {};

template <int N>
struct NthPrime
    : integral_constant<int, conditional_t<(N > 0), _NthPrime<N, 2>,
                                           integral_constant<int, -1>>{}> {};

int main() {
    cout << NthPrime<1>::value << endl;
    cout << NthPrime<2>::value << endl;
    cout << NthPrime<3>::value << endl;
    cout << NthPrime<4>::value << endl;
    cout << NthPrime<5>::value << endl;
    cout << NthPrime<6>::value << endl;
    cout << NthPrime<7>::value << endl;
    cout << NthPrime<8>::value << endl;
    cout << NthPrime<9>::value << endl;
    cout << NthPrime<10>::value << endl;
    cout << NthPrime<11>::value << endl;
    cout << NthPrime<12>::value << endl;
    cout << NthPrime<13>::value << endl;
    cout << NthPrime<14>::value << endl;
    cout << NthPrime<15>::value << endl;
    cout << NthPrime<16>::value << endl;
    cout << NthPrime<17>::value << endl;
    cout << NthPrime<18>::value << endl;
    cout << NthPrime<19>::value << endl;
    cout << NthPrime<20>::value << endl;
}
