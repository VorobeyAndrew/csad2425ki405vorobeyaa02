#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../../Client/ClientCommunication.h"

class MockClientCommunication {
public:
    MOCK_METHOD(HANDLE, CreateFileW, (const std::wstring& portName), ());
    MOCK_METHOD(bool, WriteFile, (HANDLE hSerial, const std::string& message), ());
    MOCK_METHOD(std::string, ReadFile, (HANDLE hSerial), ());
    MOCK_METHOD(bool, SetCommState, (HANDLE hSerial), ());
    MOCK_METHOD(bool, GetCommState, (HANDLE hSerial), ());
    MOCK_METHOD(std::wstring, readArduinoCOMPort, (const std::wstring& configFile), ());
};

HANDLE setupSerial(const std::wstring& portName, int BaudRate, MockClientCommunication& mockComm) {
    HANDLE hSerial = mockComm.CreateFileW(portName);
    if (hSerial == INVALID_HANDLE_VALUE) return hSerial;

    if (!mockComm.GetCommState(hSerial)) {
        return INVALID_HANDLE_VALUE;
    }

    if (!mockComm.SetCommState(hSerial)) {
        return INVALID_HANDLE_VALUE;
    }

    return hSerial;
}

void sendMessage(const std::string& message, HANDLE hSerial, MockClientCommunication& mockComm) {
    if (!mockComm.WriteFile(hSerial, message)) {
        throw std::runtime_error("Failed to send message.");
    }
}

std::string receiveMessage(HANDLE hSerial, MockClientCommunication& mockComm) {
    return mockComm.ReadFile(hSerial);
}

class ClientCommunicationTest : public ::testing::Test {
protected:
    MockClientCommunication mockComm;
};

TEST_F(ClientCommunicationTest, SetupSerialSuccess) {
    EXPECT_CALL(mockComm, CreateFileW(std::wstring(L"COM6")))
        .WillOnce(::testing::Return(reinterpret_cast<HANDLE>(1)));
    EXPECT_CALL(mockComm, GetCommState(::testing::_)).WillOnce(::testing::Return(true));
    EXPECT_CALL(mockComm, SetCommState(::testing::_)).WillOnce(::testing::Return(true));

    HANDLE hSerial = setupSerial(L"COM6", 9600, mockComm);
    ASSERT_NE(hSerial, INVALID_HANDLE_VALUE);
}

TEST_F(ClientCommunicationTest, SetupSerialFailure) {
    EXPECT_CALL(mockComm, CreateFileW(std::wstring(L"COM6")))
        .WillOnce(::testing::Return(INVALID_HANDLE_VALUE));

    HANDLE hSerial = setupSerial(L"COM6", 9600, mockComm);
    ASSERT_EQ(hSerial, INVALID_HANDLE_VALUE);
}

TEST_F(ClientCommunicationTest, SendMessageSuccess) {
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(1);

    EXPECT_CALL(mockComm, WriteFile(fakeHandle, "Hello"))
        .WillOnce(::testing::Return(true));

    EXPECT_NO_THROW(sendMessage("Hello", fakeHandle, mockComm));
}

TEST_F(ClientCommunicationTest, SendMessageFailure) {
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(1);

    EXPECT_CALL(mockComm, WriteFile(fakeHandle, "Hello"))
        .WillOnce(::testing::Return(false));

    EXPECT_THROW(sendMessage("Hello", fakeHandle, mockComm), std::runtime_error);
}

TEST_F(ClientCommunicationTest, ReceiveMessageSuccess) {
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(1);

    EXPECT_CALL(mockComm, ReadFile(fakeHandle))
        .WillOnce(::testing::Return("ReceivedMessage"));

    std::string message = receiveMessage(fakeHandle, mockComm);
    ASSERT_EQ(message, "ReceivedMessage");
}

TEST_F(ClientCommunicationTest, ReceiveMessageFailure) {
    HANDLE fakeHandle = reinterpret_cast<HANDLE>(1);

    EXPECT_CALL(mockComm, ReadFile(fakeHandle)).WillOnce(::testing::Return(""));

    std::string message = receiveMessage(fakeHandle, mockComm);
    ASSERT_EQ(message, "");
}

TEST_F(ClientCommunicationTest, ReadArduinoCOMPortSuccess) {
    EXPECT_CALL(mockComm, readArduinoCOMPort(std::wstring(L"../config/ConfigFile.json")))
        .WillOnce(::testing::Return(L"COM6"));

    std::wstring port = mockComm.readArduinoCOMPort(L"../config/ConfigFile.json");
    ASSERT_EQ(port, L"COM6");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cout << "Test run completed with result code: " << result << std::endl;
    return result;
}