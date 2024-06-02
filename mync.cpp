#include "sockets.cpp"
using namespace std;

// will execute the ttt game by -e flag in the command line
int exe_ttt(char *eflag)
{
    string add_to_command = eflag;
    string command = "./" + add_to_command;
    execlp("bash", "bash", "-c", command.c_str(), NULL);
    return 0;
}

// this function will determine the type of the socket that we will want to open.
// will be responsoble to tell if its - udp/tcp/uds socket.
char *determine_type(char *optarg)
{
    const char *validStrings[] = {
        "TCPS", "TCPC", "UDPS", "UDPC", "UDSSD", "UDSCD", "UDSSS", "UDSCS"};
    for (const auto &validString : validStrings)
    {
        if (strncmp(optarg, validString, 3) == 0)
        {
            return optarg;
        }
    }
    return nullptr;
}

// this function will be called if we call mync with the
// i flag to run the code and want to use the tcp functionality.
void tcp_i(bool isServer, char *port)
{
    int clintsock = 0;
    int serverSocket = 0;
    if (isServer) // server case
    {
        serverSocket = server_tcp_socket(atoi(port));
        clintsock = accept(serverSocket, 0, 0);
    }
    else // client case
    {
        clintsock = client_tcp_socket(atoi(port));
    }
    dup2(clintsock, STDIN_FILENO);
}

// this function will be called if we call mync with the
// i flag to run the code and want to use the udp functionality.
void udp_i(char *port, char *timeout)
{
    int udp_socket;
    udp_socket = server_udp_socket(atoi(port));
    if (timeout != nullptr)
        alarm(atoi(timeout));
    dup2(udp_socket, STDIN_FILENO);
}

// this function will be called if we call mync with the
// i flag to run the code and want to use the uds data gram functionality.
void udsd_i(char *path, char *timeout)
{
    int udsd_socket = server_udsd(path);
    if (timeout != nullptr)
    {
        alarm(atoi(timeout));
    }
    dup2(udsd_socket, STDIN_FILENO);
}

void udsss_i(char *path)
{
    int clintsock = 0;
    int udsss_socket = server_udss(path);
    clintsock = accept(udsss_socket, NULL, NULL);
    if (clintsock == -1)
    {
        perror("accept error");
        close(udsss_socket);
        exit(-1);
    }
    dup2(clintsock, STDIN_FILENO);
}

void udscs_i(char *path)
{
    int udcs_socket;
    udcs_socket = client_udss(path);
    sockaddr_un serverAddress;
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, path, sizeof(serverAddress.sun_path) - 1);
    connect(udcs_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    dup2(udcs_socket, STDIN_FILENO);
}

/* the  i flag handler, will be used if in the command line the i flag exist.
will transfer the input of the program by using the dup function.
*/
int i_flag(char *input, char *timeout)
{
    char *port;
    bool isServer = (input[3] == 'S');
    if (isServer)
    {
        port = input + 4;
    }
    else
    {
        port = input + 14;
    }
    char *type = determine_type(input);
    if (strncmp(type, "TCP", 3) == 0) // tcp part (q3)
    {
        tcp_i(isServer, port);
    }
    else if (strncmp(type, "UDP", 3) == 0) // udp part (q4)
    {
        udp_i(port, timeout);
    }
    else if (strncmp(type, "UDSSD", 5) == 0) // uds dgram (q6)
    {
        udsd_i(input + 5, timeout);
    }
    else if (strncmp(type, "UDSSS", 5) == 0) // uds stream server (q6)
    {
        udsss_i(input + 5);
    }
    else if (strncmp(type, "UDSCS", 5) == 0) // uds stream server (q6)
    {
        udscs_i(input + 5);
    }
    return 0;
}

// will be responisble for the tcp part of the code of the -o flag
void tcp_o(bool isServer, char *port)
{
    int clintsock = 0;
    int serverSocket = 0;
    if (isServer) // server case
    {
        serverSocket = server_tcp_socket(atoi(port));
        clintsock = accept(serverSocket, 0, 0);
    }
    else // client case
    {
        clintsock = client_tcp_socket(atoi(port));
    }
    dup2(clintsock, STDOUT_FILENO);
}

