#ifndef __MY_DATA__
#define __MY_DATA__

#include <unordered_map>
#include <pthread.h>
#include "util.hpp"
#include "config.hpp"

namespace cloud
{
    typedef struct BackupInfo
    {
        bool pack_flag;
        time_t mtime;
        time_t atime;
        size_t fsize;
        std::string realpath;
        std::string url;
        std::string packpath;
        bool NewBackupInfo(const std::string &realpath)
        {
            FileUtil Fu(realpath);
            if (Fu.Exists() == false)
            {
                std::cout << "new backupinfo failed!" << std::endl;
                return false;
            }
            Config *config = Config::GetInstance();
            std::string packdir = config->GetPackDir();
            std::string packfilesuffix = config->GetPackfileSuffix();
            std::string downloadprefix = config->GetDowloadPrefix();
            this->pack_flag = false;
            this->mtime = Fu.LastMTime();
            this->atime = Fu.LastATime();
            this->realpath = realpath;
            this->packpath = packdir + Fu.FileName() + packfilesuffix;
            this->url = downloadprefix + Fu.FileName();
            this->fsize = Fu.FileSize();
            return true;
        }
    } BackupInfo;

    class DataManager
    {
    private:
        std::string _backup_file; // 存储备份文件相关信息
        pthread_rwlock_t _rwlock; // 读写锁，读共享，写互斥
        std::unordered_map<std::string, BackupInfo> _table;

    public:
        DataManager()
        {
            _backup_file = Config::GetInstance()->GetBackupFile(); // 获取到cloud.conf文件
            pthread_rwlock_init(&_rwlock, NULL);
            InitLoad();
        }

        // 在Config对象中有一个备份信息管理文件，里边以字符串的形式存储BackupInfo对象
        bool InitLoad()
        {
            // 1. 将数据文件中的数据读取出来
            FileUtil Fu(_backup_file);
            if (Fu.Exists() == false)
            {
                return true;
            }
            std::string body;
            Fu.GetContent(&body);
            // 2. 反序列化
            Json::Value root;
            JsonUtil::UnSerialize(body, &root);
            // 3. 将反序列化得到的Json::Value数据添加到_table中
            for (auto e : root)
            {
                BackupInfo info;
                info.pack_flag = e["pack_flag"].asBool();
                info.fsize = e["fsize"].asInt64();
                info.atime = e["atime"].asInt64();
                info.mtime = e["mtime"].asInt64();
                info.packpath = e["packpath"].asString();
                info.realpath = e["realpath"].asString();
                info.url = e["url"].asString();
                _table[info.url] = info;
                // Insert(info);
            }
            return true;
        }

        // 将info结构体对象插入_table中，再持久化到
        bool Insert(const BackupInfo &info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info.url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }

        bool Update(const BackupInfo &info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            _table[info.url] = info;
            pthread_rwlock_unlock(&_rwlock);
            Storage();
            return true;
        }

        // 每次有信息改变则需要持久化存储一次
        // 所有信息发生了改变都是先存储在了_table中
        bool Storage()
        {
            // 1. 获取数据
            std::vector<BackupInfo> array;
            GetAll(&array); // 从_table中获取每一个BackupInfo对象至array中
            // 2. 添加到Json::Value中
            Json::Value root;
            for (auto e : array)
            {
                Json::Value item;
                item["pack_flag"] = e.pack_flag;
                item["atime"] = (Json::Value::Int64)e.atime; // 类型强转
                item["mtime"] = (Json::Int64)e.mtime;
                item["fsize"] = (Json::Int64)e.fsize;
                item["packpath"] = e.packpath;
                item["realpath"] = e.realpath;
                item["url"] = e.url;
                root.append(item);
            }
            // 3. 对Json::Value序列化
            std::string body;
            JsonUtil::Serialize(root, &body);
            // 4. 写文件
            FileUtil Fu(_backup_file);
            Fu.SetContent(body);
        }

        // 以url为key找到其对应的info对象
        bool GetOneByURL(const std::string &url, BackupInfo *info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.find(url);
            if (it == _table.end())
            {
                return false;
            }
            *info = it->second;
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }

        bool GetOneByRealPath(const std::string &realpath, BackupInfo *info)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.begin();
            while (it != _table.end())
            {
                if ((it->second.realpath) == realpath)
                {
                    *info = it->second;
                    pthread_rwlock_unlock(&_rwlock);
                    return true;
                }
                ++it;
            }
            pthread_rwlock_unlock(&_rwlock);
            return false;
        }

        // 将_table中所有数据添加到array中
        bool GetAll(std::vector<BackupInfo> *array)
        {
            pthread_rwlock_wrlock(&_rwlock);
            auto it = _table.begin();
            while (it != _table.end())
            {
                array->push_back(it->second);
                ++it;
            }
            pthread_rwlock_unlock(&_rwlock);
            return true;
        }
        ~DataManager()
        {
            pthread_rwlock_destroy(&_rwlock);
        }
    };

};

#endif
