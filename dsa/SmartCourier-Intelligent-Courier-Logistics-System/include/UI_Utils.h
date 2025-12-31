#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <iostream>
#include <string>
#include <limits>

using namespace std;

// ANSI Color Codes
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

class UI {
public:
    static string cyan() { return CYAN; }
    static string reset() { return RESET; }
    static string yellow() { return YELLOW; }
    static string red() { return RED; }
    static string green() { return GREEN; }
    static string blue() { return BLUE; }
    static string magenta() { return MAGENTA; }
    static string white() { return WHITE; }
    static string bold() { return BOLD; }
    static string underline() { return UNDERLINE; }

    static void printLogo() {
        cout << BOLD << CYAN << R"(
   _____                      _       _____           _ _   _             
  / ____|                    | |     / ____|         | | | (_)            
 | (___   ___ _ __ ___   __ _| |_   | |     ___ _ __ | |_| |_ _ __   __ _  
  \___ \ / _ \ '_ ` _ \ / _` | __|  | |    / _ \ '_ \| __| | | '_ \ / _` | 
  ____) |  __/ | | | | | (_| | |_   | |___|  __/ | | | |_| | | | | | (_| | 
 |_____/ \___|_| |_| |_|\__,_|\__|   \_____\___|_| |_|\__|_|_|_| |_|\__, | 
                                                                    __/ | 
                                                                   |___/  
        )" << RESET << endl;
        cout << BOLD << YELLOW << "Intelligent Courier Logistics Engine" << RESET << endl;
        cout << GREEN << "==========================================" << RESET << endl;
    }

    static void printHeader(const string& header) {
        cout << BOLD << BLUE << "\n=== " << header << " ===" << RESET << endl;
    }

    static void printOption(int num, const string& option) {
        cout << CYAN << num << ". " << RESET << option << endl;
    }

    static void printError(const string& error) {
        cout << RED << "Error: " << error << RESET << endl;
    }

    static void printWarning(const string& warning) {
        cout << YELLOW << "Warning: " << warning << RESET << endl;
    }

    static void printSuccess(const string& success) {
        cout << GREEN << "Success: " << success << RESET << endl;
    }

    static void waitForEnter() {
        cout << CYAN << "\nPress Enter to continue..." << RESET;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }

    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
};

#endif // UI_UTILS_H