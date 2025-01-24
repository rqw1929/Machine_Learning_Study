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
#include "calculation.h"
#include "molecule.h"
class Calculation;
class Molecule;



Molecule::Molecule(std::vector<Atom> Atoms,Box box) {
    if (!Atoms.empty()) {
        this->resId = Atoms[0].resId;
        this->resName = Atoms[0].resName;
        this->atoms = Atoms;
        
    }
    this->assignBond(box);
}

bool Molecule::hasBond(int idx1, int idx2) const {
   if (atoms.empty()) return false;
   
   return std::any_of(bonds.begin(), bonds.end(),
       [idx1, idx2](const Bond& b) {
           return b.bondPair == std::set<int>{idx1, idx2};
       });
}

void Molecule::addBond(int idx1, int idx2,Box box) {
   
   
   if (hasBond(idx1, idx2)) return;
   
   Bond newBond;
   newBond.bondPair = std::set<int>{idx1, idx2};

    if (resId == 1239 && box.X < 4.022995 && box.X > 4.022985) {
        std::cout << "Box Length: " << box.X << std::endl;
        std::cout << "Indexing: " << atoms.at(idx1).atomName << " " << atoms.at(idx1).atomId << " " << atoms.at(idx2).atomName << " " << atoms.at(idx2).atomId;
    }

   newBond.bondLength = Calculation::calculateDistance(atoms.at(idx1),atoms.at(idx2),box);
   // Calculate bond length here if needed
   bonds.push_back(newBond);
}

void Molecule::assignBond(Box box) { // for TIP4P/2005f water oxygen {OW1 / HW2, HW3}
    if (this->atoms.empty()) return;
    if (this->atoms.size()==1) return;
    
    if (this->atoms.at(0).atomName == "OW1") {
        Molecule::addBond(0,1,box);
        Molecule::addBond(0,2,box);
    }



}
