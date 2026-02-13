#include "analyzer.h"
#include "ui.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (true) {
        clearScreen();
        printHeader();

        std::string password;
        std::cout << "Enter password to analyze (or type 'gen' to generate one): ";
        std::getline(std::cin, password);

        if (password == "gen" || password == "generate") {
            std::string gen = generateStrongPassword(16);
            std::cout << "\n\033[1;32mGenerated strong password:\033[0m " << gen << "\n\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
            continue;
        }

        if (password.empty()) {
            std::cout << "\033[1;31mNo password entered.\033[0m\n";
            std::cin.get();
            continue;
        }

        auto analysis = analyzePassword(password);
        clearScreen();
        printHeader();
        printResult(analysis, password);

        std::cin.get();  // wait for Enter
    }

    return 0;
}
