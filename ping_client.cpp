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

constexpr const char *kServerIP = "127.0.0.1";
constexpr uint16_t kPort = 10000;

int main()
{
    // initialize client socket, setup with IPV4, and define TCP, while letting the OS choose the default protocol (0)
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Failed to create socket" << std::endl;
    }

    // initialize serverAddress to be of sockaddr_in data type
    sockaddr_in serverAddress{};
    // make serverAddress a uniform initialization which zero-intiailizes all fields of struct

    // setup for IPv4
    serverAddress.sin_family = AF_INET;

    // convert the port into something that's readable for serverAddress
    serverAddress.sin_port = htons(kPort);

    // make server listen to the IP
    if (inet_pton(AF_INET, kServerIP, &serverAddress.sin_addr) <= 0)
    {
        std::cerr << "Invalid address or address not supported" << std::endl;
        return 1;
    }

    // connect socket to server @ struct sockaddr* &serverAddress (pointer to IP + Port)

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        return 1;
    }

    // sends message from clientSocket
    // const char *message = "Server Connected!";
    // send(clientSocket, message, strlen(message), 0);

    char buffer[1024];
    std::string input;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, input);

        // input.c_str() must be done to turn input into a pointer, as send needs the raw char array
        send(clientSocket, input.c_str(), input.length(), 0);

        if (input == "exit")
        {
            break;
        }
        // else if (input == "ping")
        // {
        //     std::cout << "pong" << std::endl;
        // }
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRecieved = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesRecieved <= 0)
        {
            std::cout << "Server disconnected" << std::endl;
            break;
        }
        buffer[bytesRecieved] = '\0';
        std::cout << "Server: " << buffer << "\n";
    }
    close(clientSocket);
    return 0;
}