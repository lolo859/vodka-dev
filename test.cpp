#include <iostream>
#include "vodka-lib/vodka-lib.h"
#include <termcolor.hpp>
using namespace std;
int main() {
    vector<string> source={
        "vodka>vodka-lib>vodka-utilities>vodka-lib::utilities::error",
        "vodka>vodka-lib>vodka-utilities>inside_utilities::split",
        "standard>std::string::this_is_a_int"
    };
    vodka::utilities::error("not a string","test.cpp",{"int hello=8"});
}