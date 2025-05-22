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
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    const char *message = "hello, server";
    send(clientSocket, message, strlen(message), 0);
    close(clientSocket);
}