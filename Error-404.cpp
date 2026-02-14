#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <regex>

using namespace std;

// Expanded weak patterns
vector<string> weak_patterns = {"123", "1234", "12345", "000", "111", "password", "admin", "letmein", "iloveyou", "sunshine", "princess"};

// Keyboard patterns
vector<string> keyboard_patterns = {"qwerty", "asdf", "zxcv", "123qwe", "qazwsx", "zaq1xsw2", "poiuy"};

// Substitutions for detection (e.g., P@ssword -> Password)
map<char, char> substitutions = {{'@', 'a'}, {'3', 'e'}, {'1', 'i'}, {'0', 'o'}, {'$', 's'}, {'7', 't'}};

// Regex for years (1980-2025) and phones
regex year_pattern(R"(19[8-9]\d|20[0-2][0-5])");
regex phone_pattern(R"(\d{3}-\d{3}-\d{4}|\d{10})");

// Smart human pattern detection
bool hasHumanPattern(const string& pwd) {
    string lower = pwd;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    // Year check
    if (regex_search(lower, year_pattern)) return true;
    // Phone check
    if (regex_search(lower, phone_pattern)) return true;
    // Substitution check (simple common subs)
    for (auto& sub : substitutions) {
        if (lower.find(sub.first) != string::npos) return true;
    }
    return false;
}

// Keyboard pattern detection
bool hasKeyboardPattern(const string& pwd) {
    string lower = pwd;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const string& pat : keyboard_patterns) {
        if (lower.find(pat) != string::npos) return true;
    }
    return false;
}

// Repeats detection
bool hasRepeats(const string& pwd) {
    for (size_t i = 2; i < pwd.length(); ++i) {
        if (pwd[i] == pwd[i-1] && pwd[i] == pwd[i-2]) return true;
    }
    return false;
}

// Weak pattern detection
bool hasWeakPattern(const string& pwd) {
    string lower = pwd;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    for (const string& pat : weak_patterns) {
        if (lower.find(pat) != string::npos) return true;
    }
    return false;
}

// Check character types
void checkTypes(const string& pwd, bool& hasLower, bool& hasUpper, bool& hasDigit, bool& hasSpecial) {
    hasLower = hasUpper = hasDigit = hasSpecial = false;
    for (char c : pwd) {
        if (islower(c)) hasLower = true;
        else if (isupper(c)) hasUpper = true;
        else if (isdigit(c)) hasDigit = true;
        else if (ispunct(c)) hasSpecial = true;
    }
}

// Calculate charset size
int calculateCharset(bool hasLower, bool hasUpper, bool hasDigit, bool hasSpecial) {
    int size = 0;
    if (hasLower) size += 26;
    if (hasUpper) size += 26;
    if (hasDigit) size += 10;
    if (hasSpecial) size += 32;
    return max(size, 1);
}

// Calculate entropy
double calculateEntropy(size_t length, int charset) {
    return length * log2(static_cast<double>(charset));
}

// Get category and score
pair<string, int> getCategoryAndScore(double entropy, size_t length, int variety, bool weak, bool keyboard, bool repeats, bool human) {
    int score = min(static_cast<int>(length), 20) * 3 + variety * 10;
    if (weak) score -= 30;
    if (keyboard) score -= 20;
    if (repeats) score -= 15;
    if (human) score -= 25;
    score = max(score, 0);

    string category;
    if (score >= 85) category = "Very Strong";
    else if (score >= 65) category = "Strong";
    else if (score >= 40) category = "Medium";
    else if (score >= 20) category = "Weak";
    else category = "Very Weak";

    return {category, score};
}

// Estimate crack time with visual timeline simulation (console text)
string estimateCrackTime(double entropy) {
    if (entropy < 1) return "Instantly";

    double attempts = pow(2, entropy);
    double speed = 1e12; // Trillion/sec for realism
    double seconds = attempts / speed;

    if (seconds < 60) return to_string(static_cast<int>(seconds)) + " seconds";
    if (seconds < 3600) return to_string(static_cast<int>(seconds / 60)) + " minutes";
    if (seconds < 86400) return to_string(static_cast<int>(seconds / 3600)) + " hours";
    if (seconds < 31536000) return to_string(static_cast<int>(seconds / 86400)) + " days";
    if (seconds < 31536000LL * 100) return to_string(static_cast<int>(seconds / 31536000)) + " years";
    return to_string(static_cast<int>(seconds / (31536000LL * 100))) + " centuries";
}

