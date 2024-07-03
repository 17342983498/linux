#include "util.hpp"
#include "config.hpp"
#include "data.hpp"
#include "hot.hpp"
#include "server.hpp"
#include <pthread.h>


void FileUtilTest(const std::string &filename)
{
    /*    cloud::FileUtil FU(filename);
    std::cout << FU.FileSize() << std::endl;
    std::cout << FU.LastMTime() << std::endl;
    std::cout << FU.LastATime() << std::endl;
    std::cout << FU.FileName() << std::endl;

    std::string body;
    FU.GetContent(&body);
    cloud::FileUtil NFU("./test.txt");
    NFU.SetContent(body);*/

    /*std::string packname = filename + ".lz";
    cloud::FileUtil fu(filename);
    fu.Compress(packname);
    cloud::FileUtil pfu(packname);
    pfu.UnCompress("./test.txt");*/

    cloud::FileUtil FU(filename);
    FU.CreateDirectory();
    std::vector<std::string> array;
    FU.ScanDirectory(&array);
    for (auto f : array)
    {
        std::cout << f << std::endl;
    }
}

void JsonUtilTest()
{
    const char* name = "张三";
    int age = 18;
    int score[] = { 60, 60, 59 };
    Json::Value root;
    root["姓名"] = name;
    root["年龄"] = age;
    root["成绩"].append(score[0]);
    root["成绩"].append(score[1]);
    root["成绩"].append(score[2]);

    std::string json_str;
    cloud::JsonUtil::Serialize(root, &json_str);
    std::cout << json_str << std::endl;

    Json::Value val;
    cloud::JsonUtil::UnSerialize(json_str, &val);
    std::cout << val["姓名"].asString() << std::endl;
    std::cout << val["年龄"].asInt() << std::endl;
    int sz = root["成绩"].size();
    for (int i = 0; i < sz; i++)
    {
        std::cout << root["成绩"][i].asFloat() << std::endl;
    }
}
void ConfigTest()
{
    cloud::Config* config = cloud::Config::GetInstance();
    std::cout << config->GetHotTime() << std::endl;
    std::cout << config->GetServerPort() << std::endl;
    std::cout << config->GetServerIp() << std::endl;
    std::cout << config->GetDowloadPrefix() << std::endl;
    std::cout << config->GetPackfileSuffix() << std::endl;
    std::cout << config->GetPackDir() << std::endl;
    std::cout << config->GetBackDir() << std::endl;
    std::cout << config->GetBackupFile() << std::endl;
}

void DataTest(const std::string &filename)
{

    cloud::DataManager data;
    std::vector<cloud::BackupInfo> array;
    data.GetAll(&array);
    for (auto& tmp : array)
    {
        std::cout << tmp.pack_flag << std::endl;
        std::cout << tmp.mtime << std::endl;
        std::cout << tmp.atime << std::endl;
        std::cout << tmp.realpath << std::endl;
        std::cout << tmp.packpath << std::endl;
        std::cout << tmp.url << std::endl;
        std::cout << tmp.fsize << std::endl;
    }

    // cloud::BackupInfo info;
    // info.NewBackupInfo(filename);
    // std::cout << info.pack_flag << std::endl;
    // std::cout << info.mtime << std::endl;
    // std::cout << info.atime << std::endl;
    // std::cout << info.realpath << std::endl;
    // std::cout << info.packpath << std::endl;
    // std::cout << info.url << std::endl;
    // std::cout << info.fsize << std::endl;


    // std::cout << "----------insert-----------" << std::endl;
    // cloud::DataManager data;
    // data.Insert(info);
    // cloud::BackupInfo tmp;
    // data.GetOneByURL(info.url, &tmp);
    // std::cout << tmp.pack_flag << std::endl;
    // std::cout << tmp.mtime << std::endl;
    // std::cout << tmp.atime << std::endl;
    // std::cout << tmp.realpath << std::endl;
    // std::cout << tmp.packpath << std::endl;
    // std::cout << tmp.url << std::endl;
    // std::cout << tmp.fsize << std::endl;

    // std::cout << "----------update all-----------" << std::endl;
    // info.pack_flag = true;
    // data.Update(info);
    // std::vector<cloud::BackupInfo> array;
    // data.GetAll(&array);
    // for (auto& tmp : array)
    // {
    //     std::cout << tmp.pack_flag << std::endl;
    //     std::cout << tmp.mtime << std::endl;
    //     std::cout << tmp.atime << std::endl;
    //     std::cout << tmp.realpath << std::endl;
    //     std::cout << tmp.packpath << std::endl;
    //     std::cout << tmp.url << std::endl;
    //     std::cout << tmp.fsize << std::endl;
    // }
    // std::cout << "----------realpath-----------" << std::endl;
    // data.GetOneByRealPath(info.realpath, &tmp);
    // std::cout << tmp.pack_flag << std::endl;
    // std::cout << tmp.mtime << std::endl;
    // std::cout << tmp.atime << std::endl;
    // std::cout << tmp.realpath << std::endl;
    // std::cout << tmp.packpath << std::endl;
    // std::cout << tmp.url << std::endl;
    // std::cout << tmp.fsize << std::endl;
}

cloud::DataManager* _data;
void HotTest()
{
    _data = new cloud::DataManager();
    cloud::HotManager hot;
    hot.RunModule();
}

void ServerTest()
{
    _data = new cloud::DataManager();
    cloud::Server svr;
    svr.RunModule();
}

int main(int argc, char* argv[])
{
    //FileUtilTest(argv[1]);
    //JsonUtilTest();
    //ConfigTest();
    //DataTest(argv[1]);
    //HotTest();
    //ServerTest();
    std::thread thread_hot_manager(HotTest);
    std::thread thread_server(ServerTest);
    thread_hot_manager.join();
    thread_server.join();
    return 0;
}