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
#include <sys/socket.h>

// Global variable declarations
std::string logFilePath = "/var/log/chat.log";
std::ofstream logFile;
int serverPort = 8080; // server port



struct ClientInfo {
    int socket;
    std::string username;
};

// Message Queue
// Ensures that messages are not lost when the server is busy
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
    std::vector<std::thread> clientThreads;
    char buffer[1024];
    int serverSocket;
    // Creates an isntance of the message Queue
    MessageQueue<std::string> messageQueue;


public:
    bool keyBorInt = true;
    void start();
    void broadcastMessage(const std::string& message, int senderSocket);
    void handleClient(int clientSocket);
    void stop();
    ~ChatServer() {
        std::cout << "Stopping server" << std::endl;
        // Handles closing of the server
        for (auto& client : clients) {
            close(client.socket);  // Close each client's socket
        }
         logFile.close();
        close(serverSocket);
        for (auto& t : clientThreads) {
            t.join();
        }
        std::cout << "Server resources cleaned up." << std::endl;
    }
};


// Function: Start the Server
void ChatServer::start() {
    std::cout << "Server started." << std::endl;

    // TODO: Create an if statment seeing if the log file exists
    logFile.open(logFilePath, std::ios::app);  // App to append to the log file
    // Inital socket initalization

    // AF_INET = IPv4, SOCK_STREAM = TCP
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
        std::cerr << "Socket bind failed." << std::endl;
        return;
    }
    listen(serverSocket, 5);
    std::cout<< "Lestening on port "<< serverPort << std::endl;

    // Chat server logic to accept clients


    while(keyBorInt) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            std::cerr << "Socket accept failed." << std::endl;
        }
        clients.push_back(ClientInfo{clientSocket, "Temp_Uername"});

        // Create a thread so that the client can continue to communicate with the server outside of this process
        std::thread clientThread(&ChatServer::handleClient, this, clientSocket);
        clientThreads.push_back(clientThread);
        clientThread.detach();

    }
}

void ChatServer::stop() {
    keyBorInt = false;
}

// Function: Broadcast Message
void ChatServer::broadcastMessage(const std::string& message, int senderSocket) {
    // Broadcast to each client
    while (true) {
        std::string msg = messageQueue.pop(); // Waits for a message
        for (const auto& client : clients) {
            // Send the message to each client
            send(client.socket, msg.c_str(), msg.size(), 0);
        }
        // Log the broadcasted message
        logFile << msg << std::endl;
    }
}


    // Function: Handle a Single Client
void ChatServer::handleClient(int clientSocket) {
    // Handles client connections

    // Wait for messages from the client
    while (recv(clientSocket, buffer, sizeof(buffer), 0) > 0) {
        std::string message(buffer);
        messageQueue.push(message);
    }

    // If the client disconnects
    std::cout << "Client disconnected: " << clientSocket << std::endl;
    close(clientSocket);  // Close the connection
}





// Signal handeling
ChatServer* server = nullptr;

void signalHandler(int signum) {
    server->stop();
    delete server;
    std::cout << "Captured Ctrl+c" << std::endl;

    //exit(signum);
}




// Main Function
int main() {
    server = new ChatServer;
    try {
        signal(SIGINT, signalHandler);
        //server.start();
        // Gets the start function memory address from the memory space of server as defined by the sever pointer probably
        server->start();
    }
    // general catch
    catch (std::exception& e)
    {
        std::cout << e.what() << '\n';
    }

    std::cout << "Server stopped." << std::endl;

    return 0;
}