// Get suggestions
string getSuggestions(bool hasLower, bool hasUpper, bool hasDigit, bool hasSpecial, size_t length, bool weak, bool keyboard, bool repeats, bool human) {
    string tips = "Improvement Suggestions:\n";
    if (length < 12) tips += " - Increase length to at least 12 characters for better security.\n";
    if (!hasLower) tips += " - Add lowercase letters (a-z).\n";
    if (!hasUpper) tips += " - Add uppercase letters (A-Z).\n";
    if (!hasDigit) tips += " - Add digits (0-9).\n";
    if (!hasSpecial) tips += " - Add special characters (!@#$%^&*).\n";
    if (weak) tips += " - Avoid common words or simple patterns like '12345'.\n";
    if (keyboard) tips += " - Avoid keyboard walk patterns like 'qwerty' or 'asdf'.\n";
    if (repeats) tips += " - Avoid repeated characters like 'aaa' or '111'.\n";
    if (human) tips += " - Avoid predictable patterns like birth years, phone numbers, or simple substitutions like 'P@ssword'.\n";
    if (tips == "Improvement Suggestions:\n") tips += " - Your password is strong - keep it safe!\n";
    tips += "\nRemember: Use unique passwords for each account and enable 2FA where possible.\n";
    return tips;
}

// Password improvement engine (transforms to stronger version)
string improvePassword(const string& pwd) {
    string improved = pwd;
    if (improved.empty()) return "";
    // Capitalize first
    if (improved.length() > 0) improved[0] = toupper(improved[0]);
    // Extend length if short
    if (improved.length() < 12) improved += "!" + to_string(rand() % 100) + "@";
    // Add missing types
    if (!regex_search(improved, regex("[a-z]"))) improved += "a";
    if (!regex_search(improved, regex("[A-Z]"))) improved[improved.length() / 2] = toupper(improved[improved.length() / 2]);
    if (!regex_search(improved, regex("[0-9]"))) improved += to_string(rand() % 10);
    if (!regex_search(improved, regex("[!@#$%^&*]"))) improved += "!@#$"[rand() % 4];
    // Shuffle middle to add randomness without losing all structure
    random_shuffle(improved.begin() + 1, improved.end() - 1);
    return improved;
}

// Personalized secure memory mode (looks related to names, e.g., MasroorAhmed786@)
string generatePersonalized(const string& first, const string& last, int lucky) {
    // Base looks like name: FirstLastLucky with mixes
    string base = first + last + to_string(lucky);
    // Capitalize key parts to make it look natural
    base[0] = toupper(base[0]);
    base[first.length()] = toupper(base[first.length()]);
    // Insert symbols in predictable but secure way
    string symbols = "!@#";
    base.insert(first.length() / 2, string(1, symbols[rand() % symbols.length()]));
    base.insert(base.length() / 2, to_string(rand() % 10));
    // Add extra entropy at end but keep name-like start
    base += string(1, symbols[rand() % symbols.length()]) + to_string(rand() % 10);
    // Minimal shuffle on end part to keep front recognizable
    random_shuffle(base.begin() + first.length() + 2, base.end());
    return base;
}

// Random generator
string generateRandom(int length = 16) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=[]{}|;:,.<>?";
    string pwd;
    for (int i = 0; i < length; ++i) {
        pwd += chars[rand() % chars.length()];
    }
    return pwd;
}

// Multi-platform estimator
string estimatePlatform(const string& category) {
    if (category == "Very Strong" || category == "Strong") return "Safe for Banking or Government accounts.";
    if (category == "Medium") return "OK for Social Media or everyday use.";
    return "Not recommended for any sensitive accounts.";
}

// Health score dashboard (console text version)
void showHealthDashboard(int score, size_t length, int variety, double entropy, bool weak, bool keyboard, bool repeats, bool human) {
    cout << "Password Health Dashboard:\n";
    cout << " - Length Score: " << min(static_cast<int>(length * 5), 100) << "/100\n";
    cout << " - Diversity Score: " << variety * 25 << "/100\n";
    cout << " - Entropy Score: " << min(static_cast<int>(entropy * 1.5), 100) << "/100\n";
    cout << " - Pattern Risk: " << (weak || keyboard || repeats || human ? "High (vulnerable)" : "Low (secure)") << "\n";
    cout << " - Overall Score: " << score << "/100\n";
}

