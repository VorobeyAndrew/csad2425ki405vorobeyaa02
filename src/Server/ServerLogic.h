const char* ManVsAIHandle(char player1move, char randommove);
const char* ManVsManHandle(char player1move, char player2move);
const char* AIVsAIHandle(char randommove1, char randommove2);
const char* CheckWinCondition(char player1move, char player2move);
char RandomChoice();
char* TokenizeMessageFromClient(const char* message);