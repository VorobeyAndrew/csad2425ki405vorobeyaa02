#include "ServerLogic.h"

/**
 * @brief Stores the moves of Player 1 and Player 2, and the selected game mode.
 */
char player1move;
char player2move;
char gamemode;

/**
 * @brief Initializes serial communication at 9600 baud rate.
 */
void setup() 
{
  Serial.begin(9600);
}

/**
 * @brief Continuously checks for data from the client and processes the game logic.
 * 
 * If a message is available from the serial port, it reads the message and starts the game by calling `LaunchGame()`.
 */
void loop() 
{
  if (Serial.available()) 
  {
    LaunchGame(Serial.readString().c_str());
  }
}

/**
 * @brief Launches the game based on the game mode and player moves received.
 * 
 * @param tokenizedMessage A tokenized message that contains the game mode and player moves.
 */
void LaunchGame(char* tokenizedMessage)
{
  for (int i = 0; i < strlen(tokenizedMessage); i++) 
    {
      if (i == 0) gamemode = tokenizedMessage[i];
      if (i == 1) player1move = tokenizedMessage[i];
      if (i == 2) player2move = tokenizedMessage[i];
    }

    switch(gamemode)
    {
      case '1':
        char Random_Choice = RandomChoice();
        Serial.println(String(player1move) + String(Random_Choice));
        Serial.println(String(ManVsAIHandle(player1move, Random_Choice)));
        break;

      case '2':
        Serial.println(String(player1move) + String(player2move));
        Serial.println(String(ManVsManHandle(player1move, player2move)));
        break;

      case '3':
        char RandomChoice1 = RandomChoice();
        char RandomChoice2 = RandomChoice();
        Serial.println(String(RandomChoice1) + String(RandomChoice2));
        Serial.println(String(AIVsAIHandle(RandomChoice1,RandomChoice2)));
        break;
      
      default:
        Serial.println("Unknown gamemode!");
        break;
    }
}

/**
 * @brief Generates a random move for the AI (rock, paper, or scissors).
 * 
 * @return A character representing the AI's move ('r' for rock, 'p' for paper, 's' for scissors).
 */
char RandomChoice() 
{
  int randomNumber = rand() % 3;

  if (randomNumber == 0) return 'r';
  if (randomNumber == 1) return 'p';
  if (randomNumber == 2) return 's';

  return 'r';
}
