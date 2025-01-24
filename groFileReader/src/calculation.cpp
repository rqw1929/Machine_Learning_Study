#include "calculation.h"


double Calculation::calculateDistance(const Atom& a1,const Atom& a2,const Box& box) {
    
    double dx = a1.x - a2.x;
    double dy = a1.y - a2.y;
    double dz = a1.z - a2.z;
    
    dx -= box.X * round(dx/box.X);
    dy -= box.Y * round(dy/box.Y);
    dz -= box.Z * round(dz/box.Z);
    
    return sqrt(dx*dx + dy*dy + dz*dz) * NM_TO_ANGSTROM;
    
}