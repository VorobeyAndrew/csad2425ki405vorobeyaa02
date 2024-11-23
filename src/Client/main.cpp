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

HANDLE setupSerial(const std::wstring& portName, int BaudRate) {
	HANDLE hSerial = CreateFileW(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE) {
		DWORD dwError = GetLastError();
		std::cerr << "[ERROR] Failed to open COM port." << dwError << std::endl;
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

void ListAvailableCOMPorts() {
	// Define a range for COM port numbers
	const int maxPorts = 255;
	char portName[20];

	std::cout << "Active COM Ports: ";
	for (int i = 1; i <= maxPorts; ++i) {
		// Generate port name
		snprintf(portName, sizeof(portName), "\\\\.\\COM%d", i);

		// Try to open the port
		HANDLE hCOM = CreateFileA(portName,
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr);

		if (hCOM != INVALID_HANDLE_VALUE) {
			std::cout << "COM" << i << "; ";
			CloseHandle(hCOM); // Close handle if the port is active
		}
	}
	std::cout << std::endl;
}

int main()
{
	while (true)
	{
		ListAvailableCOMPorts();
		std::cout << "Enter COM Port: ";
		std::wstring COMPort; // = L"COM6"
		std::wcin >> COMPort;
		std::cout << "Posible baud rates: 300, 1200, 2400, 4800, 9600(default), 14400, 19200, 28800, 38400, 57600, 115200" << std::endl;
		std::cout << "Enter BaudRate: ";
		int BaudRate = 9600;
		std::cin >> BaudRate;
		std::cin.ignore();


		std::string message;
		std::cout << "Enter your message: ";
		std::getline(std::cin, message);

		HANDLE hSerial = setupSerial(COMPort, BaudRate);
		if (hSerial == INVALID_HANDLE_VALUE) {
			continue;
		}

		sendMessage(message, hSerial);
		receiveMessage(hSerial);

		CloseHandle(hSerial);
		system("pause");
	}
	return 0;
}
