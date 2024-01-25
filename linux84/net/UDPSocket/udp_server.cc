#include "udp_server.hpp"
#include <memory>

using namespace NS_SERVER;
using namespace std;

static void usage(string proc)
{
    cout << "Usage\t\t" << proc << " prot\n" << endl;
}

//上层的业务处理，不关心网络发送，只负责信息处理即可
// string transactionString(const string request)
// {
//     string result;
//     char c;
//     for (auto& r : request)
//     {
//         if ((r >= 97 && r <= 122) && islower(r))
//         {
//             c = toupper(r);
//             result.push_back(c);
//         }
//         else
//         {
//             result.push_back(r);
//         }
//     }
//     return result;
// }


bool isPass(string command)
{
    bool pass = true;
    auto pos = command.find("rm");
    if (pos != string::npos) pass = false;
    pos = command.find("mv");
    if (pos != string::npos) pass = false;
    pos = command.find("kill");
    if (pos != string::npos) pass = false;
    return pass;
}

string excuteCommand(string command)
{
    if (!(isPass(command))) return "you are a bad man!";

    FILE *fp = popen(command.c_str(), "r");
    if (fp == nullptr) return "None";

    char line[2048];
    string result;
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        result += line;
    }
    pclose(fp);
    return result;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    unique_ptr<UdpServer> usvr(new UdpServer(port));
    usvr->InitServer();
    //usvr->Start();

    return 0;
}

