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
#include <set>

#include "atom.h"

Atom::Atom(int resId, std::string resName, std::string atomName, int atomId) {
    this->resId = resId;
    this->resName = resName;
    this->atomName = atomName;
    this->atomId = atomId;
}
Atom::Atom(int resId, std::string resName, std::string atomName, int atomId,double x, double y, double z) {
    this->resId = resId;
    this->resName = resName;
    this->atomName = atomName;
    this->atomId = atomId;
    this->x = x;
    this->y = y;
    this->z = z;
}