//// will be responisble for the udp part of the code of the -o flag!
// only client can connect with the -o flag.
void udp_o(char *port, char *timeout)
{
    int udp_socket;
    udp_socket = client_udp_socket("127.0.0.1", atoi(port));
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(port));
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        cout << "Error: Invalid address/ Address not supported" << std::endl;
        close(udp_socket);
        exit(1);
    }
    if (timeout != nullptr)
        alarm(atoi(timeout));
    connect(udp_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    dup2(udp_socket, STDOUT_FILENO);
}

//// will be responisble for the uds data gram part of the code of the -o flag!
// only client can connect with the -o flag and uds datagram.
void udcd_o(char *path, char *timeout)
{
    int udcd_socket;
    udcd_socket = client_udsd(path);
    sockaddr_un serverAddress;
    serverAddress.sun_family = AF_INET;
    strncpy(serverAddress.sun_path, path, sizeof(serverAddress.sun_path) - 1);
    if (timeout != nullptr)
        alarm(atoi(timeout));
    connect(udcd_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    dup2(udcd_socket, STDOUT_FILENO);
}

void udsss_o(char *path)
{
    cout << path;
    int clintsock = 0;
    int udsss_socket = server_udss(path);
    clintsock = accept(udsss_socket, NULL, NULL);
    if (clintsock == -1)
    {
        perror("accept error");
        close(udsss_socket);
        exit(-1);
    }
    dup2(clintsock, STDOUT_FILENO);
}

void udscs_o(char *path)
{
    int udcs_socket;
    udcs_socket = client_udss(path);
    sockaddr_un serverAddress;
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, path, sizeof(serverAddress.sun_path) - 1);
    connect(udcs_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    dup2(udcs_socket, STDOUT_FILENO);
}

/* The o flag handler, will be used if in the command line the o flag exist.
will transfer the output of the program by using the dup function.
*/
int o_flag(char *output, char *timeout)
{
    char *port;
    bool isServer = (output[3] == 'S');
    if (isServer)
    {
        port = output + 4;
    }
    else
    {
        port = output + 14;
    }
    char *type = determine_type(output);
    if (strncmp(type, "TCP", 3) == 0) // tcp part (q3)
    {
        tcp_o(isServer, port);
    }
    else if (strncmp(type, "UDP", 3) == 0) // udp part (q4)
    {
        udp_o(port, timeout);
    }
    else if (strncmp(type, "UDSCD", 5) == 0) // udsd part (q6)
    {
        udcd_o(output + 5, timeout);
    }
    else if (strncmp(type, "UDSSS", 5) == 0) // uds stream server (q6)
    {
        udsss_o(output + 5);
    }
    else if (strncmp(type, "UDSCS", 5) == 0) // uds stream server (q6)
    {
        udscs_o(output + 5);
    }
    return 0;
}

void udsss_b(char *path)
{
    cout << path;
    int clintsock = 0;
    int udsss_socket = server_udss(path);
    clintsock = accept(udsss_socket, NULL, NULL);
    if (clintsock == -1)
    {
        perror("accept error");
        close(udsss_socket);
        exit(-1);
    }
    dup2(clintsock, STDIN_FILENO);
    dup2(clintsock, STDOUT_FILENO);
}

void udscs_b(char *path)
{
    int udcs_socket;
    udcs_socket = client_udss(path);
    sockaddr_un serverAddress;
    serverAddress.sun_family = AF_UNIX;
    strncpy(serverAddress.sun_path, path, sizeof(serverAddress.sun_path) - 1);
    connect(udcs_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    dup2(udcs_socket, STDIN_FILENO);
    dup2(udcs_socket, STDOUT_FILENO);
}

/* The b flag handler, will be used if in the command line the b flag exist.
will transfer the input and the output of the program by using the dup function.
*/
int b_flag(char *both)
{
    char *port;
    int clintsock;
    int serverSocket;
    char *type = determine_type(both);
    bool isServer = (both[3] == 'S');
    if (isServer)
    {
        port = both + 4;
    }
    else
    {
        port = both + 14;
    }
    if (strncmp(type, "TCP", 3) == 0)
    {
        if (isServer) // server case
        {
            serverSocket = server_tcp_socket(atoi(port));
            clintsock = accept(serverSocket, 0, 0);
        }
        else // client case
        {
            clintsock = client_tcp_socket(atoi(port));
        }
        dup2(clintsock, STDIN_FILENO);
        dup2(clintsock, STDOUT_FILENO);
    }
    else if (strncmp(type, "UDSSS", 5) == 0) // uds stream server (q6)
    {
        udsss_b(both + 5);
    }
    else if (strncmp(type, "UDSCS", 5) == 0) // uds stream server (q6)
    {
        udscs_b(both + 5);
    }
    return 0;
}

/*will be responsible for splitig each command that comes after the flag.
note that more than one flag can be used and the getopt function will hendle
all the flags that exist in the command line(even more than one).
*/

void flags_detector(int argc, char *argv[], char *&eflag, char *&input, char *&output, char *&both, int &opt, char *&timeout)
{
    while ((opt = getopt(argc, argv, "e:i:o:b:t:")) != -1)
    {
        switch (opt)
        {
        case 'e':
            eflag = optarg;
            break;
        case 'i':
            input = optarg;
            break;
        case 'o':
            output = optarg;
            break;
        case 'b':
            both = optarg;
            break;
        case 't':
            timeout = optarg;
            break;
        default:
            break;
        }
    }
}

/*will be used to transfer data when the parametr e is not writen.
will use the select function to modify the blocking of the read and write functions to read
from and write to a file discreptor.
*/
void transfer_data(int src_socket, int dest_socket)
{
    char buffer[1024];
    ssize_t bytes_read, bytes_written;
    fd_set read_fds;
    int max_fd = src_socket > dest_socket ? src_socket : dest_socket;
    while (true)
    {
        FD_ZERO(&read_fds);
        FD_SET(src_socket, &read_fds);
        FD_SET(dest_socket, &read_fds);
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0)
        {
            perror("select");
            break;
        }
        if (FD_ISSET(src_socket, &read_fds))
        {
            bytes_read = read(src_socket, buffer, sizeof(buffer));
            if (bytes_read <= 0)
                break;
            bytes_written = write(dest_socket, buffer, bytes_read);
            if (bytes_written <= 0)
                break;
        }
        if (FD_ISSET(dest_socket, &read_fds))
        {
            bytes_read = read(dest_socket, buffer, sizeof(buffer));
            if (bytes_read <= 0)
                break;
            bytes_written = write(src_socket, buffer, bytes_read);
            if (bytes_written <= 0)
                break;
        }
    }
}

/*
will run a chat like comunication that will allow 2 sides of mync to comunicate.(like in nc).
 */
void chat_handler(char *socket_type)
{
    char type[5] = {0};
    char *port = socket_type + 4;
    int sock, clintsock;
    strncpy(type, socket_type, 4);
    type[4] = '\0';
    if (strcmp(type, "TCPS") == 0)
    {
        sock = server_tcp_socket(atoi(port));
        clintsock = accept(sock, NULL, NULL);
        if (clintsock < 0)
        {
            perror("accept");
            close(sock);
            return;
        }
        transfer_data(clintsock, STDIN_FILENO);
        close(clintsock);
        close(sock);
    }
    else if (strcmp(type, "TCPC") == 0)
    {
        sock = client_tcp_socket(atoi(port));
        if (sock < 0)
        {
            perror("connect");
            return;
        }
        transfer_data(STDIN_FILENO, sock);
        close(sock);
    }
}

/* will use the flags that is passed from the getopt function and will use the function that corespond to this flag
in the flag list that we allow on this ex.
*/
void run(int argc, char *argv[])
{
    char *eflag = NULL;
    char *input = NULL;
    char *output = NULL;
    char *both = NULL;
    char *timeout = NULL;
    int opt = 0;
    flags_detector(argc, argv, eflag, input, output, both, opt, timeout);
    if (eflag == NULL)
    {
        chat_handler(argv[1]);
    }
    else
    {
        if (input != NULL)
        {
            i_flag(input, timeout);
        }
        if (output != NULL)
        {
            o_flag(output, timeout);
        }
        if (both != NULL)
        {
            b_flag(both);
        }
        if (eflag != NULL)
        {
            exe_ttt(eflag);
        }
    }
}

/*
main function to run the program.
will call the flags_handler function that will use each flag that the command line includes.
*/
int main(int argc, char *argv[])
{
    run(argc, argv);
    return 0;
}
