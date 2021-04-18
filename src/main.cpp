#include <iostream>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "communicator.h"

using namespace std;
using namespace boost::asio;

io_service service;

int main()
{
    cout << "Server start" << "\n";

    Communicator cm(service, "127.0.0.1", 2001);

    do {
        cm.handle_client();
    } while(true);

    cout << "Server is down" << "\n";
    return 0;
}
