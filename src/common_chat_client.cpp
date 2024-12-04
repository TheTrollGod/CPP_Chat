#include "common_chat_client.h"
#include <iostream>
#include <cstring>

void ChatClient::receiveMessages() {
    char buffer[1024];
    while (isRunning) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << buffer << std::endl;
        } else if (bytesReceived == 0) {
            std::cout << "Server disconnected." << std::endl;
            isRunning = false;
            break;
        } else {
            std::cerr << "Error receiving message." << std::endl;
            break;
        }
    }
}

bool ChatClient::connectToServer() {
    if (!initializeSocket()) {
        std::cerr << "Failed to initialize socket." << std::endl;
        return false;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid server address." << std::endl;
        return false;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
        return false;
    }

    std::thread recvThread(&ChatClient::receiveMessages, this);
    recvThread.detach();
    return true;
}

void ChatClient::sendMessage(const std::string& message) {
    if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
        std::cerr << "Failed to send message." << std::endl;
    }
}

void ChatClient::run() {
    while (isRunning) {
        std::string message;
        std::getline(std::cin, message);
        if (message.empty()) continue;

        if (message == "/quit") {
            isRunning = false;
            break;
        }

        sendMessage("[" + username + "]: " + message);
    }

    cleanupSocket();
}
