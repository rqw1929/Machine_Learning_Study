#pragma once
#include <string>

class Atom {
public:
    int resId;
    std::string resName;
    std::string atomName;
    int atomId;
    double x, y, z;
    float charge;

    Atom(int resId, std::string resName, std::string atomName, int atomId);
    Atom(int resId, std::string resName, std::string atomName, int atomId,double charge); // declare with charge
    Atom(int resId, std::string resName, std::string atomName, int atomId,double x,double y,double z);
    Atom(int resId, std::string resName, std::string atomName, int atomId,double x,double y,double z,double charge);
    
};