#include <string>

#include "communicator.h"
#include "commands/command_factory.h"
#include "protocols/json_protocol.h"

using namespace std;
using namespace boost::asio;


Communicator::Communicator(io_service& service, const std::string& ip_address,
                           unsigned short port) :
    m_service(service),
    m_ep{ ip::address::from_string(ip_address), port }
{

}

void Communicator::handle_client()
{
    std::cout << "Wait a new client:" << "\n";

    if (m_socket) {
        m_socket->close();
    }

    m_socket = make_shared<ip::tcp::socket>(m_service);

    if (m_acceptor) {
        m_acceptor->close();
    }

    m_acceptor = make_shared<ip::tcp::acceptor>(m_service, m_ep);

    m_acceptor->async_accept(*m_socket, [this](const boost::system::error_code& error) {
        std::cout << "Client accepted" << "\n"<< flush;

        if (error) {
            std::cout << "Connecion failed:" << error << "\n" << flush;
            return;
        } else {
            std::cout << "Connecion successed" << "\n" << flush;
        }

        m_protocol = std::make_shared<JsonProtocol>(*m_socket);

        if ( auto request = m_protocol->receive() ) {
            process(request);
        }

        std::cout << "Connection has been closed" << "\n" << flush;

        this->handle_client();
    });
}

void Communicator::process(const CommandPtr p_command)
{
    boost::system::error_code wec{};
    size_t n = 0;

    if ( auto p_response = p_command->execute() ) {
        auto ser_response = m_protocol->Serialize(p_response);
        n = m_socket->write_some(
                    boost::asio::buffer(ser_response, ser_response.size() ),
                    wec
            );
    } else {
        ResponsePtr err_response = std::make_shared<Response>(
                        CommandParams({{ "status", std::to_string(CommandStatus::ERROR) }})
                    );
        auto ser_err = m_protocol->Serialize(err_response);
        n = m_socket->write_some(
                    boost::asio::buffer(ser_err, ser_err.size() ),
                    wec
        );
    }

    if (!n) {
        std::cout << "Write data error:" << wec << "\n";
    }
}
