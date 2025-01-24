#pragma once
#include <vector>
#include <string>
#include <set>

#include "atom.h"
#include "box.h"

class Molecule {
public:
    struct Bond {
        std::set<int> bondPair;
        double bondLength;
    };
    struct Angle { //later implementation

    };
    struct Dihedral { // later implementation

    };
    struct Improper { // later implementation

    };

    int resId;
    std::string resName;
    std::vector<Atom> atoms;

    std::vector<Bond> bonds;
    
    Molecule(std::vector<Atom> Atoms, Box box);

    bool hasBond(int idx1, int idx2) const;
    void addBond(int idx1, int idx2,Box box);
    void assignBond(Box box);
};
