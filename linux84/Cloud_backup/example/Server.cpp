#include "httplib.h"


void Hello(const httplib::Request& req, httplib::Response& res)
{
    res.set_content("Hello World!", "text/plain");
    res.status = 200;
}

void Number(const httplib::Request& req, httplib::Response& res)
{
    auto num = req.matches[1];//0保存整个path，往后下标保存的是捕捉的数据
    res.set_content(num, "text/plain");
    res.status = 200;
}

void Multipart(const httplib::Request& req, httplib::Response& res)
{
    auto size = req.files.size();
    auto ret = req.has_file("file");
    const auto& file = req.get_file_value("file");
    res.body.clear();
    res.body = file.filename;
    res.body += "\n";
    res.body += file.content;
    res.set_header("Content-Type", "text/plain");
    res.status = 200;
}

int main()
{
    httplib::Server svr;//实例化一个Server对象，用于搭建服务器

    svr.Get("./hello", Hello);//注册一个针对./hello的Get请求的处理函数映射关系

    svr.Get("R(./numbers/(\d+))", Number);

    svr.Post("/multipart", Multipart);

    svr.listen("0.0.0.0", 8848);
    return 0;
}


