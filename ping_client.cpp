#include <iostream> // For std::cout, std::cerr, etc.
#include <string>   // For std::string
#include <cstring>  // For std::strcmp, std::strlen, std::memset
#include <thread>   // For std::thread
#include <vector>   // For std::vector
#include <mutex>    // For std::mutex, std::lock_guard
#include <atomic>   // For std::atomic<bool>

#include <sys/socket.h> // For socket(), bind(), connect(), recv(), send()
#include <netinet/in.h> // For sockaddr_in, htons(), htonl()
#include <arpa/inet.h>  // For inet_pton(), inet_ntoa()
#include <unistd.h>     // For close() to close sockets
#include <errno.h>      // For error codes like errno

int main()
{
    // initialize client socket, setup with IPV4, and define TCP, while letting the OS choose the default protocol (0)
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // initialize serverAddress to be of sockaddr_in data type
    sockaddr_in serverAddress;

    // setup for IPv4
    serverAddress.sin_family = AF_INET;

    // convert the port into something that's readable for serverAddress
    serverAddress.sin_port = htons(8080);

    // make server listen to all possible IPs
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // connect socket to server @ struct sockaddr* &serverAddress (pointer to IP + Port)
    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    // sends message from clientSocket
    const char *message = "Server Connected!";
    send(clientSocket, message, strlen(message), 0);

    std::string userMessage;
    std::cout << "Enter command";
    std::getline(std::cin, userMessage);

    // ensure that the message parameter is a pointer to the character array in st ring (this parameter is supposed to be a buffer)
    send(clientSocket, userMessage.c_str(), userMessage.size(), 0);

    close(clientSocket);
}