#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

class ChatClient {
private:
    int clientSocket;
    std::string serverIP = "127.0.0.1"; // Default to localhost
    int serverPort = 8080;  // Default port
    bool isRunning = true;


    void receiveMessages() {
        char buffer[1024];
        while (isRunning) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0'; // Null-terminate the received data
                if (buffer != message) {
                    std::cout << buffer << std::endl;
                }

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
    std::string username = "Anonymous";
    std::string message;
    ChatClient(std::string ip, int port) : serverIP(ip), serverPort(port) {}

    bool connectToServer() {
        std::cout << "Attempting to connect to server" << std::endl;
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket < 0) {
            std::cerr << "Failed to create socket." << std::endl;
            return false;
        }
        std::cout << "Created client socket" << std::endl;

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        std::cout << "Assigned client socket characteristics" << std::endl;

        // Convert IP address from text to binary form
        if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
            std::cerr << "Invalid address/ Address not supported." << std::endl;
            return false;
        }

        // Connect to the server
        // TODO: research connect function to make it time out?
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Connection to server failed." << std::endl;
            return false;
        }
        std::cout << "Server connection successful" << std::endl;

        std::cout << "Connected to the server at " << serverIP << ":" << serverPort << std::endl;
        std::cout << "Use /quit to exit" << std::endl;

        // Start the message receiving thread
        // TODO: Track threads to bring back when closing
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
        int firstMessage = 0;
        //std::cout << "Starting Client" << std::endl;
        while (isRunning) {
            std::string tempMessage;
            if (firstMessage == 0) {
                message = "[" + username + " has joined]";
                firstMessage = 1;
            }
            else {
                std::getline(std::cin, tempMessage); // Get input from the user
                message = "[" + username + "]: " + tempMessage;
                if (tempMessage == "/quit") {
                    isRunning = false;
                    break;
                }
            }
            sendMessage(message);
        }

        // Close the socket when done
        sendMessage("[" + username + " has disconnected]");
        closesocket(clientSocket);
        std::cout << "Disconnected from the server." << std::endl;
    }
};

int main() {
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0) {
        std::cerr << "WSAStartup failed with error: " << wsaResult << std::endl;
    }

    std::string serverIP = "127.0.0.1"; // Change this to your server's IP address if needed
    int serverPort = 8080;
    std::cout<< "Enter the server ip address" << std::endl;
    std::cin >> serverIP;
    std::cout << "Enter the server port" << std::endl;
    std::cin >> serverPort;

    ChatClient client(serverIP, serverPort);

    std::cout << "Enter your username" << std::endl;
    std::cin >> client.username;

    if (client.connectToServer()) {
        client.run();
    } else {
        std::cerr << "Failed to connect to the server. " << std::endl;
    }

    WSACleanup();

    return 0;
}
