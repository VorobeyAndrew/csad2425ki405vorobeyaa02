#include <string>
#include <windows.h>
#include <algorithm>
#include <cctype> 
#include <vector>

class HelperUtils {

public:
    static std::string toLowerCase(std::string& text);

    static void setConsoleColor(int color);

    enum ConsoleColor {
        BLACK = 0,
        DARK_BLUE = 1,
        DARK_GREEN = 2,
        DARK_CYAN = 3,
        DARK_RED = 4,
        DARK_MAGENTA = 5,
        DARK_YELLOW = 6,
        LIGHT_GRAY = 7,
        DARK_GRAY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        LIGHT_YELLOW = 14,
        WHITE = 15
    };
};
