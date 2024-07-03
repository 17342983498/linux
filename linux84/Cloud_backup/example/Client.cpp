#include "httplib.h"


#define SERVER_IP "172.17.0.1"
#define SERVER_PORT 8848

int main()
{
    httplib::Client cli(SERVER_IP, SERVER_PORT);

    httplib::MultipartFormData item;
    item.name = "file";
    item.content = "hello world!";
    item.filename = "hello.txt";
    item.content_type = "text/plain";
    httplib::MultipartFormDataItems items;
    items.push_back(item);
    auto res = cli.Post("/multipart", items);
    std::cout << res->status << std::endl;
    std::cout << res->body << std::endl;
    return 0;
}