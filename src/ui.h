#ifndef UI_H
#define UI_H

#include <string>
#include <iostream>
#include "json.hpp"

// Forward declaration
namespace nlohmann { class json; }
using json = nlohmann::json;

// --- ANSI COLOR CODES ---
#define COLOR_VINTAGE_GREEN "\033[38;5;10m"
#define COLOR_BLUE_SELECTION "\033[1;34m"
#define COLOR_PROMPT "\033[1;36m"
#define COLOR_YELLOW_BANNER "\033[1;33m"
#define COLOR_RESET "\033[0m"

void type_out(const std::string& text, int delay = 25000);
void print_banner();
std::string show_menu();
void show_help();
void display_report(const json& data);

#endif // UI_H
