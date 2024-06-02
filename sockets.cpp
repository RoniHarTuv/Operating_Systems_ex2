#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <sys/wait.h>
#include <fcntl.h>
#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>
#include <sys/un.h>
using namespace std;

// .------..------..------.
// |T.--. ||C.--. ||P.--. |
// | :/\: || :/\: || :/\: |
// | (__) || :\/: || (__) |
// | '--'T|| '--'C|| '--'P|
// `------'`------'`------'

// will open a server tcp socket to transfer the data from one end to another.
int server_tcp_socket(int port)
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cout << "Error";
        exit(1);
    }
    listen(serverSocket, 1);
    return serverSocket;
}

// will open a client tcp socket to transfer the data from one end to another.
int client_tcp_socket(int port)
{
    int clientSocket;
    struct sockaddr_in client_addr;
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    memset(&(client_addr.sin_zero), '\0', 8);
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(port);
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(clientSocket, (struct sockaddr *)&client_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect");
        exit(1);
    }
    return clientSocket;
}

// .------..------..------.
// |U.--. ||D.--. ||P.--. |
// | (\/) || :/\: || :/\: |
// | :\/: || (__) || (__) |
// | '--'U|| '--'D|| '--'P|
// `------'`------'`------'

// will open a udp server socket
int server_udp_socket(int port)
{
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0)
    {
        std::cerr << "Error: Unable to create socket" << std::endl;
        exit(1);
    }
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        std::cerr << "Error: Unable to bind socket" << std::endl;
        close(serverSocket);
        exit(1);
    }
    return serverSocket;
}

// will open a udp client socket.
int client_udp_socket(const char *server_ip, int port)
{
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Error: Unable to create socket" << std::endl;
        exit(1);
    }
    return clientSocket;
}

// .------..------..------.
// |U.--. ||D.--. ||S.--. |
// | (\/) || :/\: || :/\: |
// | :\/: || (__) || :\/: |
// | '--'U|| '--'D|| '--'S|
// `------'`------'`------'

int server_udsd(const char *path)
{
    int serverSocket;
    struct sockaddr_un addr;
    if ((serverSocket = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket error");
        exit(-1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    unlink(path);
    if (bind(serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind error");
        close(serverSocket);
        exit(-1);
    }
    return serverSocket;
}

int client_udsd(const char *path)
{
    int clientSocket = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Error: Unable to create socket" << std::endl;
        exit(1);
    }
    struct sockaddr_un serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, path, sizeof(serverAddress.sun_path) - 1);
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("connect error");
        close(clientSocket);
        exit(1);
    }
    return clientSocket;
}

int server_udss(const char *path)
{
    int serverSocket;
    struct sockaddr_un addr;
    if ((serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        exit(-1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path, sizeof(addr.sun_path) - 1);
    unlink(path);
    if (bind(serverSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        perror("bind error");
        close(serverSocket);
        exit(-1);
    }
    if (listen(serverSocket, 5) == -1)
    {
        perror("listen error");
        close(serverSocket);
        exit(-1);
    }
    return serverSocket;
}

int client_udss(const char *path)
{
    int clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (clientSocket < 0)
    {
        std::cerr << "Error: Unable to create socket. Errno: " << errno << std::endl;
        exit(1);
    }
    struct sockaddr_un serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, path, sizeof(serverAddress.sun_path) - 1);
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cerr << "Error: Unable to connect to server. Errno: " << errno << std::endl;
        perror("connect error");
        close(clientSocket);
        exit(1);
    }
    return clientSocket;
}