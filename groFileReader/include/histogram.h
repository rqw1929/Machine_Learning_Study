#pragma once
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include "groFile.h"

class GroFile;
template<typename T> // T should be 1D vectors with double,float,int type
class Histogram {
public:
    void setMax(const std::vector<T> &arr);
    void setMin(const std::vector<T> &arr);
    void setAvg(const std::vector<T> &arr);

    static Histogram getDifferenceHistogram(const std::vector<Histogram> &h1,const std::vector<Histogram> &h2);
    void setBins(const std::vector<T> &arr);

    Histogram(const std::vector<T> &arr,const GroFile& groFile, int numBins=50); // make histogram of averaged by frame numbers
    
    void writeFile(const GroFile &groFile);
    void writeDifferenceFile(const Histogram &h1, const Histogram &h2);

private:
    T min,max;
    T avg;
    int numBins;
    T binStarts;
    T binEnds;
    T binWidth;
    std::vector<int> bins;
    std::vector<T> frequencies;
    

};


template<typename T>
void Histogram<T>::setMax(const std::vector<T> &arr) {
    if (arr.empty()) {
        return;
    }

    max = *std::max_element(arr.begin(),arr.end());
    this->max = max;
}

template<typename T>
void Histogram<T>::setMin(const std::vector<T>& arr) {
   if (arr.empty()) {
       return;
   }
   min = *std::min_element(arr.begin(), arr.end());
   this->min = min;
}

template<typename T>
void Histogram<T>::setAvg(const std::vector<T>& arr) {
   if (arr.empty()) {
       return;
   }
   T sum = std::accumulate(arr.begin(), arr.end(), T());
   avg = sum / arr.size();
   this->avg = avg;
}

template<typename T>
Histogram<T>::Histogram(const std::vector<T> &arr, const GroFile& groFile, int numBins) { //averaged by frame numbers
    if (arr.empty() || numBins <= 0) {
        return;
    }

    // Set basic statistics
    setMin(arr);
    setMax(arr);
    setAvg(arr);
    

    // Calculate bin width
    binWidth = (max - min) / static_cast<T>(numBins);
    std::cout << "Min:" << min << " Max: " << max << " BinWidth: " << binWidth << std::endl;
    // Initialize bins vector with zeros
    bins.resize(numBins, 0);

    // Populate bins
    for (const T& value : arr) {
        int binIndex = static_cast<int>((value - min) / binWidth);
        // Handle edge case for maximum value
        if (binIndex == numBins) {
            binIndex--;
        }
        bins[binIndex]++;
    }

    // Calculate frequencies
    frequencies.resize(numBins);
    for (int i = 0; i < numBins; i++) {
        frequencies[i] = static_cast<double>(bins[i]) ; //averaged by frame #
    }
    this->numBins = numBins;
}

template<typename T>
void Histogram<T>::writeFile(const GroFile& groFile) {
    std::string trajPath = groFile.trajFileName;
    size_t prefixEnd = trajPath.find("/md/");

    if (prefixEnd != std::string::npos) {
       trajPath = trajPath.substr(prefixEnd + 4); // Skip "/md/"
    }

    // Replace remaining '/' with '.'
    std::replace(trajPath.begin(), trajPath.end(), '/', '.');

    // Open output file using processed name
    std::ofstream outFile(trajPath + "_hist.txt");
    if (!outFile) {
        throw std::runtime_error("Cannot open file for writing: " + trajPath + "_hist.txt");
    }

    outFile << "# Bin_Start Bin_End Count\n";
    
    
    for (int i = 0; i < numBins; i++) {
        T binStart = min + (i * binWidth);
        T binEnd = binStart + binWidth;
        outFile << std::fixed << std::setprecision(6) 
                << binStart << " " 
                << binEnd << " " 
                << frequencies[i] << "\n";
    }
    std::cout << "Output File: " << trajPath + "_hist.txt" << std::endl;
    outFile.close();
}