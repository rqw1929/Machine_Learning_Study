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

#include "calculation.h"
#include "atom.h"
#include "molecule.h"
#include "groFile.h"

Frame :: Frame(GroFile& groFile,int currentFrame) {
    
    std::ifstream file(groFile.trajFileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << groFile.trajFileName << std::endl;
        return;
    }

    file.seekg(groFile.framePositions[currentFrame]); // Jumping to corresponding file positions
    
    this->currentFrame = currentFrame;
    
    //this->currentTime = groFile.startTime + groFile.timestep * currentFrame;
    
    std::string line;
    std::getline(file,line);
    std::getline(file, line); //skip first two lines

    std::vector<Atom> atoms;
    
    
    

    int numAtoms = groFile.numAtoms;
    int linesPerFrame = groFile.linesPerFrame;

    for (int i = 0; i < numAtoms; i++) { // Assigning Atoms
        std::getline(file, line); linesPerFrame++;
        if (line.length() > 45) {
            std::cerr << "Error: Invalid line format at atom " << i + 1 << std::endl;
            return;
        }
        

        // initializing atom info from gromacs trajectory file
        int resId = std::stoi(line.substr(0, 5));
        std::string resName = line.substr(5, 5);
        resName.erase(remove(resName.begin(), resName.end(), ' '), resName.end());    
        std::string atomName = line.substr(10, 5);
        atomName.erase(remove(atomName.begin(), atomName.end(), ' '), atomName.end());  
        int atomId = std::stoi(line.substr(15, 5));
        float x = std::stof(line.substr(20, 8));
        float y = std::stof(line.substr(28, 8));
        float z = std::stof(line.substr(36, 8));
        Atom atom(resId,resName,atomName,atomId,x,y,z);
        atoms.push_back(atom);
    }
    
    
    //reading box dimensions

    std::getline(file,line);
    std::stringstream ss(line);
    if (!(ss >> this->box.X >> this->box.Y >> this->box.Z)) {
        std::cerr << "Error: Invalid box dimensions: " << line << std::endl;
        return ;
    }
    
    molecules = Frame::assignMolecules(atoms); // Frame의 assignMolecules들을 써서 box를 frame 껄로 얻는다
    
    this->atoms = atoms;

    
    
    for (auto& mol: molecules) {
        mol.assignBond(box);
        for (const auto& bond: mol.bonds) {
            if (bond.bondLength < 0.71) {
                std::cout << "Frame : " << currentFrame << "resId: " << mol.resId << "" << std::endl; 
            }
        }
    }
}

std::vector<double> Frame::getOHBondLengths() { // 나중에 Topology로 부터 Atom Type 받는 방식으로 고칠 것
    std::vector<double> ohBondLengths;
    for (const auto& mol: molecules) {
        std::cout <<mol.bonds.at(0).bondLength << std::endl;
        if (mol.bonds.size() >0) {
            for (const auto& bond: mol.bonds){ // 순서 체크
                if  (
                    mol.atoms.at(*bond.bondPair.begin()).atomName == "OW1" &&
                    (   mol.atoms.at(*std::prev(bond.bondPair.end())).atomName == "HW2" 
                     || mol.atoms.at(*std::prev(bond.bondPair.end())).atomName == "HW3"
                    )
                    )
                    ohBondLengths.push_back(bond.bondLength);

                    if (mol.resId == 1239 && currentFrame==194) {
                        std::cout << mol.bonds.at(0).bondLength << " " << mol.bonds.at(1).bondLength << std::endl;
                        std::cout << mol.atoms.at(0).x << " " << mol.atoms.at(0).y << " " <<  mol.atoms.at(0).z << std::endl;
                        std::cout << mol.atoms.at(1).x << " " << mol.atoms.at(1).y << " " <<  mol.atoms.at(1).z << std::endl;
                        std::cout << mol.atoms.at(2).x << " " << mol.atoms.at(2).y << " " <<  mol.atoms.at(2).z << std::endl;
                        std::cout << box.X << " " << box.Y << " " << box.Z << std::endl;
                        std::cout << Calculation::calculateDistance(mol.atoms.at(0),mol.atoms.at(1),box) << std::endl;
                    }
            }
        }
    }

    return ohBondLengths;
}

void Frame::analyzeBondInfo() { // For analyzing molecules in Frame
    for (const auto& mol: molecules) {
        for (const auto& bond : mol.bonds) {    
            if (mol.bonds.size() >0) {
                auto &bondPair = bond.bondPair;
                std::cout << mol.atoms.at(*bondPair.begin()).atomName << "-" << mol.atoms.at(*std::prev(bondPair.end())).atomName
                << " " << bond.bondLength << " A" << std::endl;
            }
        }
    }
    
}


const std::vector<Molecule> Frame :: assignMolecules(const std::vector<Atom> &Atoms) {
    std::vector<Molecule> Molecules;
    std::map<int, std::vector<Atom>> AtomMolMap;
    
    

    for (const auto& atom : Atoms) {
        
        AtomMolMap[atom.resId] = std::vector<Atom>();
    }
    
    for (const auto& atom : Atoms) {
        AtomMolMap[atom.resId].push_back(atom);
    }
    for (const auto& [resId, atoms] : AtomMolMap) {
        if (!atoms.empty()) {
            Molecule mol(atoms, box);
            Molecules.push_back(mol);
        }
    }
    ;
    
    return Molecules;
}