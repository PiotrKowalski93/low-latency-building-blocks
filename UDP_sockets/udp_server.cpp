#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/epoll.h>

#define PORT        8080 
#define BUFF_SIZE   1024 

auto run_loop(int socket_fd, char* buffer, sockaddr_in* client_addr) -> void{
    socklen_t len;
    len = sizeof(client_addr);

    //While is 100% CPU, loops all the time
    while (true) {
        int n = recvfrom(socket_fd, buffer, BUFF_SIZE, 0, (sockaddr*)client_addr, &len);

        if (n < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue; 
            } else {
                perror("recvfrom error");
                break;
            }
        }

        buffer[n] = '\0';
        std::cout << "Client: " << buffer << std::endl;
    }
}

auto run_loop_with_select(int socket_fd, char* buffer, sockaddr_in* client_addr) -> void{
    while(true){
        fd_set readfds;

        // Clear fds set
        FD_ZERO(&readfds);
        // Set fd that interests me
        FD_SET(socket_fd, &readfds);
        
        // Wait indefinetaly
        int activity = select(socket_fd + 1, &readfds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            perror("select() error, activity < 0");
            break;
        }

        if (FD_ISSET(socket_fd, &readfds)) {
            socklen_t len = sizeof(client_addr);

            int n = recvfrom(socket_fd, buffer, BUFF_SIZE, 0,(sockaddr*)client_addr, &len);

            if (n < 0) {
                perror("recvfrom error");
                continue;
            }

            buffer[n] = '\0';
            std::cout << "Client: " << buffer << std::endl;
        }
    }
}

auto run_with_epoll_EPOLLIN(int socket_fd, char* buffer, sockaddr_in* client_addr) -> void{
    int epoll_fd = epoll_create1(0);

    if (epoll_fd < 0) { 
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    epoll_event event;
    // level-triggered
    event.events = EPOLLIN;          // we need READs
    // edge-triggered
    // event.events = EPOLLET;
    event.data.fd = socket_fd;       // from this fd

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) {
        perror("epoll_ctl error");
        exit(EXIT_FAILURE);
    }

    constexpr int MAX_EVENTS = 10;
    epoll_event events[MAX_EVENTS];

    while(true){
        // Returns number of fds ready for I/O
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); // -1 - indefinetly timeout

        if (nfds < 0) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == socket_fd) {
                socklen_t len = sizeof(client_addr);
                int n = recvfrom(socket_fd, buffer, BUFF_SIZE, 0, (sockaddr*)client_addr, &len);

                if (n < 0) {
                    perror("recvfrom");
                    continue;
                }

                buffer[n] = '\0';
                std::cout << "Client: " << buffer << std::endl;
            }
        }
    }
}

int main(){
    int socket_fd;
    char buffer[BUFF_SIZE];

    const char* hello_msg = "Hello from Server";
    sockaddr_in server_addr{};
    sockaddr_in client_addr{};

    // Create file descriptor
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd < 0){
        std::cout << "Socket creation failed.";
        exit(EXIT_FAILURE);
    }

    //Set to non blocking
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    // Server info setup
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket with server address
    if(bind(socket_fd, (const sockaddr*)&server_addr, sizeof(server_addr)) < 0 ){
        std::cout << "Bind failed.";
        exit(EXIT_FAILURE);
    }

    std::cout << "Started listening on port: " << std::to_string(PORT) << std::endl;    

    //run_loop(socket_fd, buffer, &client_addr);    
    //run_loop_with_select(socket_fd, buffer, &client_addr);
    run_with_epoll_EPOLLIN(socket_fd, buffer, &client_addr);
}