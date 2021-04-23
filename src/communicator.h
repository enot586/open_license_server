#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <memory>

#include <boost/asio.hpp>

#include "json_parser.h"
#include "commands/command.h"
#include "protocols/protocol.h"

using namespace std;
using namespace boost::asio;

class Communicator
{
public:
    Communicator(io_service& service, const std::string& ip_address,
                 unsigned short port);

    void handle_client();

protected:
    void process(const CommandPtr p_command);

private:
    io_service& m_service;
    ProtocolPtr m_protocol;
    ip::tcp::endpoint m_ep;
    shared_ptr<ip::tcp::socket> m_socket;
    shared_ptr<ip::tcp::acceptor> m_acceptor;
};
