// for header-only libraries

#include <iostream>


namespace mylib {

class MyLib {
public:
    int sum(int a, int b) const noexcept {
        return a + b;
    }
};

} // mylib