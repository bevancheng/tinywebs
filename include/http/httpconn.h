#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <sys/types.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>

#include "../logger/log.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"

class HttpConn
{
public:
    HttpConn();
    ~HttpConn();

    void init(int sockFd, const struct sockaddr_in &addr);
    ssize_t read(int *saveErrno);
    ssize_t write(int *saveErrno);

    void Close();
    int GetFd() const;   //获取连接的文件描述符
    int GetPort() const; //获取连接的端口号

    const char *GetIP() const;   //获取连接的IP地址
    sockaddr_in GetAddr() const; //获取连接的地址信息

    bool process(); //处理请求

    int ToWriteBytes()
    {
        return iov_[0].iov_len + iov_[1].iov_len;
    }

    // IsKeepAlive()是否需要保持连接
    bool IsKeepAlive() const
    {
        return request_.IsKeepAlive();
    }

    static bool isET;                  //是否开启ET模式
    static const char *srcDir;         //源文件目录
    static std::atomic<int> userCount; //用户数量

private:
    int fd_;
    struct sockaddr_in addr_;

    bool isClose_;
    int iovCnt_;          // iov数组的个数
    struct iovec iov_[2]; // iov数组

    Buffer readBuff_;  //读缓冲区
    Buffer writeBuff_; //写缓冲区

    HttpRequest request_;   //请求信息
    HttpResponse response_; //响应信息
};
#endif // HTTP_CONN_H