#ifndef __MY_UTIL__
#define __MY_UTIL__


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <ctime>
#include <jsoncpp/json/json.h>
#include <experimental/filesystem>
#include "bundle.h"

namespace cloud
{
    namespace fs = std::experimental::filesystem;
    class FileUtil
    {
    private:
        std::string _filename;
    public:
        FileUtil(const std::string& filename) :_filename(filename)  
        {}

        int64_t FileSize()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Open file failed!" << std::endl;
                return -1;
            }
            return st.st_size;
        }

        time_t LastMTime()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Open file failed!" << std::endl;
                return -1;
            }
            return (st.st_mtim).tv_sec;
        }

        time_t LastATime()
        {
            struct stat st;
            if (stat(_filename.c_str(), &st) < 0)
            {
                std::cout << "Open file failed!" << std::endl;
                return -1;
            }
            return (st.st_atim).tv_sec;
        }

        std::string FileName()
        {
            size_t pos = _filename.find_last_of("/");
            if (pos == std::string::npos)
            {
                return _filename;
            }
            return _filename.substr(pos + 1);
        }
        //将文件内容给到body中
        bool GetPosLen(std::string *body, size_t pos, size_t len)
        {
            //先判断获取正文是否越界
            size_t fsize = this->FileSize();
            if (pos + len > fsize)
            {
                std::cout << "get file len is error" << std::endl;
                return false;
            }
            //打开文件
            std::ifstream ifs;
            ifs.open(_filename, std::ios::binary);
            if (ifs.is_open() == false)
            {
                std::cout << "read open file failed!" << std::endl;
                return false;
            }
            //将pos位置往后的内容读到body中
            ifs.seekg(pos, std::ios::beg);
            body->resize(len);
            ifs.read(&(*body)[0], len);
            //判断是否读成功
            if (ifs.good() == false)
            {
                std::cout << "get file content failed" << std::endl;
                ifs.close();
                return false;
            }
            ifs.close();
            return true;
        }

        bool GetContent(std::string* body)
        {
            size_t fsize = this->FileSize();
            return GetPosLen(body, 0, fsize);
        }
        //将body中的内容写进文件中
        bool SetContent(const std::string& body)
        {
            std::ofstream ofs;
            ofs.open(_filename, std::ios::binary);
            if (ofs.is_open() == false)
            {
                std::cout << "write open file failed!" << std::endl;
                return false;
            }
            ofs.write(&body[0], body.size());
            if (ofs.good() == false)
            {
                std::cout << "get file content failed" << std::endl;
                ofs.close();
                return false;
            }
            ofs.close();
            return true;
        }

        bool Compress(const std::string& packname)
        {
            std::string body;
            if (this->GetContent(&body) == false)
            {
                std::cout << "compress get file content failed!" << std::endl;
                return false;
            }

            std::string packedstring = bundle::pack(bundle::LZIP, body);

            FileUtil Fu(packname);
            if (Fu.SetContent(packedstring) == false)
            {
                std::cout << "compress write packed data failed!" << std::endl;
                return false;
            }
            return true;
        }

        bool UnCompress(const std::string& filename)
        {
            std::string packedstring;
            if (this->GetContent(&packedstring) == false)
            {
                std::cout << "uncompress get file content failed!" << std::endl;
                return false;
            }

            std::string body = bundle::unpack(packedstring);

            FileUtil Fu(filename);
            if (Fu.SetContent(body) == false)
            {
                std::cout << "uncompress write packed data failed!" << std::endl;
                return false;
            }
            return true;
        }

        bool Exists()
        {
            return fs::exists(_filename);
        }

        bool Remove()
        {
            if (this->Exists() == true)
            {
                remove(_filename.c_str());
            }
            return true;
        }

        bool CreateDirectory()
        {
            if (this->Exists()) return true;
            return fs::create_directory(_filename);
        }

        bool ScanDirectory(std::vector<std::string>* array)
        {
            for (auto& p : fs::directory_iterator(_filename))
            {
                if (fs::is_directory(p) == true)
                    continue;
                array->push_back(fs::path(p).relative_path().string());
            }
            return true;
        }

    };
    class JsonUtil
    {
    public:
        static bool Serialize(const Json::Value& root, std::string* str)
        {
            Json::StreamWriterBuilder swb;
            std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
            std::ostringstream os;
            if (sw->write(root, &os) != 0)
            {
                std::cout << "json write failed!" << std::endl;
                return false;
            }
            *str = os.str();
            return true;
        }

        static bool UnSerialize(const std::string& str, Json::Value* root)
        {
            Json::CharReaderBuilder crb;
            std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
            std::string err;//错误信息
            bool ret = cr->parse(str.c_str(), str.c_str() + str.size(), root, &err);
            if (ret == false)
            {
                std::cout << "parase error:" << err << std::endl;
                return false;
            }
            return true;
        }
    };
};

#endif
