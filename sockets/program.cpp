#include "socket_utils.h"
#include "tcp_socket.h"
#include "logging.h"


int main()
{
    using namespace Common;

    std::string time_str_;
    std:: string logger_file_name_ = "socket_log.log";
    
    Logger logger_(logger_file_name_);


     auto tcpServerRecvCallback = [&](TCPSocket *socket, Nanos rx_time) noexcept {
        logger_.log("TCPServer::defaultRecvCallback() socket:% len:% rx:%\n",
                    socket->fd_, socket->next_rcv_valid_index_, rx_time);

        //const std::string reply = "TCPServer received msg:" + std::string(socket->inbound_data_.data(), socket->next_rcv_valid_index_);
        //socket->next_rcv_valid_index_ = 0;

        //socket->send(reply.data(), reply.length());
    };

    return 0;
}
