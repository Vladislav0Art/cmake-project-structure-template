#include <iostream>
#include <string>

#include "hello-world-component.h"


void HelloWorldComponent::greet(const std::string& name) const {
    std::string msg = "Hello, " + name + "!";
    std::cout << msg << "\n";
}