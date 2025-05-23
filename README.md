# Ping Server

A simple TCP-based **ping server** built using **C++ socket programming** and **multithreading**.

This project was made to further my knowledge of how sockets work and how to handle multiple clients with threads.

---

##  Technologies Used

- C++
- POSIX Sockets (Linux, WSL)
- `std::thread` for concurrency

---

## 🧪 How to Run

1. **Compile the server and client:**

```bash
g++ ping_server.cpp -o server -pthread
g++ ping_client.cpp -o client
