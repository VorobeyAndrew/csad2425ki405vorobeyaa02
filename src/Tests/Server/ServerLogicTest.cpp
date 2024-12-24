#include <gtest/gtest.h>
#include "../../Client/ClientCommunication.h"
#include <string>
#include <vector>

static std::wstring g_port = L"COM6";
static int g_baudRate = 9600;

void parseCommandLineArgs(int argc, char** argv) 
{
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.rfind("--port=", 0) == 0) 
        {
            // Â³äð³çàºìî "--port="
            std::string portValue = arg.substr(7);
            g_port = std::wstring(portValue.begin(), portValue.end());
        }
        else if (arg.rfind("--baud=", 0) == 0) 
        {
            std::string baudValue = arg.substr(7);
            try 
            {
                g_baudRate = std::stoi(baudValue);
            }
            catch (...) 
            {
                std::cerr << "[WARNING] Failed to parse baud rate from: " << baudValue << std::endl;
            }
        }
    }
}

class ClientCommunicationTest : public ::testing::Test
{
protected:
    ClientCommunication* clientComm;

    virtual void SetUp() override
    {
        clientComm = new ClientCommunication(g_port, g_baudRate);

        ASSERT_NE(clientComm->GethSerial(), INVALID_HANDLE_VALUE)
            << "Serial handle is invalid. Possibly COM port not found or busy.";
    }

    virtual void TearDown() override
    {
        delete clientComm;
    }
};

TEST_F(ClientCommunicationTest, ReceiveMessageManVSAITest)
{
    clientComm->sendMessage("1r");

    std::vector<std::string> response = clientComm->receiveMessage();

    ASSERT_EQ(response.size(), 3);

    EXPECT_FALSE(response[0].empty()) << "First part of the message is empty!";
    EXPECT_FALSE(response[1].empty()) << "Second part of the message is empty!";
    EXPECT_FALSE(response[2].empty()) << "Third part of the message is empty!";
}

TEST_F(ClientCommunicationTest, ReceiveMessageManVSManTest)
{
    clientComm->sendMessage("2rp");

    std::vector<std::string> response = clientComm->receiveMessage();

    ASSERT_EQ(response.size(), 3);

    EXPECT_FALSE(response[0].empty()) << "First part of the message is empty!";
    EXPECT_FALSE(response[1].empty()) << "Second part of the message is empty!";
    EXPECT_FALSE(response[2].empty()) << "Third part of the message is empty!";
}

TEST_F(ClientCommunicationTest, ReceiveMessageAIVSAITest)
{
    clientComm->sendMessage("3");

    std::vector<std::string> response = clientComm->receiveMessage();

    ASSERT_EQ(response.size(), 3);

    EXPECT_FALSE(response[0].empty()) << "First part of the message is empty!";
    EXPECT_FALSE(response[1].empty()) << "Second part of the message is empty!";
    EXPECT_FALSE(response[2].empty()) << "Third part of the message is empty!";
}

TEST_F(ClientCommunicationTest, SendMessageTest)
{
    std::string testMessage = "1p";

    clientComm->sendMessage(testMessage);

    ASSERT_NE(clientComm->GethSerial(), INVALID_HANDLE_VALUE);
}

TEST(ClientCommunicationNoFixtureTest, MultipleConnectionsTest)
{
    {
        ClientCommunication comm1(g_port, g_baudRate);
        EXPECT_NE(comm1.GethSerial(), INVALID_HANDLE_VALUE);
    }
    {
        ClientCommunication comm2(g_port, g_baudRate);
        EXPECT_NE(comm2.GethSerial(), INVALID_HANDLE_VALUE);
    }
}

int main(int argc, char** argv) 
{
    parseCommandLineArgs(argc, argv);
    std::wcout << L"COMPort" << g_port << std::endl;
    std::cout << "BaudRate" << g_baudRate << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    std::cout << "Test run completed with result code: " << result << std::endl;
    return result;
}