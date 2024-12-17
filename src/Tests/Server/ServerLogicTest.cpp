#include <gtest/gtest.h>
#include "../../Server/ServerLogic.h"

// Тест функції CheckWinCondition
TEST(ServerLogicTests, CheckWinConditionTests) {
    // Тести для "камінь"
    EXPECT_STREQ(CheckWinCondition('r', 'r'), "Draw!");
    EXPECT_STREQ(CheckWinCondition('r', 'p'), "Player 2 win!");
    EXPECT_STREQ(CheckWinCondition('r', 's'), "Player 1 win!");

    // Тести для "папір"
    EXPECT_STREQ(CheckWinCondition('p', 'r'), "Player 1 win!");
    EXPECT_STREQ(CheckWinCondition('p', 'p'), "Draw!");
    EXPECT_STREQ(CheckWinCondition('p', 's'), "Player 2 win!");

    // Тести для "ножиці"
    EXPECT_STREQ(CheckWinCondition('s', 'r'), "Player 2 win!");
    EXPECT_STREQ(CheckWinCondition('s', 'p'), "Player 1 win!");
    EXPECT_STREQ(CheckWinCondition('s', 's'), "Draw!");

    // Некоректні дані
    EXPECT_STREQ(CheckWinCondition('x', 'r'), "Incorrect data!");
    EXPECT_STREQ(CheckWinCondition('r', 'x'), "Incorrect data!");
    EXPECT_STREQ(CheckWinCondition('x', 'x'), "Incorrect data!");
}

// Тест функції ManVsAIHandle
TEST(ServerLogicTests, ManVsAIHandleTests) {
    // Перевіряємо тільки чи обгортка правильно передає аргументи
    EXPECT_STREQ(ManVsAIHandle('r', 'p'), CheckWinCondition('r', 'p'));
    EXPECT_STREQ(ManVsAIHandle('p', 's'), CheckWinCondition('p', 's'));
    EXPECT_STREQ(ManVsAIHandle('s', 'r'), CheckWinCondition('s', 'r'));
}

// Тест функції ManVsManHandle
TEST(ServerLogicTests, ManVsManHandleTests) {
    // Перевіряємо тільки чи обгортка правильно передає аргументи
    EXPECT_STREQ(ManVsManHandle('r', 'r'), CheckWinCondition('r', 'r'));
    EXPECT_STREQ(ManVsManHandle('p', 'r'), CheckWinCondition('p', 'r'));
    EXPECT_STREQ(ManVsManHandle('s', 'p'), CheckWinCondition('s', 'p'));
}

// Тест функції AIVsAIHandle
TEST(ServerLogicTests, AIVsAIHandleTests) {
    // Перевіряємо тільки чи обгортка правильно передає аргументи
    EXPECT_STREQ(AIVsAIHandle('r', 'p'), CheckWinCondition('r', 'p'));
    EXPECT_STREQ(AIVsAIHandle('p', 's'), CheckWinCondition('p', 's'));
    EXPECT_STREQ(AIVsAIHandle('s', 'r'), CheckWinCondition('s', 'r'));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cout << "Test run completed with result code: " << result << std::endl;
    return result;
}