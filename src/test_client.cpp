#include <iostream>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>
#include <vector> 

using namespace std;
using namespace boost::asio;

io_service service;
ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 2001);
ip::tcp::socket s(service);

std::vector<ip::tcp::endpoint> eps = {ep};

int main()
{
    cout << "client start!" << "\n";
    cout.flush();
    std::string packet;

    std::getline(cin,packet);

    async_connect(s, eps.begin(), eps.end(), [&](const boost::system::error_code& ec,
                                                std::vector<ip::tcp::endpoint>::iterator it) {
        std::cout << "Connect!" << "\n";

        if( packet.empty() )
            packet = "{ \"a\":5, \"vov\":342 }";

        async_write(s, buffer(packet, packet.size()), [](const boost::system::error_code& ec,
                                size_t bytes_transferred  ) {
            std::cout << "Send packet!" << "\n";
        });

    });

    service.run();

    return 0;
}
