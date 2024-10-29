#include <string>
#include <windows.h>
#include <iostream>

void sendMessage(const std::string& message, HANDLE& hSerial) {
    DWORD bytesSent;
    if (!WriteFile(hSerial, message.c_str(), message.size(), &bytesSent, NULL)) {
        std::cerr << "[ERROR] Failed to send message.\n";
    }
    else {
        std::cout << "[INFO] Message sent to Arduino: " << message << std::endl;
    }
}

std::string receiveMessage(HANDLE& hSerial) {
    char buffer[128];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
        std::cerr << "[ERROR] Failed to read from Arduino.\n";
        return "";
    }
    std::string received(buffer, bytesRead);
    std::cout << "[INFO] Message received from Arduino: " << received << std::endl;
    return received;
}

HANDLE setupSerial(const std::wstring& portName) {
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

int main() {
    HANDLE hSerial = setupSerial(L"COM6");
    if (hSerial == INVALID_HANDLE_VALUE) {
        return 1;
    }

    std::string message;
    std::cout << "Enter your message: ";
    std::getline(std::cin, message);

    sendMessage(message, hSerial);
    receiveMessage(hSerial);

    CloseHandle(hSerial);
    system("pause");
    return 0;
}
