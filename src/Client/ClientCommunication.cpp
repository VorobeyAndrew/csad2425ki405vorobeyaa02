#include "ClientCommunication.h"
#include <iostream>
#include <fstream>
#include "../third_party/include/nlohmann/json.hpp"


// Constructor
ClientCommunication::ClientCommunication(int BaudRate) 
{
    portName = readArduinoCOMPort(L"../config/ConfigFile.json");
    hSerial = setupSerial(portName, BaudRate);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "[ERROR] Failed to initialize serial communication." << std::endl;
    }
}

ClientCommunication::ClientCommunication(std::wstring portName, int BaudRate)
{
    hSerial = setupSerial(portName, BaudRate);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "[ERROR] Failed to initialize serial communication." << std::endl;
    }
}

// Destructor
ClientCommunication::~ClientCommunication() 
{
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

// Method to send a message
void ClientCommunication::sendMessage(const std::string& message) 
{
    DWORD bytesWritten;
    BOOL writeSuccess = WriteFile(hSerial, message.c_str(), message.size(), &bytesWritten, nullptr);
    if (!writeSuccess) {
        std::cerr << "[ERROR] Failed to send message. Error code: " << GetLastError() << std::endl;
    }
    else {
        std::cout << "[INFO] Message sent: " << message << std::endl;
    }

}

// Method to receive a message
std::vector<std::string> ClientCommunication::receiveMessage() 
{
    char buffer[256];
    DWORD bytesRead;
    std::vector<std::string> receivedMessage = {"", "", ""};
    std::string StringBuffer;

    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, nullptr)) {
        buffer[bytesRead] = '\0';  // Завершуємо рядок нульовим символом
        std::cout << "[INFO] Received message: " << std::endl << buffer << std::endl;

        StringBuffer = std::string(buffer);
        for (int i = 0; i < StringBuffer.size(); i++)
        {
            if (i == 0)
            {
                receivedMessage[0] = StringBuffer[i];
            }
            else if (i == 1)
            {
                receivedMessage[1] = StringBuffer[i];
            }
            else
            {
                receivedMessage[2] = receivedMessage[2] + StringBuffer[i];
            }
        }
        return receivedMessage;
    }
    else {
        std::cerr << "[ERROR] Failed to read from serial port. Error code: " << GetLastError() << std::endl;
        return receivedMessage;
    }
}

// Setup the serial port
HANDLE ClientCommunication::setupSerial(const std::wstring& portName, int BaudRate)
{
    hSerial = CreateFileW(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = { 0 };
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = BaudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = 1;
    dcbSerialParams.Parity = 0;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

// Method to get hSerial handle
HANDLE ClientCommunication::GethSerial() 
{
    return hSerial;
}

// Method to read ArduinoCOMPort from a JSON file
std::wstring ClientCommunication::readArduinoCOMPort(const std::wstring& configFile) 
{
    std::wstring ArduinoPortName;

    try {
        // Convert wstring to const char* using wstring::c_str() for the file path
        std::ifstream file(std::string(configFile.begin(), configFile.end()));  // Open file as regular stream
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open the config file.");
        }

        nlohmann::json jsonData;
        file >> jsonData;  // Deserialize JSON data

        // Read ArduinoCOMPort from JSON
        std::string comPort = jsonData["ArduinoSettings"]["COMPort"];
        ArduinoPortName = std::wstring(comPort.begin(), comPort.end()); // Convert to wstring

        // Output the COM port to verify
        std::wcout << L"[INFO] Arduino COM Port: " << ArduinoPortName << std::endl;
        return ArduinoPortName;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to read COM port from JSON: " << e.what() << std::endl;
    }
    return nullptr;
}


