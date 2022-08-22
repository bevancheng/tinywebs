#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <unordered_map>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "../buffer/buffer.h"
#include "../logger/log.h"

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();

    void Init(const std::string &srcDir, std::string &path, bool isKeepAlive = false, int code = -1);
    void MakeResponse(Buffer &buff);
    void UnmapFile();       //释放文件映射
    char *File();           //获取文件映射
    size_t FileLen() const; //获取文件长度
    void ErrorContent(Buffer &buff, std::string message);
    int Code() const { return code_; }

private:
    void AddStateLine_(Buffer &buff); //添加状态行
    void AddHeader_(Buffer &buff);   //添加头部信息
    void AddContent_(Buffer &buff);   //添加内容信息

    void ErrorHtml_(); //错误页面
    std::string GetFileType_();

    int code_;
    bool isKeepAlive_;

    std::string path_;
    std::string srcDir_; //源文件目录

    char *mmFile_;           //文件映射
    struct stat mmFileStat_; //文件信息

    static const std::unordered_map<std::string, std::string> SUFFIX_TYPE;
    static const std::unordered_map<int, std::string> CODE_STATUS;
    static const std::unordered_map<int, std::string> CODE_PATH;
};

#endif