#include "httpServer.hpp"
#include "Util.hpp"
#include <memory>
#include <vector>

const std::string SEP = "\r\n";
//一般一个webserver，不做特殊说明，如果用户直接默认访问‘/’，我们绝对不能把整个给对方
//需要添加默认首页，一般都是下面这个，而且不能让用户访问wwwroot里面的任何一个目录本身，也可以给每个目录都带上一个默认首页。
const std::string defaultHomePage = "index.html";
const std::string webRoot = "wwwroot"; //代表web根目录
const std::string page_404 = "./wwwroot/err-404.html";

class HttpRequest
{
public:
    HttpRequest():_path(webRoot)
    {}

    ~HttpRequest()
    {}

    void Print()
    {
        logMessage(Debug, "method: %s, url: %s, httpVersion: %s", _method.c_str(), _url.c_str(), _httpVersion.c_str());
        for (const auto& line : _body)
        {
            logMessage(Debug, "-%s", line.c_str());
        }
        logMessage(Debug, "path: %s", _path.c_str());
        logMessage(Debug, "suffix: %s", _suffix.c_str());
    }
    public:
    std::string _method;
    std::string _url;
    std::string _httpVersion;
    std::vector<std::string> _body;
    std::string _path;
    std::string _suffix;
};


HttpRequest Deseralize(std::string &message)
{
    HttpRequest req;
    std::string line = Util::ReadOneLine(message, SEP);
    Util::ParaRequestLine(line, &req._method, &req._url, &req._httpVersion);

    while (!message.empty())
    {
        std::string line = Util::ReadOneLine(message, SEP);
        req._body.push_back(line);
    }
    req._path += req._url;
    if (req._path[req._path.size() - 1] == '/') req._path += defaultHomePage;

    auto pos = req._path.rfind(".");
    if (pos == std::string::npos) req._suffix = ".html";
    else req._suffix = req._path.substr(pos);
        
    return req;
}

std::string GetContentType(const std::string &suffix)
{
    std::string content_type = "Content-Type: ";
    if (suffix == ".html" || suffix == ".htm") return content_type + "text/html";
    else if (suffix == ".css") content_type += "text/css";
    else if(suffix == ".js") content_type += "application/x-javascript"; 
    else if(suffix == ".jpg") content_type += "image/jpeg"; 
    else if(suffix == ".png") content_type += "image/png"; 
    return content_type + SEP;
}

std::string HandlerHttp(std::string &message)
{
    // 1. 先读取请求
    std::cout << "-----------------------------" << std::endl;
    //std::cout << message << std::endl;

    // 2. 反序列化和分析请求
    // HttpRequest req = Deseralize(message);
    // req.Print();
    // // 3. 使用请求 
    // std::string body;
    // // std::string path = webRoot;
    // // path += req._url;
    // std::string response;
    // if (true == Util::ReadFile(req._path, &body))
    // {
    //     response = "HTTP/1.0 200 OK" + SEP;
    //     response += "Content-Length: " + std::to_string(body.size()) + SEP;
    //     response += GetContentType(req._suffix);
    //     response += SEP;
    //     response += body;
    // }
    // else
    // {
    //     response = "HTTP/1.0 404 NOT FOUND" + SEP;
    //     Util::ReadFile(page_404, &body);
    //     response += "Content-Length: " + std::to_string(body.size()) + SEP;
    //     response += GetContentType(req._suffix);
    //     response += SEP;
    //     response += body;
    // }

    std::string response;
    response = "HTTP/1.0 302 FOUND" + SEP;
    response += "Location: https//www.qq.com/" + SEP;
    response += SEP;
    return response;
}


int main(int argc, char *argv[]) 
{
    if (argc != 2)
    {
        exit(USAGE_ERR);
    }
    uint16_t port = atoi(argv[1]);
    //uint16_t port = 8848;
    std::unique_ptr<httpServer> tsvr(new httpServer(HandlerHttp, port));
    tsvr->InitServer();
    tsvr->Start();
    return 0;
}