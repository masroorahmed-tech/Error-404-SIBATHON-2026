#include "analyzer.h"
#include <cctype>
#include <cmath>
#include <string>
#include <algorithm>

void checkCharacterTypes(const std::string& pwd,
                         bool& hasLower, bool& hasUpper,
                         bool& hasDigit, bool& hasSpecial) {
    hasLower = hasUpper = hasDigit = hasSpecial = false;
    for (char c : pwd) {
        if (std::islower(c)) hasLower = true;
        else if (std::isupper(c)) hasUpper = true;
        else if (std::isdigit(c)) hasDigit = true;
        else if (std::ispunct(c) || std::string("~`!@#$%^&*()-_=+[{]}\\|;:'\",<.>/?").find(c) != std::string::npos)
            hasSpecial = true;
    }
}

int calculateCharsetSize(bool lower, bool upper, bool digit, bool special) {
    int size = 0;
    if (lower)  size += 26;
    if (upper)  size += 26;
    if (digit)  size += 10;
    if (special) size += 32;  // common printable special chars
    return size ? size : 1;   // avoid log2(0)
}

double calculateEntropy(const std::string& pwd, int charsetSize) {
    if (pwd.empty()) return 0.0;
    return pwd.length() * std::log2(static_cast<double>(charsetSize));
}

std::string estimateCrackTime(double entropyBits) {
    if (entropyBits < 1) return "instantly";

    // Assume 10 billion guesses per second (modern GPU cluster approx)
    double attempts = std::pow(2.0, entropyBits);
    double seconds = attempts / 1e10;

    if (seconds < 60)               return std::to_string(static_cast<int>(seconds + 0.5)) + " seconds";
    if (seconds < 3600)             return std::to_string(static_cast<int>(seconds / 60 + 0.5)) + " minutes";
    if (seconds < 86400)            return std::to_string(static_cast<int>(seconds / 3600 + 0.5)) + " hours";
    if (seconds < 31536000)         return std::to_string(static_cast<int>(seconds / 86400 + 0.5)) + " days";
//    if (seconds < 31536000 * 100)   return std::to_string(static_cast<int>(seconds / 31536000 + 0.5)) + " years";
    return "centuries";
}

std::string getCategoryAndScore(double entropy, size_t length, int variety) {
    int score = 0;
    score += std::min(static_cast<int>(length), 20) * 3;      // max 60
    score += variety * 10;                                     // max 40
    score = std::min(score, 100);

    if (entropy < 30 || length < 8 || variety < 2)  return "Weak|" + std::to_string(score);
    if (entropy < 60 || length < 12 || variety < 3) return "Medium|" + std::to_string(score);
    return "Strong|" + std::to_string(score);
}

std::string getSuggestions(bool l, bool u, bool d, bool s, size_t len) {
    std::string tips;
    if (len < 12)           tips += "- Make it at least 12–16 characters long\n";
    if (!l)                 tips += "- Add lowercase letters (a–z)\n";
    if (!u)                 tips += "- Add uppercase letters (A–Z)\n";
    if (!d)                 tips += "- Add numbers (0–9)\n";
    if (!s)                 tips += "- Add special characters (!@#$%^&* etc.)\n";
    if (tips.empty())       tips = "Excellent! This is a very strong password.";
    else                    tips = "To make it stronger:\n" + tips;
    return tips;
}

PasswordAnalysis analyzePassword(const std::string& password) {
    PasswordAnalysis result;

    bool hasLower, hasUpper, hasDigit, hasSpecial;
    checkCharacterTypes(password, hasLower, hasUpper, hasDigit, hasSpecial);

    int variety = hasLower + hasUpper + hasDigit + hasSpecial;
    int charset = calculateCharsetSize(hasLower, hasUpper, hasDigit, hasSpecial);
    double entropy = calculateEntropy(password, charset);

    std::string catScore = getCategoryAndScore(entropy, password.length(), variety);
    size_t sep = catScore.find('|');
    result.category = catScore.substr(0, sep);
    result.strengthScore = std::stoi(catScore.substr(sep + 1));

    result.entropy = entropy;
    result.crackTime = estimateCrackTime(entropy);
    result.suggestions = getSuggestions(hasLower, hasUpper, hasDigit, hasSpecial, password.length());

    return result;
}

// Bonus: simple strong password generator
std::string generateStrongPassword(int length) {
    const std::string chars =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789"
        "!@#$%^&*()_+-=[]{}|;:,.<>?";
    
    std::string pwd;
    for (int i = 0; i < length; ++i) {
        pwd += chars[rand() % chars.size()];
    }
    return pwd;
}
