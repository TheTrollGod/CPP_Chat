#include <iostream>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class ChatClient {
private:
    int clientSocket;
    std::string serverIP = "127.0.0.1"; // Default to localhost
    int serverPort = 8080;
    bool isRunning = true;

    void receiveMessages() {
        char buffer[1024];
        while (isRunning) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0'; // Null-terminate the received data
                std::cout << "[Server]: " << buffer << std::endl;
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

public:
    ChatClient(std::string ip, int port) : serverIP(ip), serverPort(port) {}

    bool connectToServer() {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            return false;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);

        // Convert IP address from text to binary form
        if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported." << std::endl;
            return false;
        }

        // Connect to the server
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection to server failed." << std::endl;
            return false;
        }

        std::cout << "Connected to the server at " << serverIP << ":" << serverPort << std::endl;

        // Start the message receiving thread
        std::thread recvThread(&ChatClient::receiveMessages, this);
        recvThread.detach();

        return true;
    }

    void sendMessage(const std::string& message) {
        if (send(clientSocket, message.c_str(), message.length(), 0) < 0) {
            std::cerr << "Message sending failed." << std::endl;
        }
    }

    void run() {
        while (isRunning) {
            std::string message;
            std::getline(std::cin, message); // Get input from the user

            if (message == "/quit") {
                isRunning = false;
                break;
            }

            sendMessage(message);
        }

        // Close the socket when done
        close(clientSocket);
        std::cout << "Disconnected from the server." << std::endl;
    }
};

int main() {
    std::string serverIP = "127.0.0.1"; // Change this to your server's IP address if needed
    int serverPort = 8080;

    ChatClient client(serverIP, serverPort);

    if (client.connectToServer()) {
        client.run();
    } else {
        std::cerr << "Failed to connect to the server." << std::endl;
    }

    return 0;
}
