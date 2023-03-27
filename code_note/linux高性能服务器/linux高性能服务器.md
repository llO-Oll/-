## socket地址API

### 主机字节序 和 网络字节序

- 小端字节序：高位存储在内存高地址处，低位存储在内存低地址处。

大部分PC采用小端字节序，又称主机字节序。

大端字节序又称网络字节序。

### IP地址转换函数

```c
#include <arpa/inet.h>
/*
把 用字符串表示IP地址的src 转换成 用网络字节序整数表示的IP地址，
并把转换结果存储在dst指向的地址
Args:
    af: 指定的地址族
    src: 字符串表示的IP地址
    dst: 转换后的存储地址
Return:
    成功返回1，失败返回0
*/
int inet_pton(int af, const char* src,void* dst) ;

/*
inet_ntop函数进行相反的转换。前三个参数与 inet_pton 相同，最后一个参数cnt
指定目标存储单元的大小。
*/
const char* inet_ntop(int af, const void* src, char* dst,
socklen_t cnt);

/*
以下两个宏 用于指定 cnt 大小，分别用于IPv4和IPv6
*/
#include <netinet/in.h>
#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46
```

## 创建 socket

```c
# include <bits/socket.h>
struct sockaddr_storage
{
    sa_family_t sa_family;
    unsigned long int __ss_align;
    char __ss_padding[128-sizeof(__ss_align)];
};
/*
通用socket地址结构体
sa_family_t: 地址族类型,与协议族类型对应，
地址族就是一个协议族所使用的地址集合

常见的协议族:
协议族        地址族        描述                地址值含义和长度
PF_UNIX        AF_UNIX        UNIX本地域协议族    文件的路径名
PF_INET        AF_INET        TCP/IPv4协议族    16bit端口号和32bitIPv4地址
PF_INET6    AF_INET6    TCP/IPv6协议族    16bit端口号和32bit流标识，
                                        128bitIPv6地址，32bit范围ID
*/


/*TCP/IP协议族有 sockaddr_in 和 sockaddr_in6 两个专用socket地址结构体，
分别用于IPv4和IPv6*/
struct sockaddr_in
{
    sa_family_t sin_family;    // 地址簇: AF_INET
    u_int16_t sin_port;        // 端口号
    struct in_addr sin_addr;    // IPv4地址结构体
};
struct in_addr
{
    u_int32_t s_addr;    //IPv4地址
};
struct sockaddr_in6
{
    sa_family_t sin6_family;    // 地址族: AF_INET6
    u_int16_t sin6_port;        //
    u_int32_t sin6_flowinfo;    //
    struct in6_addr sin6_addr;    //
    u_int32_t sin_scope_id;    //
}



# include <sys/types.h>
# include <sys/socket.h>
int socket(int domain, int type, int protocol);
/*
Function:
    创建一个socket
Args:
    domain: 底层协议,对TCP/IP协议簇，应选择PF_INET(Protocol Family of Internet,用于IPv4)
    type: 服务类型, SOCK_STREAM(流服务)/SOCK_UGRAM(数据报).对TCP/IP,SOCK_STEAM表示传输层使用TCP,SOCK_DGRAM表示使用UDP协议
    protocol: 前两个参数构成的协议集下再选择一个具体的协议.默认为0
Return:
    成功返回一个socket文件描述符，失败返回-1
*/


int bind(int sockfd, const struct sockaddr* my_addr, socklen_t addrlen);
/*
Function:
    将 socket 与 socket地址 绑定
Args:
    sockfd: 文件描述符
    addrlen:sorcket地址长度
*/
```

## 监听socket

```c
# include <sys/socket.h>
int listen(int sockfd, int backlog);
/*
Args:
    sockfd: 被监听的socket
    backlog: 内核监听队列的最大长度
*/
```

`testlisten.c`接收三个参数：IP地址、端口号和backlog值。

`gcc testlisten.c -o testlisten`生成可执行程序`testlisten`

测试本地ip的两个端口通信

`./testlisten 本地ip 12345 5`

`telnet 本地ip 12345 # 建立连接`

`netstat -nt | grep 12345 # 查看连接状态`

在终端输入

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool stop = false;
/* SIGTERM信号的处理函数，触发时结束主程序中的循环*/
static void handle_term( int sig )
{
    stop = true;
}

