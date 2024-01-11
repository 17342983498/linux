#include "tcp_server.hpp"
#include <memory>


using namespace std;
using namespace ns_server;



static void usage(string proc)
{
    cout << "Usage\t\t" << proc << " prot\n" << endl;
}

string echo(string message)
{
    return message;
}

int main(int argc, char* argv[])
{
    if (argc != 2) 
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<TcpServer> tsvr(new TcpServer(echo, port));

    tsvr->InitServer();
    tsvr->Start();

    return 0;
}