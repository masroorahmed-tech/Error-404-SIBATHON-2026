#ifndef ANALYZER_H
#define ANALYZER_H

#include <string>

struct PasswordAnalysis {
    std::string category;
    double entropy;
    std::string crackTime;
    std::string suggestions;
    int strengthScore;  // 0–100
};

PasswordAnalysis analyzePassword(const std::string& password);
std::string generateStrongPassword(int length = 16);

#endif
