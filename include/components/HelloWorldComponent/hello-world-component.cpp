#include <iostream>
#include <string>

#include "hello-world-component.h"

// utils
#include "utils/PrintingUtility/printing-utility.h"



void HelloWorldComponent::greet(const std::string& name) const {
    PrintingUtility util;
    util.print("Hello, " + name + "!");
}