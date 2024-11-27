// Server source file for the chat application

//Includes
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <queue>

// Includes for error handeling
#include <exception>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <signal.h>

// Linux specific includes
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>

// Global variable declarations
std::string logFilePath = "/var/log/chat.log";
std::ofstream logFile;
//int serverPort = 1234; // server port


// Function: Structure for saving client info
struct ClientInfo {
    int socket;
    std::string username;
};

// Data structure: A queue for messages
template <typename T>
class MessageQueue {
    std::queue<T> queue;
public:
    bool isEmpty() const {
        return queue.empty();
    }
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
    // Avector of all of the clients with their respective information
    std::vector<ClientInfo> clients;
    // A vector that stores thread ID's to keep track of them
    // See the folowing datatype that will be put into the vector:
    // std::thread clientThread(&ChatServer::handleClient, this, clientSocket);
    std::vector<std::thread> clientThreads;
    // Message buffer
    char buffer[1024];
    // Socket for the server
    int serverSocket;
    // On object of the message Queue
    MessageQueue<std::string> messageQueue;


public:
    // Variable to detect a keyboard interupt. Its function is do capture a ctrl+c
    // so that the main loop breaks cleanly
    bool keyBorInt = true;
    // Starts the chat server
    void start(int serverPort);
    // Starts the broadcast loop to send messages from the message queue
    void broadcastMessage();
    // Handles the client interactions
    void handleClient(int clientSocket);
    // Stops the server
    void stop();
    // Deconstructor to clean up the server
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
void ChatServer::start(int serverPort) {
    std::cout << "Server started." << std::endl;

    // TODO: Create an if statment seeing if the log file exists
    std::cout << "Opening log file" << std::endl;
    logFile.open(logFilePath, std::ios::app);  // App to append to the log file
    std::cout << "Log file opened" << std::endl;
    // Inital socket initalization

    // AF_INET = IPv4, SOCK_STREAM = TCP
    std::cout << "Starting socket initalization" << std::endl;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }
    std::cout << "Socket creation success" << std::endl;

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);
    std::cout << "Assigned socket charecteristics" << std::endl;

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr))) {
        std::cerr << "Socket bind failed." << std::endl;
        return;
    }
    std::cout << "Socket Bind success" << std::endl;

    listen(serverSocket, 5);
    std::cout<< "Listening on port "<< serverPort << std::endl;

    // Chat server logic to accept clients
    std::thread broadcastThread = std::thread(&ChatServer::broadcastMessage, this);
    while(keyBorInt) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);
        if (clientSocket < 0) {
            std::cerr << "Socket accept failed. Error: " << std::strerror(errno) << std::endl;
        }
        std::cout << "New Client connected!" << std::endl;
        clients.push_back(ClientInfo{clientSocket, "Temp_Uername"});
        std::cout << "Client added to thread vector" << std::endl;
        // Create a thread so that the client can continue to communicate with the server outside of this process
        std::thread clientThread(&ChatServer::handleClient, this, clientSocket);
        std::cout << "Thread started" << std::endl;
        clientThreads.push_back(std::move(clientThread));

        //clientThread.detach();

    }
}

void ChatServer::stop() {
    keyBorInt = false;
}

// Function: Broadcast Message
//void ChatServer::broadcastMessage(const std::string& message, int senderSocket) {
void ChatServer::broadcastMessage() {
    // Broadcast to each client
    std::cout << "Broadcasting loop starting" << std::endl;
    while (true) {
        while (messageQueue.isEmpty()) {
            sleep(0.25);
        }
        std::string msg = messageQueue.pop(); // Waits for a message
        std::cout << "Message received: " << msg << std::endl;
        for (const auto& client : clients) {
            // Send the message to each client
            send(client.socket, msg.c_str(), msg.size(), 0);
            std::cout << "Message sent" << std::endl;
            // Log the broadcasted message
            logFile << msg << std::endl;
            std::cout << "Message saved to logFile" << std::endl;
        }
    }
}


    // Function: Handle a Single Client
void ChatServer::handleClient(int clientSocket) {
    // Handles client connections

    // Wait for messages from the client
    while (recv(clientSocket, buffer, sizeof(buffer), 0) > 0) {
        std::cout << "Recived message. Adding to buffer. Message: " << buffer << std::endl;
        std::string message(buffer);
        messageQueue.push(message);
        std::cout << "Message added to the message queue" << std::endl;
        memset(buffer, 0, sizeof(buffer));
    }

    // If the client disconnects
    std::cout << "Client disconnected: " << clientSocket << std::endl;
    close(clientSocket);  // Close the connection
}





// Signal handeling
ChatServer* server = nullptr;

void signalHandler(int signum) {
    if (server) {
        server->stop();  // Stop the server's loops
        std::cout << "Captured Ctrl+C, stopping the server..." << std::endl;
    }
}





// Main Function
int main() {
    server = new ChatServer;
    try {
        signal(SIGINT, signalHandler);

        int start_port;
        std::cout << "Server port: ";
        std::cin >> start_port;

        server->start(start_port);  // This runs until stop() is called
    } catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
    }

    delete server;  // Cleanup server resources after stop() is called
    server = nullptr;

    std::cout << "Server stopped." << std::endl;
    return 0;
}
