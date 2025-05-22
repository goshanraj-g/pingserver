#include <iostream> // For std::cout, std::cerr, etc.
#include <string>   // For std::string
#include <cstring>  // For std::strcmp, std::strlen, std::memset
#include <thread>   // For std::thread
#include <vector>   // For std::vector
#include <mutex>    // For std::mutex, std::lock_guard
#include <atomic>   // For std::atomic<bool>

// 🧠 Socket-related headers (Linux-specific):
#include <sys/socket.h> // For socket(), bind(), connect(), recv(), send()
#include <netinet/in.h> // For sockaddr_in, htons(), htonl()
#include <arpa/inet.h>  // For inet_pton(), inet_ntoa()
#include <unistd.h>     // For close() to close sockets
#include <errno.h>      // For error codes like errno
