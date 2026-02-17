#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <fcntl.h>

int main(){
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    sockaddr_in local_addr{};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(5000);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    bind(socket_fd, (sockaddr*)&local_addr, sizeof(local_addr));

    // join multicast group
    ip_mreq mreq{};
    inet_pton(AF_INET, "239.1.1.1", &mreq.imr_multiaddr);
    mreq.imr_interface.s_addr = INADDR_ANY;

    setsockopt(socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    fcntl(socket_fd, F_SETFL, O_NONBLOCK);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) { 
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    epoll_event event;
    event.events = EPOLLIN;          // we need READs
    event.data.fd = socket_fd;       // from this fd

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &event) < 0) {
        perror("epoll_ctl error");
        exit(EXIT_FAILURE);
    }

    constexpr int MAX_EVENTS = 10;
    epoll_event events[MAX_EVENTS];
    char buffer[2048];

    while (true) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == socket_fd) {

                while (true) {
                    ssize_t n = recv(socket_fd, buffer, sizeof(buffer), 0);

                    if (n < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        else
                            perror("recv");
                    }

                    std::cout << "Recieved: " << buffer << std::endl;
                }
            }
        }
    }
}