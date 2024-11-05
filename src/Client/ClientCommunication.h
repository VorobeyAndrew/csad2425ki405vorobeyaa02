#include <string>
#include <windows.h>

class ClientCommunication {
private:
    HANDLE hSerial;
    std::wstring portName;
public:
    ClientCommunication(const std::wstring& portName);
    ~ClientCommunication();

    void sendMessage(const std::string& message);
    std::string receiveMessage();
    HANDLE setupSerial(const std::wstring& portName);

    HANDLE GethSerial();
};
