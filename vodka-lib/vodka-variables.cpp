#include "vodka-lib.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <filesystem>
using namespace std;
//* Convert VariableDatatype object to string
string vodka::variables::datatype_to_string(VariableDatatype datatype) {
    switch (datatype) {
        case VariableDatatype::vodint:return "vodint";
        case VariableDatatype::vodec:return "vodec";
        case VariableDatatype::vodarg:return "vodarg";
        default:return "Unknown";
    }
}