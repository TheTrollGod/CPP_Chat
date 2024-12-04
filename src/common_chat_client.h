#ifndef COMMON_CHAT_CLIENT_H
#define COMMON_CHAT_CLIENT_H

#include <string>
#include <thread>

class ChatClient {
protected:
    int clientSocket;               // Socket descriptor
    std::string serverIP;           // Server IP address
    int serverPort;                 // Server port
    bool isRunning = true;          // Control loop flag

    virtual bool initializeSocket() = 0;  // Platform-specific implementation
    virtual void cleanupSocket() = 0;    // Platform-specific cleanup

    void receiveMessages();         // Receive messages in a separate thread

public:
    std::string username = "Anonymous";

    ChatClient(const std::string& ip, int port) : serverIP(ip), serverPort(port) {}
    virtual ~ChatClient() {}

    bool connectToServer();         // Handles server connection logic
    void sendMessage(const std::string& message); // Send a message
    void run();                     // Main loop
};

#endif
