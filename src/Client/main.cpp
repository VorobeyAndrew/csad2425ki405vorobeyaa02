#include "ClientCommunication.h"
#include "HelperUtils.h"
#include <iostream>
#include <string>

int main() 
{
    ClientCommunication clientcommunication;


    std::string message;
    std::string gamemode;
    std::string player1move;
    std::string player2move;
    std::string isNewGame;

    do
    {
        std::cout << "Choose gamemode (1 - Man Vs AI, 2 - Man Vs Man, 3 - AI Vs AI): ";
        std::getline(std::cin, gamemode);

        system("cls");

        if (gamemode == "1") {
            std::cout << "Make your move (r - rock, p - paper, s - scissors): ";
            std::getline(std::cin, player1move);
            message = gamemode + player1move;

            clientcommunication.sendMessage(HelperUtils::toLowerCase(message));
            clientcommunication.receiveMessage();
        }
        else if (gamemode == "2") {
            std::cout << "Player 1 make your move (r - rock, p - paper, s - scissors): ";
            std::getline(std::cin, player1move);

            system("cls");

            std::cout << "Player 2 make your move (r - rock, p - paper, s - scissors): ";
            std::getline(std::cin, player2move);
            message = gamemode + player1move + player2move;

            
            clientcommunication.sendMessage(HelperUtils::toLowerCase(message));
            clientcommunication.receiveMessage();
        }
        else if (gamemode == "3") {
            message = gamemode;
            clientcommunication.sendMessage(HelperUtils::toLowerCase(message));
            clientcommunication.receiveMessage();
        }
        else {
            std::cout << "Invalid gamemode!" << std::endl;
        }

        while (true)
        {
            std::cout << "New game(Y/n): ";
            std::getline(std::cin, isNewGame);
            HelperUtils::toLowerCase(isNewGame);

            if (isNewGame == "n" || isNewGame == "y") break;

            else std::cout << "Invalid option!" << std::endl;
        }

        if (isNewGame == "n") break;
        else system("cls");
        

    } while (true);

    return 0;
}