int main(int argc, char* argv[])
{
    signal(SIGTERM, handle_term);
    if(argc <= 3)
    {
        printf("usage: %s ip_address port_number backlog\n",basename(argv[0]) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);    // atoi:把字符串转化为整形
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0 );

    /* 创建一个IPv4 socket 地址 */
    struct sockaddr_in address;
    bzero( &address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip , &address.sin_addr );
    address.sin_port = htons(port);

    int ret = bind(sock,(struct sockaddr* )&address , sizeof(address));
    assert(ret != -1 );

    ret = listen(sock, backlog);
    assert(ret != -1);

    /* 循环等待连接，直到有SIGTERM信号将它中断 */
    while(! stop){
    sleep(1);
    }

    /* 关闭socket */
    close(sock);
    return 0;
}
```

## 接受连接

```c
#include <sys/types.h>
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
/*
接受连接
Args:
    sockfd: 执行过listen系统调用的监听socket
    addr: 获取被接受连接的远端socket地址
    addrlen: addr的长度

*/
```

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc <= 2){
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    /* 创建一个IPv4 socket地址 */
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert( sock >= 0);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    /* 暂停20秒以等待客户端连接和相关操作（掉线或者退出）完成 */
    sleep(20);
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr* )&client, 
    &client_addrlength);
    if( connfd < 0){
        printf("errno is %d\n",errno);
    }else{
        /* 接受连接成功则打印出客户端的IP地址和端口号 */
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip: %s and port: %d\n",
        inet_ntop(AF_INET,&client.sin_addr,remote,INET_ADDRSTRLEN),
        ntohs(client.sin_port));
        close(connfd);
    }
    close(sock);
    return 0;
}
```

## 发起连接

```cpp
#include <sys/types.h>
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *serv_addr,
 socklen_t addrlen);
/*
Args:
    sockfd: 参数由 socket 系统调用返回一个 socket.
    serv_addr: 服务器监听的 socket地址.
    addrlen: 地址长度
Return:
    成功返回0，失败返回-1
*/
```

## 关闭连接

```c
#include <unistd.h>
int close(int fd);
/*
通过关闭普通文件描述符的系统，fd引用计数-1，为0才真正关闭连接
*/

#include <sys/socket.h>
int shutdown(int sockfd, int howto);
/*
Args:
    sockfd: 参数由 socket 系统调用返回一个 socket.
    howto: shutdown的行为:SHUT_RD|SHUT_WR|SHUT_RDWR.
Return:
    成功返回0，失败返回-1
*/
```

## 数据读写

### TCP数据读写

