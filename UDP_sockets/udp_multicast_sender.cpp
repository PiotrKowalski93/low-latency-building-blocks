#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>

int main(){
    int multicast_socket_fd_ = socket(AF_INET, SOCK_DGRAM, 0);

    int ttl = 1;
    setsockopt(multicast_socket_fd_, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

    sockaddr_in multicast_addr{};
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(5000);
    inet_pton(AF_INET, "239.1.1.1", &multicast_addr.sin_addr);

    std::cout << "Publisher started." << std::endl;

    std::string msg;

    while(true){
        std::cout << "Type and press ENTER to send: ";
        std::getline(std::cin, msg);
        if (msg.empty()) continue;

        sendto(multicast_socket_fd_, msg.data(), msg.size(), 0, (sockaddr*)&multicast_addr, sizeof(multicast_addr));
    }
}