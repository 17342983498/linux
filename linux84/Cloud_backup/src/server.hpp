#ifndef __MY_SVR__
#define __MY_SVR__
#include <iostream>
#include "data.hpp"
#include "httplib.h"



extern cloud::DataManager* _data;

namespace cloud
{
    class Server
    {
    private:
        int _server_port;
        std::string _server_ip;
        std::string _download_suffix;
        httplib::Server _server;
    private:
        static void Upload(const httplib::Request& req, httplib::Response& res)
        {
            // 检查请求中是否包含名为 "file" 的文件
            auto ret = req.has_file("file");
            if (ret == false)  // 注意：这里应该使用 == 而不是 =，否则会有逻辑错误
            {
                res.status = 400; // 设置响应状态码为 400（Bad Request）
                return; // 结束函数
            }
            // 获取上传的文件对象
            const auto& file = req.get_file_value("file");
            // 从配置中获取备份目录路径
            std::string back_dir = Config::GetInstance()->GetBackDir();
            // 生成文件的实际路径
            std::string realpath = back_dir + FileUtil(file.filename).FileName();
            // 创建 FileUtil 对象并设置文件内容
            FileUtil Fu(realpath);
            Fu.SetContent(file.content);
            // 创建 BackupInfo 对象并更新备份信息
            BackupInfo info;
            info.NewBackupInfo(realpath);
            // 向数据存储中插入备份信息
            _data->Insert(info);
            return; // 结束函数
        }
        static std::string TimetoStr(time_t time)
        {
            std::string tmp = std::ctime(&time);
            return tmp;
        }
        static void ListShow(const httplib::Request& req, httplib::Response& res)
        {
            //获取所有文件的备份信息
            std::vector<BackupInfo> array;
            _data->GetAll(&array);
            //根据备份信息组织html文件数据
            std::stringstream ss;
            ss << "<html><head><title>Download</title></head>";
            ss << "<body><h1>Download</h1><table>";
            for (auto& e : array)
            {
                ss << "<tr>";
                std::string filename = FileUtil(e.realpath).FileName();
                ss << "<td><a href='" << e.url << "'>" << filename << "</a></td>";
                ss << "<td align='right'>" << TimetoStr(e.mtime) << "</td>";
                ss << "<td align='right'>" << e.fsize / 1024 << "k</td>";
                ss << "<tr>";
            }
            ss << "</table></body></html>";
            res.body = ss.str();
            res.set_header("Content-Type", "text/html");
            res.status = 200;
            return;
        }

        static std::string GetETag(const BackupInfo &info)
        {
            FileUtil Fu(info.realpath);
            std::string etag = Fu.FileName();
            etag += "-";
            etag += std::to_string(info.fsize);
            etag += "-";
            etag += std::to_string(info.mtime);
            return etag;
        }
        static void Download(const httplib::Request& req, httplib::Response& res)
        {
            //获取客户端请求的资源路径，根据资源路径获取文件备份信息
            BackupInfo info;
            _data->GetOneByURL(req.path, &info);
            //判断文件是否被压缩，若压缩了就解压缩至备份目录下，并删除压缩包，修改备份信息
            if (info.pack_flag == true)
            {
                FileUtil Fu(info.packpath);
                Fu.UnCompress(info.realpath);
                Fu.Remove();
                info.pack_flag = false;
                _data->Update(info);
            }
            FileUtil Fu(info.realpath);
            //判断请求头部是否包含 If_Range ，如果有并且该字段的值与请求文件的最新etag一致则是断点续传
            bool retrans_flag = false;
            std::string old_etag;
            if (req.has_header("If_Range"))
            {
                old_etag = req.get_header_value("If_Range");
                if (old_etag == GetETag(info))
                {
                    retrans_flag = true;
                }
            }
            if (retrans_flag == false)
            {
                //读取文件数据至res.body中
                Fu.GetContent(&res.body);
                //设置响应头部字段：ETag, Accept-Ranges: bytes
                res.set_header("Accept-Ranges", "bytes");
                res.set_header("ETag", GetETag(info));
                res.set_header("Content-Type", "application/octet-stream");
                res.status = 200;
            }
            else
            {
                //注意：在httplib库中已经给出了会根据body中的Ranges自行完成断点续传
                //只需我们将文件数据给到res.body中，其内部会自动获取到续传的指定区间，并完成响应
                Fu.GetContent(&res.body);
                //设置响应头部字段：ETag, Accept-Ranges: bytes
                res.set_header("Accept-Ranges", "bytes");
                res.set_header("ETag", GetETag(info));
                res.set_header("Content-Type", "application/octet-stream");
                res.status = 206;
            }
        }
    public:
        Server()
        {
            Config* config = Config::GetInstance();
            _server_port = config->GetServerPort();
            _server_ip = config->GetServerIp();
            _download_suffix = config->GetDowloadPrefix();
        }
        bool RunModule()
        {
            _server.Post("/upload", Upload);
            _server.Get("/listshow", ListShow);
            _server.Get("/", ListShow);
            _server.Get("/download/(.*)", Download);
            _server.listen(_server_ip.c_str(), _server_port);
            return true;
        }
    };
}




#endif 