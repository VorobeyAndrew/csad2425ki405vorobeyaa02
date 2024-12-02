#include <string>
#include <windows.h>
#include <algorithm>
#include <cctype> 
#include <vector>

class HelperUtils {

public:
    /**
    * @brief Converts the given string to lowercase.
    *
    * @param text Reference to the string to be converted.
    * @return The converted lowercase string.
    */
    static std::string toLowerCase(std::string& text);

    /**
     * @brief Sets the color of the console output.
     *
     * @param color The integer value representing the console color.
     */
    static void setConsoleColor(int color);

    /**
     * @brief Enumeration of console color values.
     */
    enum ConsoleColor {
        BLACK = 0,          /**< Black color */
        DARK_BLUE = 1,      /**< Dark blue color */
        DARK_GREEN = 2,     /**< Dark green color */
        DARK_CYAN = 3,      /**< Dark cyan color */
        DARK_RED = 4,       /**< Dark red color */
        DARK_MAGENTA = 5,   /**< Dark magenta color */
        DARK_YELLOW = 6,    /**< Dark yellow color */
        LIGHT_GRAY = 7,     /**< Light gray color */
        DARK_GRAY = 8,      /**< Dark gray color */
        LIGHT_BLUE = 9,     /**< Light blue color */
        LIGHT_GREEN = 10,   /**< Light green color */
        LIGHT_CYAN = 11,    /**< Light cyan color */
        LIGHT_RED = 12,     /**< Light red color */
        LIGHT_MAGENTA = 13, /**< Light magenta color */
        LIGHT_YELLOW = 14,  /**< Light yellow color */
        WHITE = 15          /**< White color */
    };
};
