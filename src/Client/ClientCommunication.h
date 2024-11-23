#include <string>
#include <windows.h>
#include <vector>

class ClientCommunication 
{
private:
    HANDLE hSerial;
    std::wstring portName;
    int baudRate;

    std::wstring readArduinoCOMPort(const std::wstring& configFile);
public:
    ClientCommunication(int BaudRate);
    ~ClientCommunication();

    void sendMessage(const std::string& message);
    std::vector<std::string> receiveMessage();
    HANDLE setupSerial(const std::wstring& portName, int BaudRate);

    HANDLE GethSerial();
};
