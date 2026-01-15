#pragma once

#include <iostream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <sstream>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <fcntl.h>

#include "logging.h"

// A network interface is the boundary between the kernel’s networking stack and the physical hardware that transmits data over the network.

// A socket is a software abstraction for communication, while a network interface is the boundary where the kernel sends data to the physical network hardware.

namespace Common{
    constexpr int MaxTCPServerBacklog = 1024;

    // Maps network interface to IPv4 without DNS, so network is not used.
    // It works locally
    auto getIfaceIP(const std::string &iface) -> std::string{
        char buf[NI_MAXHOST] = {'\0'};
        ifaddrs *ifaddr = nullptr;

        // Iterate throught network interfaces and finds 'iface' ex. eth0
        if (getifaddrs(&ifaddr) != -1) 
        {
            for (ifaddrs *ifa = ifaddr; ifa; ifa = ifa->ifa_next) 
            {
                if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET && iface == ifa->ifa_name) 
                {
                    // translates to ip 123.123.123.123
                    getnameinfo(ifa->ifa_addr, sizeof(sockaddr_in), buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);
                    break;
                }
            }
            freeifaddrs(ifaddr);
        }

        return buf;
    }

    auto setNonBlocking(int fd) -> bool{
        const auto flags = fcntl(fd, F_GETFL, 0);

        if (flags & O_NONBLOCK) return true;

        return (fcntl(fd, F_SETFL, flags | O_NONBLOCK) != -1);
    }

    //TODO: 
    auto setNoDelay(int fd) -> bool;
    auto setSOTimestamp(int fd) -> bool;
    auto wouldBlock() -> bool;
    auto setMcastTTL(int fd, int ttl) -> bool;
    auto setTTL(int fd, int ttl) -> bool;
    auto join(int fd, const std::string &ip, const std::string &iface, int port) -> bool;
    auto createSocket(Logger &logger, const std::string &t_ip, const std::string &iface, int port, bool is_udp, bool is_blocking, 
        bool is_listening, int ttl, bool needs_so_timestamp) -> int;    
}