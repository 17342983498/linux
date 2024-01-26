#include "TcpClient.hpp"
#include "Sock.hpp"
#include <iostream>
#include <string>
#include "Protocaol.hpp"

using namespace Protocaol_ns;

static void usage(std::string proc)
{
    std::cout << "Usage\t\t" << proc << " serverip serverprot\n" << std::endl;
}

enum
{
    LEFT,
    OPER,
    RIGHT
};

Request ParseLine(std::string line)
{
    std::string left, right;
    char op;
    int status = LEFT;
    int i = 0;
    while (i<line.size())
    {
        switch (status)
        {
        case LEFT:
            if (isdigit(line[i]))
                left.push_back(line[i++]);
            else
            {
                status = OPER;
            }
            break;
        case OPER:
            op = line[i++];
            status = RIGHT;
            break;
        case RIGHT:
            if (isdigit(line[i]))
                right.push_back(line[i++]);
            break;
        }
    }
    Request req(std::stoi(left), std::stoi(right), op);
    // Request req;
    // req._x = std::stoi(left);
    // req._y = std::stoi(right);
    // req._op = op;
    return req;
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        usage(argv[0]);
        exit(USAGE_ERR);
    }

    std::string serverip = argv[1];
    uint16_t serverport = atoi(argv[2]);
    Sock sock;
    sock.Socket();
    int n = sock.Connect(serverip, serverport);
    if (n != 0) return 1;

    while (true)
    {
        std::cout << "Please Enter# ";
        std::string line;
        getline(std::cin, line);

        Request req = ParseLine(line);
        // std::cout << "data1# ";
        // std::cin >> req._x;
        // std::cout << "data2# ";
        // std::cin >> req._y;
        // std::cout << "op# ";
        // std::cin >> req._op;
        std::cout << "test: " << req._x << req._op << req._y << std::endl;

        // 1.  序列化
        std::string send_string;
        req.Serialize(&send_string);
        // 2. 添加报头
        send_string = AddHeader(send_string);
        // 3. send
        send(sock.Fd(), send_string.c_str(), send_string.size(), 0);
        // 4. 获取响应
        std::string inbuffer;
        std::string package;
        int n = 0;
    START:
        n = ReadPackage(sock.Fd(), inbuffer, &package);
        if (n == 0) goto START;
        else if (n < 0) break;
        else
        {
            // 5. 去报头
            std::string payload = RemoveHeader(package, n);
            // 6. 反序列化
            Response resp;
            resp.Deserialize(payload);

            std::cout << "result: " << resp._result << "[code: " << resp._code << "]"  << std::endl;
        }
    }

    sock.Close();
    return 0;
}