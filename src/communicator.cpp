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
    ip::tcp::socket current_socket(m_service);
    ip::tcp::acceptor acceptor(m_service, m_ep);

    acceptor.async_accept(current_socket, [&](const boost::system::error_code& error) {
        std::cout << "Client accepted:" << "\n";

        m_protocol = std::make_shared<JsonProtocol>(current_socket);

        if (error) {
            std::cout << "Connecion failed:" << error << "\n";
        } else {
            std::cout << "Connecion successed:" << "\n";
        }

        m_protocol->receive();

        std::cout << "Connection has been closed:" << "\n";
    });

    m_service.run();
    m_service.reset();
}
