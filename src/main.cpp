#include <iostream>

#include "components/HelloWorldComponent/hello-world-component.h"

int main() {
    HelloWorldComponent component;

    component.greet("World");
    component.greet("C++");
    component.greet("CMake");

    return 0;
}