#include "ClientCommunication.h"
#include <iostream>

// Constructor
ClientCommunication::ClientCommunication(const std::wstring& portName) {
    this->portName = portName;
    hSerial = setupSerial(portName);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "[ERROR] Failed to initialize serial communication." << std::endl;
    }
}

// Destructor
ClientCommunication::~ClientCommunication() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
    }
}

// Method to send message
void ClientCommunication::sendMessage(const std::string& message) {
    DWORD bytesSent;
    if (!WriteFile(hSerial, message.c_str(), message.size(), &bytesSent, NULL)) {
        std::cerr << "[ERROR] Failed to send message.\n";
    }
    else {
        std::cout << "[INFO] Message sent to Arduino: " << message << std::endl;
    }
}

// Method to receive message
std::string ClientCommunication::receiveMessage() {
    char buffer[128];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::cerr << "[ERROR] Failed to read from Arduino.\n";
        return "";
    }
    std::string received(buffer, bytesRead);
    std::cout << "[INFO] Message received from Arduino: " << std::endl << received << std::endl;
    return received;
}

// Method to set up serial connection
HANDLE ClientCommunication::setupSerial(const std::wstring& portName) {
    HANDLE hSerial = CreateFileW(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "[ERROR] Failed to open COM port.\n";
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "[ERROR] Failed to get COM port state.\n";
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "[ERROR] Failed to set COM port state.\n";
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

HANDLE ClientCommunication::GethSerial() {
    return hSerial;
}
