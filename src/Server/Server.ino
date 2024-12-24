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
    LaunchGame(TokenizeMessageFromClient(Serial.readString()));
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
        ManVsAIHandle(player1move);
        break;

      case '2':
        ManVsManHandle(player1move, player2move);
        break;

      case '3':
        AIVsAIHandle();
        break;
      
      default:
        Serial.println("Unknown gamemode!");
        break;
    }
}

/**
 * @brief Converts a message from the client into a tokenized char array.
 * 
 * @param message The message received from the client as a String.
 * @return A pointer to a tokenized char array.
 */
char* TokenizeMessageFromClient(String message) 
{
  int length = message.length();
  
  static char* decryptedMessage = new char[length + 1];  // Add 1 for null terminator
  
  // Convert the String to a char array
  for (int i = 0; i < length; i++) 
  {
    decryptedMessage[i] = message[i];
  }
  decryptedMessage[length] = '\0';  // Null-terminate the string
  
  return decryptedMessage;
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

/**
 * @brief Compares the moves of Player 1 and Player 2 and determines the winner.
 * 
 * @param player1move The move of Player 1.
 * @param player2move The move of Player 2.
 */
void CheckWinCondition(char player1move, char player2move) 
{
  //Serial.println("Player 1 move: " + String(player1move));
  //Serial.println("Player 2 move: " + String(player2move));

  Serial.println(String(player1move) + String(player2move));

  if (player1move == 'r') 
  {
    if (player2move == 'r') Serial.println("Draw!");

    else if (player2move == 'p') Serial.println("Player 2 win!");

    else if (player2move == 's') Serial.println("Player 1 win!");

    else Serial.println("Incorrect data!");
  } 
  else if (player1move == 'p') 
  {
    if (player2move == 'r') Serial.println("Player 1 win!");

    else if (player2move == 'p') Serial.println("Draw!");

    else if (player2move == 's') Serial.println("Player 2 win!");

    else Serial.println("Incorrect data!");
  } 
  else if (player1move == 's') 
  {
    if (player2move == 'r') Serial.println("Player 2 win!");

    else if (player2move == 'p') Serial.println("Player 1 win!");

    else if (player2move == 's') Serial.println("Draw!");

    else Serial.println("Incorrect data!");
  } 
  else 
  {
    Serial.println("Incorrect data!");
  }
}

/**
 * @brief Handles the game logic for the Man vs AI game mode.
 * 
 * @param player1move The move of Player 1.
 */
void ManVsAIHandle(char player1move) 
{
  CheckWinCondition(player1move, RandomChoice());
}

/**
 * @brief Handles the game logic for the Man vs Man game mode.
 * 
 * @param player1move The move of Player 1.
 * @param player2move The move of Player 2.
 */
void ManVsManHandle(char player1move, char player2move) 
{
  CheckWinCondition(player1move, player2move);
}

/**
 * @brief Handles the game logic for the AI vs AI game mode.
 */
void AIVsAIHandle() 
{
  CheckWinCondition(RandomChoice(), RandomChoice());
}