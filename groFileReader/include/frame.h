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
#include <set>


#include "groFile.h"
#include "box.h"
#include "atom.h"
#include "molecule.h"

class GroFile;
class Molecule;
class Frame;

class Frame { // Inherits GroFile Information
public:
    int currentFrame;
    double currentTime;
    Box box;

    int currentLinePos; // 파일 내 라인 위치

    std::vector<Molecule> molecules;
    std::vector<Atom> atoms;
    Frame(GroFile &groFile,int currentFrame);
    std::vector<double> getOHBondLengths();
    void analyzeBondInfo();
    const std::vector<Molecule> assignMolecules(const std::vector<Atom> &Atoms);
private:

};


