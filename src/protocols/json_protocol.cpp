#include <iostream>
#include "json_protocol.h"
#include "../commands/command_factory.h"


JsonProtocol::JsonProtocol(ip::tcp::socket& socket):
    m_socket(socket)
{

}

void JsonProtocol::receive()
{
    std::vector<char> receive_buffer(1024);

    while ( m_socket.is_open() ) {
        boost::system::error_code ec{};
        int n = m_socket.read_some(
                    boost::asio::buffer(receive_buffer, receive_buffer.size() ),
                    ec
            );

        if(ec) {
            if(ec.value() == boost::asio::error::eof){
                break;
            } else {
                std::cout << "Read data error:" << ec << "\n";
                ec.clear();
            }
        }

        if ( n <= 0 ) {
            continue;
        }

        handle_received_bytes(receive_buffer);
    }
}

void JsonProtocol::handle_received_bytes(const std::vector<char>& recv_buffer)
{
    for (auto& ch : recv_buffer) {
        m_jp.parse(ch);

        if( m_jp.is_finished() ) {
            std::cout << "Packet has been recevied" << "\n";
            receive_command( CommandFactory::create( m_jp.get_ref() ) );
            m_jp.reset();
        }
    }
}

void JsonProtocol::receive_command(const CommandPtr p_command)
{
    boost::system::error_code wec{};
    size_t n = 0;

    if ( auto p_response = p_command->execute() ) {
        auto ser_response = Serialize(p_response);
        n = m_socket.write_some(
                    boost::asio::buffer(ser_response, ser_response.size() ),
                    wec
            );
    } else {
        ResponsePtr err_response = std::make_shared<Response>(
                        CommandParams({{ "status", std::to_string(CommandStatus::ERROR) }})
                    );
        auto ser_err = Serialize(err_response);
        n = m_socket.write_some(
                    boost::asio::buffer(ser_err, ser_err.size() ),
                    wec
        );
    }

    if (!n) {
        std::cout << "Write data error:" << wec << "\n";
    }
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
