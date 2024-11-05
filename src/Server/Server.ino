char player1move;
char player2move;
char gamemode;

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{
  if (Serial.available()) 
  {
    LaunchGame(TokenizeMessageFromClient(Serial.readString()));
  }
}

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

char RandomChoice() 
{
  int randomNumber = rand() % 3;

  if (randomNumber == 0) return 'r';
  if (randomNumber == 1) return 'p';
  if (randomNumber == 2) return 's';

  return 'r';
}

void CheckWinCondition(char player1move, char player2move) 
{
  Serial.println("Player 1 move: " + String(player1move));
  Serial.println("Player 2 move: " + String(player2move));

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

void ManVsAIHandle(char player1move) 
{
  CheckWinCondition(player1move, RandomChoice());
}

void ManVsManHandle(char player1move, char player2move) 
{
  CheckWinCondition(player1move, player2move);
}

void AIVsAIHandle() 
{
  CheckWinCondition(RandomChoice(), RandomChoice());
}