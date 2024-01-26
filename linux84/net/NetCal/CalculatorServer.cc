#include "TcpServer.hpp"
#include <memory>
using namespace  Protocaol_ns;

//完成计算任务
Response calculate(const Request& req)
{
    Response resp(0, 0);
    switch (req._op)
    {
    case '+':
        resp._result = req._x + req._y; 
        break;
    case '-':
        resp._result = req._x - req._y; 
        break;
    case '*':
        resp._result = req._x * req._y; 
        break;
    case '/':
        if (req._y == 0)
        {
            resp._code = 1; // 1代表计算出错
        }
        else
            resp._result = req._x / req._y;
        break;
    case '%':
        if (req._y == 0)
        {
            resp._code = 2; // 1代表计算出错
        }
        else
            resp._result = req._x % req._y;
        break;
    default:
        resp._code = 3;
        break;
    }
    return resp;
}


int main()
{
    uint16_t port = 8848;
    std::unique_ptr<ns_tcpserver::TcpServer> tsvr(new ns_tcpserver::TcpServer(calculate, port));
    tsvr->InitServer();
    tsvr->Start();
    return 0;
}