// Server source file for the chat application

//Includes
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <queue>

// Includes for error handeling
#include <exception>
#include <cstdlib>
#include <signal.h>

// Linux specific includes
#include <netinet/in.h>
#include <unistd.h>

// Global variable declarations
std::string logFilePath = "/var/log/chat.log";
std::ofstream logFile;


// Socket setup
// Receive clients
// Handel messages
// Log

struct ClientInfo {
    int socket;
    std::string username;
};

// Template: Message Queue
template <typename T>
class MessageQueue {
    std::queue<T> queue;
public:
    void push(const T& message) {
        queue.push(message);
    }
    T pop() {
        T front = queue.front();
        queue.pop();
        return front;
    }
};
// Class: Chat Client
class ChatClient {
    int clientSocket;
public:
    void connectToServer();
    void sendMessage(const std::string& message);
    void receiveMessages();
};

// Class: Chat Server
class ChatServer {
    // Creates a vector of all of the clients with their respective information
    std::vector<ClientInfo> clients;
    int serverSocket;


public:
    bool keyBorInt = true;
    void start();
    void broadcastMessage(const std::string& message, int senderSocket);
    void handleClient(int clientSocket);

    void keyboardInteruptHandler(int signum);
    void close();
};

void ChatServer::keyboardInteruptHandler(int signum) {
    std::cout << "Interrupt signal recived (" << signum << ")" << std::endl;
    keyBorInt = false;
}
// Global signal declaration to make things easier
void signal(int i, void(ChatServer::* keyboard_interupt_handler)(int signum));

// Function: Start the Server
void ChatServer::start() {
    std::cout << "Server started." << std::endl;
    std::ofstream logFile(logFilePath, std::ios::app);  // App to append to the log file
    // Inital socket initalization


     do{
        signal(SIGINT, keyboardInteruptHandler);
        // Do the logic of the chat server here
    }
    while(keyBorInt);

}


// Function: Broadcast Message
void ChatServer::broadcastMessage(const std::string& message, int senderSocket) {
    char buffer[1024];

    // Broadcast to each client
    // Saves each buffer to a log file
    logFile << buffer << std::endl;

}

// Function: Handle a Single Client
void ChatServer::handleClient(int clientSocket) {
    // Handles client connections
}

void ChatServer::close() {
    // Handles closing of the server
}

// Main Function
int main() {
    ChatServer server;
    try {
        server.start();
    }
    // general catch
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
    catch () {
        // Default catch
        std::cout << "Something unexpected happened" << std::endl;
    }


    return 0;
}