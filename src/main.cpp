#include <iostream>

#include "components/HelloWorldComponent/hello-world-component.h"
#include "mylib/mylib.hpp"

int main() {
    HelloWorldComponent component;

    component.greet("World");
    component.greet("C++");
    component.greet("CMake");

    mylib::MyLib lib;
    std::cout << lib.sum(1, 3) << "\n";

    return 0;
}