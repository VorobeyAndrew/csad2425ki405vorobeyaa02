#include "ServerLogic.h"

const char* ManVsAIHandle(char player1move, char randommove) {
	return CheckWinCondition(player1move, randommove);
}

const char* ManVsManHandle(char player1move, char player2move) {
	return CheckWinCondition(player1move, player2move);
}

const char* AIVsAIHandle(char randommove1, char randommove2) {
	return CheckWinCondition(randommove1, randommove2);
}

const char* CheckWinCondition(char player1move, char player2move) 
{
	const char* result;

	if (player1move == 'r')
	{
		if (player2move == 'r') result = "Draw!";

		else if (player2move == 'p') result = "Player 2 win!";

		else if (player2move == 's') result = "Player 1 win!";

		else result = "Incorrect data!";
	}
	else if (player1move == 'p')
	{
		if (player2move == 'r') result = "Player 1 win!";

		else if (player2move == 'p') result = "Draw!";

		else if (player2move == 's') result = "Player 2 win!";

		else result = "Incorrect data!";
	}
	else if (player1move == 's')
	{
		if (player2move == 'r') result = "Player 2 win!";

		else if (player2move == 'p') result = "Player 1 win!";

		else if (player2move == 's') result = "Draw!";

		else result = "Incorrect data!";
	}
	else
	{
		result = "Incorrect data!";
	}

	return result;
}