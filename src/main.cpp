#include <iostream>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include "communicator.h"

using namespace std;
using namespace boost::asio;

io_service service;

int main()
{
    cout << "Server start" << "\n" << flush;

    Communicator cm(service, "127.0.0.1", 2001);

    cm.handle_client();

    service.run();

    cout << "Server is down" << "\n" << flush;
    return 0;
}
