//
// Created by Dmitriy Steshenko on 28.04.2020.
//

#ifndef FINALPROJECTCPP_INCLUDELIB_H
#define FINALPROJECTCPP_INCLUDELIB_H

// JSON
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace pt = boost::property_tree;
extern pt::ptree root;

// UUID
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
using namespace boost::uuids;

// Tokenizer
#include <boost/tokenizer.hpp>
typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

// Other
#include <iostream>
#include <clocale>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <list>
#include <string>

using namespace std;

#endif //FINALPROJECTCPP_INCLUDELIB_H