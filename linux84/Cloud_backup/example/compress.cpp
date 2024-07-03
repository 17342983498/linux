#include <iostream>
#include <string>
#include <fstream>
#include "bundle.h"

int main(int argc, char* argv[])
{
    std::string ifilename = argv[1];
    std::string ofilename = argv[2];

    std::ifstream ifs;
    ifs.open(ifilename, std::ios::binary);
    ifs.seekg(0, std::ios::end);
    size_t fsize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    std::string body;
    body.resize(fsize);
    ifs.read(&body[0], fsize);

    std::string packed = bundle::pack(bundle::LZIP, body);
    std::ofstream ofs;
    ofs.open(ofilename, std::ios::binary);
    ofs.write(&packed[0], packed.size());
    ifs.close();
    ofs.close();
}
