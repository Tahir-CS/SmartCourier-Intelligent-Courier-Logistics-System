#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

class UI {
public:
    // --- Colors ---
    static string red() { return "\033[1;31m"; }
    static string green() { return "\033[1;32m"; }
    static string blue() { return "\033[1;34m"; }
    static string yellow() { return "\033[1;33m"; }
    static string cyan() { return "\033[1;36m"; }
    static string reset() { return "\033[0m"; }

    static void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    // New Logo: "SMART COURIER"
    static void printLogo() {
        cout << green() << R"(
   _____                      __  
  / ___/____ ___  ____ ______/ /_ 
  \__ \/ __ `__ \/ __ `/ ___/ __/ 
 ___/ / / / / / / /_/ / /  / /_   
/____/_/ /_/ /_/\__,_/_/   \__/   
   ______                 _       
  / ____/___  __  _______(_)__  _____
 / /   / __ \/ / / / ___/ / _ \/ ___/
/ /___/ /_/ / /_/ / /  / /  __/ /    
\____/\____/\__,_/_/  /_/\___/_/     
       v1.0 - LOGISTICS SYSTEM
        )" << reset() << endl;
    }

    // Draws a solid divider line
    static void printLine() {
        cout << blue() << "============================================================" << reset() << endl;
    }

    // Prints a menu option aligned nicely
    static void printOption(int id, string text) {
        cout << "   " << cyan() << "[" << id << "]" << reset() << "  " << text << endl;
    }

    // A fake loading bar that looks different from your friend's
    static void showLoading(string message) {
        cout << message << " [";
        for(int i=0; i<20; i++) {
            cout << yellow() << "/" << reset(); // Uses slashes instead of dots
            cout.flush();
            this_thread::sleep_for(chrono::milliseconds(30));
        }
        cout << "] " << green() << "OK" << reset() << endl;
    }

    static void pause() {
        cout << "\n   Press Enter to return...";
        cin.ignore();
        cin.get();
    }
};

#endif
