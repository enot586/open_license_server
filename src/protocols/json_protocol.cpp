#include <iostream>
#include "json_protocol.h"
#include "../commands/command_factory.h"


JsonProtocol::JsonProtocol(ip::tcp::socket& socket):
    m_socket(socket)
{

}

CommandPtr JsonProtocol::receive()
{
    std::vector<char> receive_buffer(1024);

    while ( m_socket.is_open() ) {
        boost::system::error_code ec{};
        int n = m_socket.read_some(
                    boost::asio::buffer(receive_buffer, receive_buffer.size() ),
                    ec
            );

        if(ec) {
            if(ec.value() == boost::asio::error::eof) {
                break;
            } else {
                std::cout << "Read data error:" << ec << "\n";
                ec.clear();
            }
        }

        if ( n <= 0 ) {
            continue;
        }

        for (auto& ch : receive_buffer) {
            m_jp.parse(ch);

            if( m_jp.is_finished() ) {
                std::cout << "Packet has been recevied" << "\n";
                auto request = CommandFactory::create( m_jp.get_ref() );
                m_jp.reset();
                return request;
            }
        }
    }

    return {};
}

std::string JsonProtocol::Serialize(const CommandPtr p_command)
{
    std::string result;

    result+= "{";
    for (auto const& [key, val] : p_command->get_params() )
    {
        if (result.size() > 1)
            result+= ",";
        result+= key+":"+val;
    }
    result+= "}";

    return result;
}