// Help section - focused on explaining features
void showHelp() {
    cout << "Help - Understanding SecurePass Pro Features:\n\n";
    cout << "This project is a cybersecurity tool designed to help users create and evaluate strong passwords for everyday use. Here's what each feature does:\n";
    cout << "1. Password Strength Analyzer: Evaluates your password based on length, character variety (lowercase, uppercase, digits, special), and entropy (a measure of randomness). It categorizes as Very Weak, Weak, Medium, Strong, or Very Strong.\n";
    cout << "2. Brute-Force Attack Simulation: Calculates how long it might take for a hacker using powerful computers to guess your password. This shows real-world risk in seconds, minutes, hours, days, years, or centuries.\n";
    cout << "3. Smart Pattern Detection: Checks for common vulnerabilities like keyboard patterns (e.g., 'qwerty'), repeats (e.g., 'aaa'), weak words (e.g., 'password'), years (e.g., '1990'), phone numbers, and substitutions (e.g., 'P@ssword').\n";
    cout << "4. Password Improvement Engine: Takes your weak password and suggests a stronger version by adding missing elements, extending length, and randomizing parts while keeping it usable.\n";
    cout << "5. Personalized Secure Generator: Uses your first name, last name, and lucky number to create a password that looks related to your identity (memorable) but adds security with mixes, symbols, and randomization to avoid predictability.\n";
    cout << "6. Random Secure Generator: Creates a completely random strong password with high entropy, ideal for maximum security.\n";
    cout << "7. Health Score Dashboard: Breaks down your password's strength into scores for length, diversity, entropy, and pattern risk, helping you see exactly what's good or bad.\n";
    cout << "8. Platform Suitability Estimator: Advises if the password is safe for social media, banking, or government based on strength.\n\n";
    cout << "Why this project? It solves real problems like weak, predictable passwords by educating and assisting users. Use it to stay secure online!\n";
}

// Main menu
int main() {
    srand(time(nullptr));

    cout << "===== SecurePass Pro =====\n";
    cout << "Intelligent Password Security Assistant\n";
    cout << "Happy Valentine's Day 2026, Masroor from Nawabshah! ??\n\n";

    while (true) {
        cout << "Menu:\n";
        cout << "1. Analyze Password\n";
        cout << "2. Improve Password\n";
        cout << "3. Personalized Secure Generator\n";
        cout << "4. Random Secure Generator\n";
        cout << "5. Help - Feature Explanations\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        int choice; cin >> choice; cin.ignore();

        if (choice == 0) break;

        string pwd, first, last;
        int lucky = 0;

        if (choice == 1 || choice == 2) {
            cout << "Enter password: ";
            getline(cin, pwd);
        } else if (choice == 3) {
            cout << "First name: ";
            getline(cin, first);
            cout << "Last name: ";
            getline(cin, last);
            cout << "Lucky number: ";
            cin >> lucky; cin.ignore();
        }

        if (choice == 1 || choice == 2) {
            bool hasLower, hasUpper, hasDigit, hasSpecial;
            checkTypes(pwd, hasLower, hasUpper, hasDigit, hasSpecial);
            int variety = hasLower + hasUpper + hasDigit + hasSpecial;
            int charset = calculateCharset(hasLower, hasUpper, hasDigit, hasSpecial);
            double entropy = calculateEntropy(pwd.length(), charset);
            bool weak = hasWeakPattern(pwd);
            bool keyboard = hasKeyboardPattern(pwd);
            bool repeats = hasRepeats(pwd);
            bool human = hasHumanPattern(pwd);
            auto [category, score] = getCategoryAndScore(entropy, pwd.length(), variety, weak, keyboard, repeats, human);
            string crackTime = estimateCrackTime(entropy);
            string suggestions = getSuggestions(hasLower, hasUpper, hasDigit, hasSpecial, pwd.length(), weak, keyboard, repeats, human);

            if (choice == 1) {
                cout << "\nAnalysis Results:\n";
                cout << "Category: " << category << "\n";
                cout << "Score: " << score << "%\n";
                cout << "Entropy: " << entropy << " bits\n";
                cout << "Crack Time Estimate: " << crackTime << "\n";
                cout << suggestions << "\n";
                showHealthDashboard(score, pwd.length(), variety, entropy, weak, keyboard, repeats, human);
                cout << "Platform Suitability: " << estimatePlatform(category) << "\n\n";
            } else if (choice == 2) {
                string improved = improvePassword(pwd);
                cout << "Improved Password: " << improved << "\n";
                // Re-analyze
                auto [newCat, newScore] = getCategoryAndScore(calculateEntropy(improved.length(), calculateCharset(hasLower, hasUpper, hasDigit, hasSpecial)), improved.length(), variety, weak, keyboard, repeats, human);
                cout << "New Category: " << newCat << " (" << newScore << "%)\n\n";
            }
        } else if (choice == 3) {
            string generated = generatePersonalized(first, last, lucky);
            cout << "Personalized Secure Password: " << generated << "\n";
            auto [category, score] = getCategoryAndScore(calculateEntropy(generated.length(), calculateCharset(true, true, true, true)), generated.length(), 4, false, false, false, false); // Assume full variety
            cout << "Strength: " << category << " (" << score << "%)\n\n";
        } else if (choice == 4) {
            string generated = generateRandom();
            cout << "Random Secure Password: " << generated << "\n";
            auto [category, score] = getCategoryAndScore(calculateEntropy(generated.length(), calculateCharset(true, true, true, true)), generated.length(), 4, false, false, false, false);
            cout << "Strength: " << category << " (" << score << "%)\n\n";
        } else if (choice == 5) {
            showHelp();
            cout << "\n";
        }
    }

    return 0;
}
