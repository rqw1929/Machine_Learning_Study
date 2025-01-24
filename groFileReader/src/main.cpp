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
#include "frame.h"
#include "calculation.h"
#include "histogram.h"


int main() {
    std::vector<double> ohBondLengths;
    
    std::string trajFilePath = "/home/hurriup/backyard/md/bulk/TIP4P2005f/npt/pure/10Acutoff/trajectory/traj.gro";
    GroFile testGro(trajFilePath);
    
    
    
    for (int i=0; i < testGro.numFrames;i++) {
        Frame currentFrame(testGro,i);
        std::vector<double> currentFrameOHBondLengths =  (currentFrame.getOHBondLengths());
        ohBondLengths.insert(ohBondLengths.end(),currentFrameOHBondLengths.begin(),currentFrameOHBondLengths.end());
        
    }
    std::cout << ohBondLengths.size() << std::endl;
    double avg = std::accumulate(ohBondLengths.begin(),ohBondLengths.end(),0.0) / static_cast<double>(ohBondLengths.size());
    std::cout << avg << std::endl;
    
    if (ohBondLengths.size() == 0) {
        std::cout << "No OH Bonds" << std::endl;
    }
    Histogram<double> hist(ohBondLengths,testGro,50);
    hist.writeFile(testGro);

    return 0;
}