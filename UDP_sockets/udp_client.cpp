#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <arpa/inet.h>

#define PORT        8080
#define BUFF_SIZE   1024

int main(){
    int socket_fd;
    char buffer[BUFF_SIZE];

    const char* hello = "Hello from client";
    sockaddr_in server_addr;

    // Create UDP socket
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd < 0){
        std::cout << "Failed creating socket.";
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Prepare server address info
    server_addr.sin_family = AF_INET;                       //IPv4
    server_addr.sin_port = htons(PORT);                     //8080
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   //localhost

    socklen_t len = sizeof(server_addr);

    // Send message to server
    sendto(socket_fd, hello, strlen(hello), MSG_CONFIRM,(const sockaddr*)&server_addr, sizeof(server_addr));
    
    printf("Hello message sent.\n");
}