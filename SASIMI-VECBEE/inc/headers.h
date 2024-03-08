#ifndef HEADERS_H
#define HEADERS_H

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#ifdef __cpp_lib_filesystem
#include <filesystem>
#else
#include <experimental/filesystem>
namespace std {
namespace filesystem = experimental::filesystem;
}
#endif
#include <experimental/filesystem>
#include <iostream>
#include <limits.h>
#include <list>
#include <random>
#include <set>
#include <sstream>
#include <stdint.h>
#include <unordered_map>

#define IN
#define OUT
#define INOUT

#endif
