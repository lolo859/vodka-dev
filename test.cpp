#include "vodka-lib/vodka-lib.h"
#include <iostream>
#include <string>
int main() {
    auto a=vodka::utilities::split("vodka  ","  ");
    cout<<a.size()<<endl;
    return 1;
}