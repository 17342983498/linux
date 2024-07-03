#ifndef __MY_HOT__
#define __MY_HOT__
#include <unistd.h>
#include "data.hpp"
#include "config.hpp"

extern cloud::DataManager* _data;

namespace cloud
{
    class HotManager
    {
    private:
        std::string _back_dir;
        std::string _pack_dir;
        std::string _pack_suffix;
        int _hot_time;
    private:
        bool HotJudge(const std::string& filename)
        {
            FileUtil Fu(filename);
            time_t last_time = Fu.LastATime();
            time_t cur_time = time(NULL);
            if (cur_time - last_time > _hot_time)
            {
                return true;
            }
            return false;
        }
    public:
        HotManager()
        {
            Config* config = Config::GetInstance();
            _back_dir = config->GetBackDir();
            _pack_dir = config->GetPackDir();
            _pack_suffix = config->GetPackfileSuffix();
            _hot_time = config->GetHotTime();
            FileUtil tmp1(_back_dir);
            tmp1.CreateDirectory();
            FileUtil tmp2(_pack_dir);
            tmp2.CreateDirectory();
        }
        bool RunModule()
        {
            FileUtil Fu(_back_dir);
            std::vector<std::string> array;
            while (1)
            {
                //1. 遍历备份目录，获取备份文件夹下所有文件的真实路径
                array.clear();
                Fu.ScanDirectory(&array);

                //2. 遍历备份信息管理文件判断是否为非热点文件
                for (auto& e : array)
                {
                    //通过第一步获得的真实路径获取该文件的备份信息(从_table中)到info中，如果没有则添加
                    BackupInfo info;
                    bool isNewInfo = false;
                    if (!_data->GetOneByRealPath(e, &info))
                    {
                        //创建没有该文件备份信息的info,这时候并不着急将info存入_table中，因为有可能需要压缩，这样的操作就多此一举
                        info.NewBackupInfo(e);
                        isNewInfo = true;
                    }
                    if (HotJudge(e) == true)
                    {
                        //3. 对非热点文件进行压缩处理
                        FileUtil tmp(e);
                        tmp.Compress(info.packpath);
                        //4. 删除源文件，修改备份信息
                        tmp.Remove();
                        info.pack_flag = true;
                    }
                    // 更新备份信息
                    if (isNewInfo || info.pack_flag)
                    {
                        _data->Update(info);
                    }
                }
                usleep(1000);
            }       
        }
    };
};


#endif
