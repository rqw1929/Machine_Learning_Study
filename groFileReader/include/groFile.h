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

#include "atom.h"
#include "molecule.h"
#include "frame.h"
#include "calculation.h"
#include "box.h"

class GroFile { // traj.gro 파일에서 전반적인 정보 불러들이기, numFrame 읽는 것과 numAtoms 읽는게 중요
public:
    
    double timestep;
    double startTime,endTime;
    int numFrames;
    int numAtoms;
    int linesPerFrame;
    std::map<std::string, int> atomInfo; // key: atomName, value: atom count
    std::map<std::string, int> molInfo; // key: resName, value: residue 개수 
    std::string trajFileName;
    std::string title;
    std::string waterModel; //TIP4P2005f / SPCFw
    std::vector<std::streampos> framePositions;

    Box box;
    GroFile(std::string trajFileName);
    
    void setAtomInfo(const std::vector<Atom>& Atoms); // Atom type별 개수 찾기, Map 형식으로 ㄱㄱ
    const std::vector<Molecule> assignMolecules(const std::vector<Atom>& Atoms); // 같은 residue name 가지는 atom끼리 molecule로 묶어서 molecules로 return Frame에서도 상속하면 좋을듯
    void setMolInfo(const std::vector<Molecule>& Molecules); //Map 기능 확실히 알아두기
    void setBondInfo();
    void loadTimestep();
    void setLinePos(); // Frame별 시작하는 라인 잡아두기





    double getTimestep() const;
    double getStartTime() const;
    double getEndTime() const;
    int getNumFrames() const;
    int getNumAtoms() const;
    std::string getTrajFileName() const;
    std::string getTitle() const;
    std::string getWaterModel() const;
    std::map<std::string, int> getAtomInfo() const;
    std::map<std::string, int> getMolInfo() const;

    void setTimestep(double ts);
    void setStartTime(double st);
    void setEndTime(double et);
    void setNumFrames(int nf);
    void setNumAtoms(int na);
    void setTrajFileName(const std::string& fn);
    void setTitle(const std::string& t);
    void setWaterModel(const std::string& wm);
private:
};
