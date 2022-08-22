#ifndef BUFFER_H
#define BUFFER_H
#include <cstring> //perror
#include <iostream>
#include <unistd.h>  // write
#include <sys/uio.h> //readv
#include <vector>    //readv
#include <atomic>
#include <assert.h>
class Buffer
{
public:
    Buffer(int initBuffSize = 1024);
    ~Buffer() = default;

    size_t WritableBytes() const;    //可写的字节数
    size_t ReadableBytes() const;    //可读的字节数
    size_t PrependableBytes() const; //可预写的字节数

    const char *Peek() const;         //指向可读的字节的首地址
    void EnsureWriteable(size_t len); //确保可写的字节数足够len
    void HasWritten(size_t len);      //已经写入len字节

    void Retrieve(size_t len);           //抛弃len字节
    void RetrieveUntil(const char *end); //抛弃到end

    void RetrieveAll();             //抛弃所有字节
    std::string RetrieveAllToStr(); ////抛弃所有字节并返回字符串

    const char *BeginWriteConst() const;
    char *BeginWrite();

    void Append(const std::string &str);
    void Append(const char *str, size_t len);
    void Append(const void *data, size_t len);
    void Append(const Buffer &buff);

    ssize_t ReadFd(int fd, int *Errno);  //从fd中读取数据到缓冲区
    ssize_t WriteFd(int fd, int *Errno); //向fd中写入数据

private:
    char *BeginPtr_();             //指向缓冲区的首地址
    const char *BeginPtr_() const; //指向缓冲区的首地址
    void MakeSpace_(size_t len);   //确保可写的字节数足够len

    std::vector<char> buffer_;          //缓冲区
    std::atomic<std::size_t> readPos_;  //读取位置
    std::atomic<std::size_t> writePos_; //写入位置
};

#endif // BUFFER_H