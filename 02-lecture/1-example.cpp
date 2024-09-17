//clang++ 1-example.cpp -std=c++23
#include <cassert>

template <class T1, class T2>
struct Pair {
  template <class U1 = T1, class U2 = T2>
  Pair(U1&&, U2&&) {}
};

struct S { 
  S() = default; 
};

struct E {
  explicit E() = default;
};

int f(Pair<E, S>) {
  return 1;
}

int f(Pair<S, S>) {
  return 2;
}

int main() {
  assert(f({{}, {}}) == 2);
}

/*
===Case description===
First of all static_assrt can't be applied here, because functions are not constexpr.

Program validity:
  - according to 12.1 chapter of ISO f has 2 definitions, because Pair<E, S> and Pair<S, S> are different types

Steps of overload resolution:
  - this is a 12.4.1.1.1 context of resolution:
    "invocation of a function named in the function call syntax"
  - there are 2 viable functions (both have suitable number of parameters, 
    suitable ssociated constraints and each parameter has an implecit conversation 
    that converts that argument to the corresponding parameter of a function)
  - decision which sequence of implicit casts(ICF) is better should be made:
    both sequences have one user-defined cast frominitializer list to class
  - 12.4.3.1.5
*/