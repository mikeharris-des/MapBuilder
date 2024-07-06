#pragma once
#include <iostream>
#include <cstdlib>       // for srand
#include <ctime>         // for time
#include <string>
#include <unordered_map> // for hash map
#include <iomanip>       // for setw and setfill

#define DEBUG 0               // alot of printout try with smallest (n x n = 3 x 3) dimension size for map object
#define DEFAULT_ARRAY_SIZE 32 // default size of array backing of collection objects before they need to resize

using namespace std;
