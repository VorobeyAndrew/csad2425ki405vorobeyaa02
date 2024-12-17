#include <gtest/gtest.h>
#include "../../Client/HelperUtils.h"

// Test toLowerCase function
TEST(HelperUtilsTest, ToLowerCase) {
    std::string text = "HELLO";
    std::string result = HelperUtils::toLowerCase(text);
    EXPECT_EQ(result, "hello");
}

TEST(HelperUtilsTest, ToLowerCaseMixedCase) {
    std::string text = "HeLLo";
    std::string result = HelperUtils::toLowerCase(text);
    EXPECT_EQ(result, "hello");
}

TEST(HelperUtilsTest, ToLowerCaseEmptyString) {
    std::string text = "";
    std::string result = HelperUtils::toLowerCase(text);
    EXPECT_EQ(result, "");
}

// Test setConsoleColor function
TEST(HelperUtilsTest, SetConsoleColorDoesNotCrash) {
    for (int color = 0; color <= 15; ++color) {
        EXPECT_NO_THROW(HelperUtils::setConsoleColor(color));
    }
}