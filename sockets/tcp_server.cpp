#pragma once

#include "tcp_server.h"

namespace Common
{
    auto TCPServer::destroy() {
        close(efd_);
        efd_ = -1;
        listener_socket_.destory();
    }

    auto TCPServer::addToEpollList(TCPSocket *socket) noexcept -> bool {
        // EPOLLIN - event when file is ready for read
        // EPOLLET - edge-triggered? notify only once, when data needs to be read
        // pointer - event returns pointer, data at hand
        epoll_event ev{EPOLLET | EPOLLIN, {reinterpret_cast<void *>(socket)}};
        return !epoll_ctl(efd_, EPOLL_CTL_ADD, socket->fd_, &ev);
    }

    auto TCPServer::removeFromEpollList(TCPSocket *socket) noexcept -> bool {
        return epoll_ctl(efd_, EPOLL_CTL_DEL, socket->fd_, nullptr);
    }

    auto TCPServer::deleteSocket(TCPSocket *socket) noexcept -> void {
        removeFromEpollList(socket);

        // Erase-remove
        sockets_.erase(std::remove(sockets_.begin(), sockets_.end(), socket), sockets_.end());
        recv_sockets_.erase(std::remove(recv_sockets_.begin(), recv_sockets_.end(), socket), recv_sockets_.end());
        send_sockets_.erase(std::remove(send_sockets_.begin(), send_sockets_.end(), socket), send_sockets_.end());
    }

    auto TCPServer::poll() noexcept -> void {
        
    }

    auto TCPServer::listen(const std::string &iface, int port) -> void {
        destroy();

        // Creates epoll for events
        efd_ = epoll_create(1);
        //ASSERT(efd_ >= 0, "epoll_create() failed:" + std::string(std::strerror(errno)));

        //ASSERT(listener_socket_.connect("", iface, port, true) >= 0, "connect() failed:" + std::string(std::strerror(errno))); // Add rest of the params
        listener_socket_.connect("", iface, port, true);

        //ASSERT(!addToEpollList(&listener_socket_), "epoll_ctl() failed:" + std::string(std::strerror(errno))); // Add rest of the params
        addToEpollList(&listener_socket_);
    }
}
