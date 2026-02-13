#ifndef UI_H
#define UI_H

#include "analyzer.h"
#include <string>

void printHeader();
void printResult(const PasswordAnalysis& analysis, const std::string& password);
void clearScreen();

#endif
