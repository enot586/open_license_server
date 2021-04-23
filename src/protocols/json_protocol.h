#pragma once

#include <boost/asio.hpp>

#include "protocol.h"
#include "../json_parser.h"
#include "../commands/command.h"

using namespace std;
using namespace boost::asio;

class JsonProtocol : public Protocol
{
public:
    JsonProtocol(ip::tcp::socket& socket);

    // Protocol interface
    CommandPtr receive() override;
    std::string Serialize(const CommandPtr p_command) override;

protected:
    void handle_received_bytes(const std::vector<char>& recv_buffer);

protected:
    ip::tcp::socket& m_socket;
    JsonParser m_jp;
};
