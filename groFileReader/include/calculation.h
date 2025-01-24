#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <limits>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <regex>
#include "frame.h"
#include "atom.h"
#include "box.h"

const float NM_TO_ANGSTROM = 10.0;

class Calculation {
public:
    static double calculateDistance(const Atom& a1,const Atom& a2,const Box& box);
    

};


