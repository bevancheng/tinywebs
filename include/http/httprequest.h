#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <regex>
#include <errno.h>
#include <mysql/mysql.h>

#include "../buffer/buffer.h"
#include "../logger/log.h"
#include "../pool/sqlconnRAII.h"
#include "../pool/sqlconnpool.h"

class HttpRequest
{
public:
    enum PARSE_STATE
    {
        REQUEST_LINE,
        HEADERS,
        BODY,
        FINISH,
    };

    enum HTTP_CODE
    {
        NO_REQUEST = 0,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION,
    };

    HttpRequest() { Init(); };
    ~HttpRequest() = default;

    void Init();              //初始化
    bool parse(Buffer &buff); //解析请求

    std::string path() const; //获取请求路径
    std::string &path();
    std::string method() const;                        //获取请求方法
    std::string version() const;                       //获取请求版本
    std::string GetPost(const std::string &key) const; //获取post请求参数
    std::string GetPost(const char *key) const;        //获取post请求参数

    bool IsKeepAlive() const; //是否需要保持连接

private:
    bool ParseRequestLine_(const std::string &line);
    void ParseHeader_(const std::string &line);
    void ParseBody_(const std::string &line);

    void ParsePath_();
    void ParsePost_();
    void ParseFromUrlencoded_(); //解析urlencoded格式的post请求参数

    static bool UserVerify(const std::string &name, const std::string &pwd, bool isLogin);

    PARSE_STATE state_; //解析状态
    std::string method_, path_, version_, body_;
    std::unordered_map<std::string, std::string> header_; //请求头信息
    std::unordered_map<std::string, std::string> post_;   // post请求参数

    static const std::unordered_set<std::string> DEFAULT_HTML;          //默认的html文件
    static const std::unordered_map<std::string, int> DEFAULT_HTML_TAG; //默认的html文件的标签
    static int ConverHex(char ch);                                      //将十六进制的字符转换为数字
};
#endif // HTTP_REQUEST_H