#ifndef __MY_CONFIG__
#define __MY_CONFIG__

#include <mutex>
#include "util.hpp"


namespace cloud
{
#define CONFIG_FILE "./cloud.conf"
    class Config
    {
    private:
        Config()
        {
            ReadConfigFile();
        }
        static Config* _instance;
        static std::mutex _mutex;
        int _hot_time;//热点时间
        int _server_port;//服务器端口
        std::string _server_ip;//服务器IP
        std::string _download_prefix;//下载路径前缀
        std::string _packfile_suffix;//压缩文件后缀
        std::string _pack_dir;//压缩路径
        std::string _back_dir;//备份路径
        std::string _backup_file;//备份信息配置管理

        //将设定好的信息配置到Config对象当中
        bool ReadConfigFile()
        {
            FileUtil Fu(CONFIG_FILE);
            std::string body;
            if (Fu.GetContent(&body) == false)
            {
                std::cout << "Get Content failed!" << std::endl;
                return false;
            }
            Json::Value root;
            if (JsonUtil::UnSerialize(body, &root) == false)
            {
                std::cout << "parse config file failed!" << std::endl;
                return false;
            }
            _hot_time = root["hot_time"].asInt();
            _server_port = root["server_port"].asInt();
            _server_ip = root["server_ip"].asString();
            _download_prefix = root["download_prefix"].asString();
            _packfile_suffix = root["packfile_suffix"].asString();
            _pack_dir = root["pack_dir"].asString();
            _back_dir = root["back_dir"].asString();
            _backup_file = root["backup_file"].asString();
            return true;
        }
    public:
        static Config* GetInstance()
        {
            if (_instance == NULL)
            {
                _mutex.lock();
                if (_instance == NULL)
                {
                    _instance = new Config();
                }
                _mutex.unlock();
            }
            return _instance;

        }
        int GetHotTime()
        {
            return _hot_time;
        }
        int GetServerPort()
        {
            return _server_port;
        }
        std::string GetServerIp()
        {
            return _server_ip;
        }
        std::string GetDowloadPrefix()
        {
            return _download_prefix;
        }
        std::string GetPackfileSuffix()
        {
            return _packfile_suffix;
        }
        std::string GetPackDir()
        {
            return _pack_dir;
        }
        std::string GetBackDir()
        {
            return _back_dir;
        }
        std::string GetBackupFile()
        {
            return _backup_file;
        }
    };
    Config* Config::_instance = NULL;
    std::mutex Config::_mutex;
};



#endif