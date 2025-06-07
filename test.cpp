#include "vodka-lib/vodka-lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
int main() {
    vector<string> lib={"memory","conversions","math","vodstr"};
    for (auto a:lib) {
        for (auto b:vodka::InternalLibraryFunctions.at(a)) {
            cout<<a+"."+b<<endl;
        }
    }
}