
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
#include "molecule.h"
#include "groFile.h"
#include "frame.h"
#include "calculation.h"


int countLines(const std::string& filename) {
    std::ifstream file(filename);
    return std::count(std::istreambuf_iterator<char>(file), 
                 std::istreambuf_iterator<char>(), '\n') + 1;
}


GroFile::GroFile(std::string trajFileName) {
    this->trajFileName = trajFileName;
    int linesPerFrame = 0;
    std::ifstream file(trajFileName);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << trajFileName << std::endl;
        return;
    }
    std::streampos pos = file.tellg();
    

    std::vector<std::streampos> framePositions;
    std::getline(file, this->title); linesPerFrame++; // title을 포인터로 활용할 수 있는지?
    
    

    std::string numAtomsStr; 
    std::getline(file, numAtomsStr); linesPerFrame++;
    this->numAtoms = std::stoi(numAtomsStr);
    

    std::vector<Atom> atoms;
    std::vector<Molecule> mols;
    std::string line;
    
    for (int i = 0; i < numAtoms; i++) {
        std::getline(file, line); linesPerFrame++;
        if (line.length() > 45) {
            std::cerr << "Error: Invalid line format at atom " << i + 1 << std::endl;
            return;
        }
        

        int resId = std::stoi(line.substr(0,5));
        
        std::string resName = line.substr(5, 5);
        resName.erase(remove(resName.begin(), resName.end(), ' '), resName.end());    
        
        std::string atomName = line.substr(10, 5);
        atomName.erase(remove(atomName.begin(), atomName.end(), ' '), atomName.end());  
        
        int atomId = std::stoi(line.substr(15, 5));
        double x = std::stof(line.substr(20, 8));
        double y = std::stof(line.substr(28, 8));
        double z = std::stof(line.substr(36, 8));
        
        
        Atom atom(resId, resName, atomName, atomId, x, y, z);
        
        atoms.push_back(atom);
    }
    
    mols = GroFile::assignMolecules(atoms); 
    GroFile::setAtomInfo(atoms);
    setMolInfo(mols); // molecular name, count info


    std::getline(file,line); linesPerFrame++; // discarding boxline
    std::stringstream ss(line);

    if (!(ss >> this->box.X >> this->box.Y >> this->box.Z)) {
        std::cerr << "Error: Invalid box dimensions: " << line << std::endl;
        return ;
    }
    assignMolecules(atoms);
    file.close();



    std::ifstream forCountingLines(trajFileName);
    if (!forCountingLines.is_open()) {
        std::cerr << "Error: Could not open file " << trajFileName << std::endl;
        return;
    }
    
    int totalLines = countLines(this->trajFileName);
    this->linesPerFrame = linesPerFrame;
    this->numFrames = totalLines / linesPerFrame;
    
    
    forCountingLines.close();


    std::ifstream forIndexingLinePos(trajFileName);
    if (!forIndexingLinePos.is_open()) {
        std::cerr << "Error: Could not open file " << trajFileName << std::endl;
        return;
    }
    int count = 0;
    
    do  {
        if (count % linesPerFrame == 0) { 
            framePositions.push_back(forIndexingLinePos.tellg());
        }
        count++;
    }while(std::getline(forIndexingLinePos,line));
;
    this->framePositions = framePositions;

    std::cout << "Atom #: " << atoms.size() << std::endl;
    std::cout << "Molecule #: " << mols.size() << std::endl;
    
}

const std::vector<Molecule> GroFile :: assignMolecules(const std::vector<Atom> &Atoms) {
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

void GroFile::setAtomInfo(const std::vector<Atom> & Atoms) {
   atomInfo.clear();
   
   // Count atoms from constructor's atoms vector
   for (const auto& atom : Atoms) {
       atomInfo[atom.atomName]++;
   }
}

void GroFile::setMolInfo(const std::vector<Molecule>& Molecules) {

   molInfo.clear(); // Clear existing map
   
   // Count occurrences of each resName
   for (const auto& mol : Molecules) {
       molInfo[mol.resName]++; // Increment count for this resName
   }
}






double GroFile::getTimestep() const { return timestep; }
double GroFile::getStartTime() const { return startTime; }
double GroFile::getEndTime() const { return endTime; }
int GroFile::getNumFrames() const { return numFrames; }
int GroFile::getNumAtoms() const { return numAtoms; }
std::string GroFile::getTrajFileName() const { return trajFileName; }
std::string GroFile::getTitle() const { return title; }
std::string GroFile::getWaterModel() const { return waterModel; }
std::map<std::string, int> GroFile::getAtomInfo() const { return atomInfo; }
std::map<std::string, int> GroFile::getMolInfo() const { return molInfo; }

void GroFile::setTimestep(double ts) { timestep = ts; }
void GroFile::setStartTime(double st) { startTime = st; }
void GroFile::setEndTime(double et) { endTime = et; }
void GroFile::setNumFrames(int nf) { numFrames = nf; }
void GroFile::setNumAtoms(int na) { numAtoms = na; }
void GroFile::setTrajFileName(const std::string& fn) { trajFileName = fn; }
void GroFile::setTitle(const std::string& t) { title = t; }
void GroFile::setWaterModel(const std::string& wm) { waterModel = wm; }