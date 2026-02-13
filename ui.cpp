#include "ui.h"
#include <iostream>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

void clearScreen() {
    std::system(CLEAR);
}

void printHeader() {
    std::cout << "\033[1;36m========================================\033[0m\n";
    std::cout << "\033[1;33m     PASSWORD STRENGTH ANALYZER v1.0\033[0m\n";
    std::cout << "\033[1;36m========================================\033[0m\n\n";
}

void printResult(const PasswordAnalysis& a, const std::string& pwd) {
    std::string color;
    if (a.category == "Weak")   color = "\033[1;31m";
    else if (a.category == "Medium") color = "\033[1;33m";
    else color = "\033[1;32m";

    std::cout << color << "Strength: " << a.category << " (" << a.strengthScore << "/100)\033[0m\n";
    std::cout << "Password length: " << pwd.length() << " characters\n";
    std::cout << "Entropy: " << std::fixed << std::setprecision(1) << a.entropy << " bits\n";
    std::cout << "Estimated crack time: " << a.crackTime << "\n\n";

    std::cout << "\033[1;37mSuggestions:\033[0m\n" << a.suggestions << "\n\n";

    std::cout << "\033[90m(press Enter to analyze another, or Ctrl+C to quit)\033[0m\n";
}
