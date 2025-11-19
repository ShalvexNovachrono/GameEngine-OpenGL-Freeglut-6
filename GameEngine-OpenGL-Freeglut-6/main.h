#ifndef main_h
#define main_h

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>
#include <vector>
#include <random>
#include <cmath>
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "GL/freeglut.h"


using namespace std;


#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN  "\033[32m"
#define COLOR_YELLOW  "\033[33m"

#define MAKE_STRING_RED(_string)    (string(COLOR_RED) + _string + COLOR_RESET)
#define MAKE_STRING_YELLOW(_string) (string(COLOR_YELLOW) + _string + COLOR_RESET)
#define MAKE_STRING_GREEN(_string) (string(COLOR_GREEN) + _string + COLOR_RESET)

#define LOG_DEBUG(message) cout << MAKE_STRING_GREEN("[LOGGING] " + string(message)) << "\n"
#define LOG_DEBUG_R(message) cout << "\r" << MAKE_STRING_GREEN("[LOGGING] " + string(message)) << flush
#define LOG_WARNING(message) cerr << MAKE_STRING_YELLOW("[WARNING] " + string(message)) << "\n"
#define LOG_ERROR(message) { cerr << MAKE_STRING_RED("[ERROR] " + string(message)) << "\n"; exit(EXIT_FAILURE); }
#define print(message) cout << message << "\n"



inline int count_reoccurrence(const string& s, const char ch) {
    return count_if(s.begin(), s.end(), [=](const char c) { return c == ch; });
}

#endif // !main_h