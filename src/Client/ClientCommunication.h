#include <string>
#include <windows.h>
#include <vector>

class ClientCommunication 
{
private:
    HANDLE hSerial;            /**< Handle to the serial port */
    std::wstring portName;     /**< Name of the COM port */
    int baudRate;              /**< Communication baud rate */

    
public:
    /**
    * @brief Constructor that initializes the serial communication with the specified baud rate.
    *
    * @param BaudRate The baud rate for serial communication.
    */
    ClientCommunication(int BaudRate);

    /**
     * @brief Destructor to close the serial port and clean up resources.
     */
    ~ClientCommunication();

    /**
    * @brief Sends a message to the Arduino device.
    *
    * @param message The message to be sent as a standard string.
    */
    void sendMessage(const std::string& message);


    /**
     * @brief Receives a message from the Arduino device.
     *
     * @return A vector of strings containing the received message.
     */
    std::vector<std::string> receiveMessage();

    /**
     * @brief Configures the serial port communication with the specified port and baud rate.
     *
     * @param portName Name of the COM port.
     * @param BaudRate The baud rate for communication.
     * @return A handle to the configured serial port.
     */
    HANDLE setupSerial(const std::wstring& portName, int BaudRate);

    /**
     * @brief Retrieves the handle to the serial port.
     *
     * @return The handle to the serial port.
     */
    HANDLE GethSerial();

    /**
    * @brief Reads the configured COM port from a configuration file.
    *
    * @param configFile Path to the configuration file containing port settings.
    * @return The name of the Arduino COM port as a wide string.
    */
    std::wstring readArduinoCOMPort(const std::wstring& configFile);
};
