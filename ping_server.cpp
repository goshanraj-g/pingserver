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

constexpr uint16_t kPort = 10000;
// create port that the server will listen on

constexpr size_t kMaxLineLen = 100;
// create message length

void runServer()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET -> IPv4 protocol family
    // SOCK_STREAM -> defines TCP type socket

    if (serverSocket == -1)
    {
        std::cerr << "Failed to create socket\n"
                  << std::endl;
        return;
    }
    // error handling

    sockaddr_in serverAddress{};
    // data type to store socket address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(kPort);
    // Host TO Network Short -> converts 16 bit integer (port number) to hosts's byte order (ensures everyone on the internet understands the numbers the same way)
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // don't bind socket to any particular IP, and make it listen to all available IPs

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Bind failed: " << strerror(errno) << std::endl;
        close(serverSocket);
        return;
    }
    // when if statement is run, the bind is already created, if there's an issue, then it will be handled by the error handler
    // without bind, os won't know port number, and ip address/interface it sohuld be bound to
    // tells OS which IP and port it belongs to so it can be binded
    // serverSocket (int) -> id number for socket
    // (struct sockaddr*)&serverAddress (casted pointer) -> pointer to sockaddr_in structure
    // sizeof(serverAddress) (socklen_t) -> tells os how big the address struct is, needed so it knows how much memory to read

    if (listen(serverSocket, 5) < 0)
    {
        std::cerr << "Listen failed: " << strerror(errno) << std::endl;
        close(serverSocket);
        return;
    }
    // tell application to listen to the socket referred by the serverSocket, and if more than 5 people try to connect at once, their connection will be refused/delayed

    std::cout << "Server listening on port " << kPort << "...\n"
              << std::endl;

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    // accept() is used to accept the connection request which is recieved on the socket the application was listening to
    // take first client from waiting queue on serverSocket, and give a new socket to talk to that client
    // second and third parameter can be used to get the client's IP and port, but we don't necessarily need it

    if (clientSocket < 0)
    {
        std::cerr << "Accept failed: " << strerror(errno) << std::endl;
        close(serverSocket);
        return;
    }

    char buffer[1024];

    // create a constant loop which listens to client
    while (true)
    {
        std::memset(buffer, 0, sizeof(buffer));
        // clear buffer before each recieve request
        ssize_t bytesRecieved = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        // sizeof(buffer) - 1 -> how many bytes recieve function should try to read, -1 is included because of the null terminator
        // 0 -> read data as is
        // NOTE: recv() does not automatically add a null terminator to count the end of the string

        if (bytesRecieved <= 0)
        {
            std::cout << "Client disconnected or error has occured. \n";
            break;
        }
        buffer[bytesRecieved] = '\0';
        // this is required to indicate the end of a string

        if (std::strcmp(buffer, "ping") == 0)
        {
            std::cout << "Recieved: ping -> Sending: pong\n";
            send(clientSocket, "pong", 4, 0);
        }
        else
        {
            std::cout << "Recieved unknown command: " << buffer << "\n";
            send(clientSocket, "Unknown Command", 16, 0);
        }
    }
    close(clientSocket);
    close(serverSocket);
}

int main()
{
    runServer();
    return 0;
}