```c
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
ssize_t send(int sockfd, const void *buf, size_t len, int flags);
```

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
int main(int argc, char* argv[]){
    if(argc <= 2){
        printf("usage: %s ip_address port_number\n",
        basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    bzero(&server_address,sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons( port );

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    if(connect(sockfd,( struct sockaddr* )&server_address,
                       sizeof(server_address) )<0)
    {
        printf("connection failed %d\n",errno);
    }
    else
    {
        const char* oob_data = "abc";
        const char* normal_data = "123";
        send( sockfd, normal_data, strlen( normal_data ),0 );
        send( sockfd, oob_data, strlen( oob_data), MSG_OOB);
        send( sockfd, normal_data, strlen( normal_data ),0 );
    }
    close(sockfd);
    return 0;
}  }
    close(sockfd);
    return 0;
}
```

接收数据

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE 1024
int main(int argc ,char* argv[]){
    if(argc <= 2){
        printf("usage: %s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >=0 );

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(sock,5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock,( struct sockaddr*)&client,&client_addrlength);
    if(connfd < 0){
        printf("errno is: %d\n",errno);
    }
    else{
        char buffer[BUF_SIZE];

        memset(buffer,'\0',BUF_SIZE);
        ret = recv(connfd,buffer,BUF_SIZE-1,0);
        printf("got %d bytes of normal data '%s'\n",ret,buffer);

        memset(buffer,'\0',BUF_SIZE);
        ret = recv(connfd,buffer,BUF_SIZE-1,MSG_OOB);
        printf("got %d bytes of oobdata '%s'\n",ret,buffer);

        memset(buffer,'\0',BUF_SIZE);
        ret = recv(connfd,buffer,BUF_SIZE-1,0);
        printf("got %d bytes of normal '%s'\n",ret,buffer);  

        close(connfd);   
    }
    close(sock);
    return 0;
}
```

`testoobsend.c`和`testoobrecv.c`接收两个参数：IP地址、端口号。

`gcc testoobsend.c -o testoobsend`生成可执行程序`testoobsend`

测试本地ip的两个端口通信

`./testoobrecv 本地ip 12345`

`./testoobsend 本地ip 12345`

![](assets/2023-03-15-21-56-07-image.png)

### UDP数据读写

```c
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags,
                 struct sockaddr* src_addr, socklen_t* addrlen);
ssize_t sendto(int sockfd, const void* buf, size_t len, int flags, 
                const struct sockaddr* dest_addr, socklen_t* addrlen);
```

## 地址信息函数

```c
#include <sys/socket.h>
/*
    获取sockfd对应的本端socket地址
*/
int getsockname(int sockfd, struct sockaddr* address,
 socklen_t* address_len);


/*
    获取sockfd对应的远端socket地址
*/
int getpeername(int sockfd, struct sockaddr* address,
 socklen_t* address_len);
```

# 高级I/O函数

## pipe函数

```cpp
#include <unistd.h>
/*
传入一对文件描述，只能数据只能f[0]->f[1]
*/
int pipe(int fd[2]);
```

## dup函数和dup2函数

```cpp
#include <unistd.h>
/*
    创建一个新的文件描述符，新的文件描述符和原本文件描述符file_descriptor指向
相同的文件，网络连接或管道。
    所以可以把标准输入重定向到一个文件，或者把标准输出重定向到一个网络连接。
Return:
    返回系统当前可用的最小文件描述符。
*/
int dup(int file_descriptor);

/*
    与dup类似
*/
int dup2(int file_descriptor_one, int file_descriptor_two);
```

利用dup函数实现一个基本的CGI服务器

CGI通用网关接口（Common Gateway Interface/CGI）是一种重要的互联网技术，可以让一个客户端，从网页浏览器向执行在网络服务器上的程序请求数据。CGI描述了服务器和请求处理程序之间传输数据的一种标准

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc ,char* argv[]){
    if(argc <= 2){
        printf("usage: %s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert( sock >= 0);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client,
 &client_addrlength );
    if( connfd < 0){
        printf("errno is : %d\n",errno);
    }else{
    /* CGI工作原理 */
        close(STDOUT_FILENO);//先关闭 标准输出 文件描述符STDOUT_FILENO(其值为1)
        dup(connfd); //返回值为1，标准输出 重定向到 网络连接
        printf("abcd\n");//原本标准输出的内容 输出到 与客户连接的socket上
        close(connfd;)
    }
    close(sock);
    return 0;
}
```

## readv函数和writev函数

```c
#include <sys/uio.h>

/*
readv把数据从文件描述符读到分散的内存块中
*/
ssize_t readv(int fd,const struct iovec* vector, int count);

/*
writev将多块分散的内存数据写入到文件描述符中
*/
ssize_t writev(int fd, const struct iovec* vector, int count);
```

## sendfile函数

```c
#include <sys/sendfile.h>
/*
sendfile在两个文件描述符之间直接传递数据，避免了数据拷贝，
零拷贝操作，效率很高。
*/
ssize_t sendfile(int out_fd, int in_fd, off_t* offset, size_t count);
```

## mmap函数和munmap函数

```c
#include <sys/mman.h>
/*
mmap用于申请一段内存空间。可以把这段内存作为进程通信的共享内存，也可以将文件
直接映射到其中。
Args:
    start:内存起始地址
    length:内存长度
    prot:内存段访问权限 
        PROT_READ 可读|PROT_WRITE 可写|PROT_EXEC 可执行|PROT_NONE 不能被访问 
    fd:被映射文件的文件描述符
    offset:从文件的何处开始映射
*/
void *mmap(void *start, size_t length, int prot, int flags, int fd,
    off_t offset);

/*
释放这段内存
*/
int munmap(void *start, size_t length);
```

## splice函数

```c
#include <fcntl.h>
/*
用于两个文件描述符之间移动数据，零拷贝操作
*/
ssize_t splice(int fd_in, loff_t* off_in, int fd_out,
loff_t* off_out, size_t len, unsigned int flags);
```

`fd_in`参数是待输人数据的文件描述符。如果`fd_in`是一个管道文件描述符，那么`off_in`参数必须被设置为NULL。如果`fd_in`不是一个管道文件描述符（比如 socket)，那么`off_in`表示从输入数据流的何处开始读取数据。此时，若`off_in`被设置为NULL，则表示从输入数据流的当前偏移位置读入;若`off_in`不为NULL，则它将指出具体的偏移位置。`fd_out/off_out`参数的含义与`fd_in/off_in`相同，不过用于输出数据流。`len`参数指定移动数据的长度;`flags`参数则控制数据如何移动，



使用`splice`函数时，`fd_in`和 `fd_out`必须至少有一个是管道文件描述符。`splice`函数调用成功时返回移动字节的数量。它可能返回0，表示没有数据需要移动，这发生在从管道中读取数据（`fd_in`是管道文件描述符）而该管道没有被写人任何数据时。`splice`函数失败时返回-1并设置`errno`。





利用`splice`函数实现一个零拷贝的回射服务器，它将客户端发送的数据原样返回客户端。

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc ,char* argv[]){
    if(argc <= 2){
        printf("usage: %s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert( sock >= 0);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock, (struct sockaddr*)&client,
 &client_addrlength );
    if( connfd < 0){
        printf("errno is : %d\n",errno);
    }else{
        int pipefd[2];
        assert(ret != -1);
        ret = pipe(pipefd);    //创建管道
        /* 将connfd上流入的客户数据定向到管道中 */
        ret = splice(connfd, NULL, pipefd[1], NULL,32768,
                    SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        /* 将管道的输出定向到connfd客户连接文件描述符 */
        ret = splice(pipefd[0], NULL, connfd, NULL,32768,
                    SPLICE_F_MORE | SPLICE_F_MOVE);
        assert(ret != -1);
        close(connfd);
    }
    close(sock);
    return 0;
}
```



# 线程池

## 实现简单的Web服务器

```c
#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include
```

