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
int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // AF_INET -> IPv4 protocol family
    // SOCK_STREAM -> defines TCP type socket

    sockaddr_in serverAddress;
    // data type to store socket address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    // Host TO Network Short -> converts 16 bit integer (port number) to hosts's byte order (ensures everyone on the internet understands the numbers the same way)
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    // don't bind socket to any particular IP, and make it listen to all available IPs

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    // without bind, os won't know port number, and ip address/interface it sohuld be bound to
    // tells OS which IP and port it belongs to so it can be binded
    // serverSocket (int) -> id number for socket
    // (struct sockaddr*)&serverAddress (casted pointer) -> pointer to sockaddr_in structure
    // sizeof(serverAddress) (socklen_t) -> tells os how big the address struct is, needed so it knows how much memory to read

    listen(serverSocket, 5);
    // tell application to listen to the socket referred by the serverSocket, and if more than 5 people try to connect at once, their connection will be refused/delayed

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    // accept() is used to accept the connection request which is recieved on the socket the application was listening to
    // take first client from waiting queue on serverSocket, and give a new socket to talk to that client
    // second and third parameter can be used to get the client's IP and port, but we don't necessarily need it

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer << std::endl;
    // recieve data from client, buffer -> space to recieve the data sent to the client

    close(serverSocket);
}
