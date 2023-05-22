# Linux网络编程基础api

## socket地址API

### 主机字节序 和 网络字节序

- 小端字节序：高位存储在内存高地址处，低位存储在内存低地址处。

大部分PC采用小端字节序，又称主机字节序。

大端字节序又称网络字节序。

linux提供下面4个函数完成主机字节序和网络字节序之间的转换：

```c
#include <netinet/in.h>
/*	host to network long	*/
unsigned long int htonl( unsigned long int hostlong );

unsigned short int htons( unsigned short int hostshort );

unsigned long int ntohl( unsigned long int netlong );

unsigned short int ntohs( unsigned short int netshort );
```



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



## 带外数据(out—of—band data)

OOB有时也称为加速数据(expedited data)，

是指连接双方中的一方发生重要事情，想要迅速地通知对方。

这种通知在已经排队等待发送的任何“普通”(有时称为“带内”)数据之前发送。

带外数据设计为比普通数据有更高的优先级。

带外数据是映射到现有的连接中的，而不是在客户机和服务器间再用一个连接。



### TCP数据读写

```c
#include <sys/types.h>
#include <sys/socket.h>
/*
@return		-1 失败
        	0	对方关闭了通信
        	>0	成功读取数据
*/
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

## socket选项

读取 和 设置 socket文件描述符属性

```c
#include <sys/socket.h>
int getsockopt(int sockfd, int level, int option_name, void* option_value, socklen_t* restrict option_len);
int setsockopt(int sockfd, int level, int option_name, const void* option_value, socklen_t option_len);
```

![image-20230409184113571](./assets/image-20230409184113571.png)

# 高级I/O函数

## pipe函数

```cpp
#include <unistd.h>
/*
传入一对文件描述。

往fd[1]写入的数据，可以从fd[0]读出。
且fd[0]只能从管道读数据，fd[1]只能用于往管道写入数据。
写入->fd[1]->fd[0]->读出
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

## 零拷贝

零拷贝是指计算机执行IO操作时，CPU不需要将数据从一个存储区域复制到另一个存储区域，从而可以减少上下文切换以及CPU的拷贝时间。它是一种`I/O`操作优化技术。



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

## fcntl函数

file control，对文件进行控制

```c
#include <fcntl.h>
int fcntl(int fd, int cmd, ……);

/* 将文件描述符设置为非阻塞 , 如果read没成功,就会 立即返回 并 报一个资源暂时不可用的错误.*/
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);	// 获取文件描述符 旧的状态标志
	int new_option = old_option | O_NONBLOCK;	// 设置非阻塞标志
	fcntl(fd, F_SETFL, new_option);
	return old_option;	// 返回 旧的状态标志，以便日后恢复该状态标志
}

```

# 高性能服务器程序框架

服务器三个主要模块

- I/O处理单元。
- 逻辑单元。
- 存储单元。

## 服务器模型

### 1.C/S模型

![image-20230424160840806](./assets/image-20230424160840806.png)

1. 服务器启动后，首先创建监听`socket()`,并调用`bind()`函数将其绑定到服务器感兴趣的端口上，然后调用`listen`函数等待客户连接。
2. 服务器稳定运行后，客户端就可以调用`connect`函数向服务器发起连接。
3. 由于客户连接请求是随机到达的异步事件，服务器需要I/O复用技术的`select`系统调用。同时select也用于监听多个客户请求。
4. 当监听到连接请求后，服务器就调用`accept`函数接受它，并分配一个逻辑单元为新的连接服务。逻辑单元可以是新创建的子进程、子线程或者其他。
5. 图8-2中，服务器给客户端分配的逻辑单元是由`fork()`创建的子进程。逻辑单元读取客户请求，处理该请求，然后将处理结果返回给客户端。

![image-20230424160909826](./assets/image-20230424160909826.png)

### 2.P2P模型

![image-20230424193924090](./assets/image-20230424193924090.png)

从编程角度来讲，P2P模型可以看作C/S模型的扩展:每台主机既是客户端，又是服务器。因此，我们仍然采用C/S模型来讨论网络编程。

## 两种高效的并发模式

### 半同步/半异步模式

![image-20230424194827513](./assets/image-20230424194827513.png)

### 领导者/追随者模式



# I/O复用

I/O复用 使得程序能够**监控多个文件表述符**。

**同时处理多个描述符IO事件的一种技术手段**

下列情况需要使用I/O复用技术：

1. 客户端同时处理多个socket。比如非阻塞的connect技术
2. 客户端同时处理用户输入和网络连接
3. TCP服务器要同时处理监听socket和连接socket。
4. ……



## select系统调用

轮询感兴趣的文件描述符

```c
#include <sys/select.h>
/*
args:
	nfds: 指定被监听的文件描述符总数。描述符从0开始，nfds通常是最大描述符+1
	readfds、writefds、exceptfds: 用户要监听的可读、可写、异常文件描述符
	timeout: 设置select函数超时时间
return:
	成功 返回 就绪的可读可写异常的文件描述符总数
	失败 返回 -1
*/
int select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds,
          struct timeval* timeout);

/*fd_set结构体仅包含一个整形数组，该数组的每个元素的每一位(bit)标记一个文件描述符。fd_set能容纳的文件描述符数量由FD_SETSIZE指定。这就限制了select的最大连接数。

```





`select`接受普通数据和带外数据

```c
#include <sys/types.h>
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

int main(int argc, char* argv[]){
	if( argc <= 2){
		printf("usage: %s ip_address port_number\n", basename(argv[0]));
		return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    /* 创建一个IPv4 socket地址 */
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert( listenfd >= 0 );
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    ret = listen(listenfd,5);
    assert(ret != -1);
   
    /* 接受socket连接 */
    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    int connfd = accept(listenfd, (struct sockaddr* )&client_address,
                       &client_addrlength);
    if( connfd < 0 ){
		printf("errno is: %d\n",errno);
    	close(listenfd);
    }
    char buf[1024];
    /* fd_set 一组文件描述字(fd)的集合，它用一位来表示一个fd */
    fd_set read_fds;
    fd_set exception_fds;
    /* FD_ZERO 将指定的文件描述符集清空，在对文件描述符集合进行设置前，必须对其进行初始化 */
    FD_ZERO(&read_fds);
    FD_ZERO(&exception_fds);
    
    while(1){
		memset( buf, '\0', sizeof( buf ));
        /*
        	每次调用select前都要重新在 read_fds 和 exception_fds 中设置文件描述符connfd，因为事件发生后，文件描述集合将被内核修改。
        	FD_SET 用于在文件描述符集合中增加一个新的文件描述符。
        */
        FD_SET( connfd, &read_fds); //将connfd套节字加入到集合read_fds
        FD_SET( connfd, &exception_fds); //将connfd套节字加入到集合exception_fds
        ret = select( connfd + 1, &read_fds, NULL, &exception_fds, NULL);
        if( ret < 0){
			printf("selection failure\n");
			break;
        }
        
        /*
        	对于可读事件,采用普通的 recv 函数读取数据
        	FD_ISSET(int fd,fd_set *fdset);用于测试指定的文件描述符是否在该集合中。
        */
        if( FD_ISSET(connfd, &read_fds) ){
			ret = recv(connfd, buf, sizeof(buf)-1, 0);
			if(ret <= 0){
				break;
			}
            printf("get %d bytes of normal data: %s\n", ret, buf);
        }
        /* 对于异常事件，采用带MSG_OOB标志的recv函数读取带外数据 */
        else if(FD_ISSET(connfd, &exception_fds)){
			ret = recv(connfd, buf, sizeof(buf)-1, MSG_OOB);
    		if( ret<=0 ){
				break;
            }
            printf("get %d bytes of oob data: %s\n", ret, buf);
        }
    }
    close(connfd);
    close(listenfd);
    return 0;
}

```

## poll

和`select`类似，在指定时间内轮询一定数量的文件描述符，以测试其中是否有就绪文件描述符。

```c
#include <poll.h>
/*

@param 	fds: 监听的事件集合
@param	nfds: fds的大小
@param	timeout: 单位毫秒
			-1	将永远阻塞直到某个事件发生
			0	立即返回
@return:
	和select一样
*/
int poll(struct pollfd* fds, nfds_t nfds, int timeout);


struct pollfd{
	int fd;	//文件描述符
    short events;	//注册的事件 poll需要监听的事件
    short revents;	//实际发生的事件，由内核填充
}
```

![image-20230409144321305](./assets/image-20230409144321305.png)

![image-20230409144332698](./assets/image-20230409144332698.png)



## epoll

epoll是linux特有的I/O复用函数。

不需要像`select`和`poll`那样重复传入文件描述符集。使用**一个额外的文件描述符** 标识 文件描述符集（或者叫事件集）。

```c
#include <sys/epoll.h>

int epoll_create(int size);

/*
操作epoll内核事件表
@param epfd:内核事件表
@param op:指定操作类型
		EPOLL_CTL_ADD	往事件表中注册fd上的事件
		EPOLL_CTL_MOD	修改fd上的注册事件
		EPOLL_CTL_DEL	删除fd上的注册事件
		
@param fd:要操作的文件描述符
@param event:
@return:
	success	0
	fail	-1
*/
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

/*
timeout时间内监听一组文件描述符上的事件。
如果检测到事件，就将所有就绪事件从 内核事件表epfd 中复制到 events 指向的数组中。
@param maxevents:最多监听事件个数
@return:
	成功	返回就绪的文件描述符个数
	失败	返回-1
*/
int epoll_wait(int epfd, struct epoll_event* events, int maxevents, int timeout);


```



```c
/* 如何索引poll返回的就绪文件描述符 */
int ret = poll(fds, MAX_EVENT_NUMBER, -1);
/* 必须遍历所有已注册文件描述符并找到其中的就绪者 */
for(int i=0;i<MAX_EVENT_NUMBER;++i){
	if(fds[i].revents & POLLIN){	/* 判断第i个文件描述符是否就绪 */
		int sockfd = fds[i].fd;
        /* 处理sockfd */
    }
}

/* 如何索引epoll返回的就绪文件描述符 */
int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
/* 仅遍历就绪的ret个文件描述符 */
for(int i=0;i<ret;i++){
	int sockfd = events[i].data.fd;
	/* sockfd肯定就绪，直接处理 */
}


```







### LT和ET模式

**水平触发(level-triggered)**

- socket接收缓冲区不为空 有数据可读 读事件一直触发
- socket发送缓冲区不满 可以继续写入数据 写事件一直触发

**边沿触发(edge-triggered)**

- socket的接收缓冲区状态变化时触发读事件，即空的接收缓冲区刚接收到数据时触发读事件
- socket的发送缓冲区状态变化时触发写事件，即满的缓冲区刚空出空间时触发读事件

边沿触发仅触发一次，水平触发会一直触发。

```
**事件宏**

EPOLLIN ： 表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT： 表示对应的文件描述符可以写；
EPOLLPRI： 表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR： 表示对应的文件描述符发生错误；
EPOLLHUP： 表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式（默认为水平触发），这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT： 只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
```



```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

int setnonblocking(int fd){
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
}
/* 将文件描述符fd上的EPOLLIN注册到epollfd指示的epoll内核事件表中,参数enable_et指定是否对fd启用ET模式 */
void addfd(int epollfd, int fd, bool enable_et){
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;		// EPOLLIN 表示可读事件
	if(enable_et){
		event.events |= EPOLLET;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}
/* LT模式的工作流程 */
void lt(epoll_event* events, int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	for(int i=0;i<number;i++){
		int sockfd = events[i].data.fd;
		if(sockfd == listenfd){
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength );
			addfd(epollfd, connfd, false);
		}
		else if(events[i].events & EPOLLIN)
		{
			/* 只要 socket 读缓存中还有未读出的数据,这段代码就被触发 */
			printf("event trigger once\n");
			memset(buf,'\0',BUFFER_SIZE);
			int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
			if(ret <= 0){
				close(sockfd);
				continue;
			}
			printf("get %d bytes of content: %s\n",ret,buf);
		}
		else
		{
			printf("something else happened \n");
		}
	}
}

/* ET模式的工作流程 */

void et(epoll_event* events,int number, int epollfd, int listenfd)
{
	char buf[BUFFER_SIZE];
	for(int i=0;i<number;i++){
		int sockfd = events[i].data.fd;
		if( sockfd == listenfd ){
			struct sockaddr_in client_address;
			socklen_t client_addrlength = sizeof(client_address);
			int connfd = accept(listenfd,(struct sockaddr*)&client_address, &client_addrlength );
			addfd(epollfd, connfd, true);	
		}
		else if(events[i].events & EPOLLIN)
		{
			/* 这段代码不会重复触发,所以我们循环读取数据，确保把socket读缓存中的所有数据读出 */
			printf("event trigger once\n");
			while(1)
			{
				memset(buf, '\0', BUFFER_SIZE);
				int ret = recv( sockfd, buf, BUFFER_SIZE-1, 0);
				if(ret < 0){
					/* 对于非阻塞IO，下面的条件成立表示数据已经全部读取完毕。此后，epoll就能再次触发sockfd上的EPOLLIN事件，以驱动下一次读操作 */
                	if((errno == EAGAIN) || (errno == EWOULDBLOCK)){
                        printf("read later\n");
                        break;
                    }
                	close(sockfd);
                	break;
				}		
				else if(ret==0){
					close(sockfd);
				}
				else
				{
					printf("get %d bytes of content: %s\n", ret, buf);
				}
			}
		}
        else
        {
			printf("something else happened \n");
        }
	}
}
int main(int argc, char* argv[]){
	const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
    
    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);
    
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret!=-1);
    
    ret = listen(listenfd,5); 	//内核监听队列的最大长度为5
    assert(ret!=-1);
    
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listenfd, true);
    
    while(1)
    {
		int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
    	if(ret<0)
        {
        	printf("epoll failure\n");
        	break;
        }
        lt(events, ret, epollfd, listenfd);	/* 使用LT模式 */
        //et(events, ret, epollfd, listenfd); /* 使用ET模式 */
    }
    close(listenfd);
    return 0;
}

```



## EPOLLONESHOT事件

**问题：**

使用ET模式，一个socket上的某个事件还是可能触发多次。

如果一个线程刚刚读完socket数据，然后去处理读取的数据，而socket上又有新的数据（再次触发EPOLLIN事件）,此时另一个新线程处理这些数据，于是**出现了两个线程同时操作一个socket**的局面。这不是我们所期望的。

EPOLLONESHOT可以解决此问题。当一个线程处理某个socket，其他线程不可以操作该socket。等线程处理完该socket，重置EPOLLONESHOT事件。

**！！！！以下代码没写完**

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024

struct fds
{
	int epollfd;
    int sockfd;
};
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}
/* 将文件描述符fd上的EPOLLIN和EPOLLET事件注册到epollfd指示的epoll内核事件表中,参数 oneshot 指定是否注册fd上的EPOLLONESHOT事件 */
void addfd(int epollfd, int fd, bool oneshot){
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;		// EPOLLIN 表示可读事件
	if(oneshot){
		event.events |= EPOLLONESHOT;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

/* 重置fd上的事件。这样操作之后，尽管fd上的EPOLLONESHOT事件被注册，但是操作系统仍然会触发fd上的EPOLLIN事件，且只触发一次 */
void reset_oneshot(int epollfd, int fd){
	epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

/* 工作线程 */
void* worker(void* arg)
{
	int sockfd = ( (fds*)arg )->sockfd;
    int epollfd = ( (fds*)arg )->epollfd;
    printf("start new thread to receive data on fd: %d\n", sockfd);
    char buf[ BUFFER_SIZE ];
    memset(buf, '\0', BUFFER_SIZE);
    
}
```

## ⭐select,poll,epoll三者区别

select和poll采用**轮询**的方式。每次调用都要扫描整个注册（被监听的）的文件描述符集合。算法事件复杂度O(n)。

epoll_wait采用的是回调的方式。内核检测到就绪的文件描述符会触发回调函数，不需要轮询。算法事件复杂度O(1)。

![image-20230330212114123](./assets/image-20230330212114123.png)

## I/O复用应用--非阻塞的connect

 在 TCP socket 被设置为**非阻塞的情况下调用 connect ，若没有立即返回成功，则会返回 -1 以及 errno =EINPROGRESS的错误，其表示连接操作正在进行中，但是尚未完成，与此同时 TCP 三次握手操作会同时进行。**在这之后，我们可以通过调用 select 来检查这个链接是否建立成功。 

在 socket 是**阻塞模式下 connect 函数会一直到有明确的结果才会返回**（或连接成功或连接失败），如果服务器地址“较远”，连接速度比较慢，connect 函数在连接过程中可能会导致程序阻塞在 connect 函数处好一会儿（如两三秒之久）。

在广域网中，connect函数可能需要比较长的时间返回（等待对端发送ack），所以我们通常需要非阻塞connect。

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 1023

/* 将文件描述符设置为非阻塞 */
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);	// 获取文件描述符 旧的状态标志
	int new_option = old_option | O_NONBLOCK;	// 设置非阻塞标志
	fcntl(fd, F_SETFL, new_option);
	return old_option;	// 返回 旧的状态标志，以便日后恢复该状态标志
}

/* 超时连接函数，参数分别是服务器IP地址、端口号和超时时间(毫秒)。函数成功时返回已经处于连接状态的socket，失败则返回-1 */
int unblock_connect(const char* ip, int port, int time)
{
	int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    
    int sockfd = 
}

```



## I/O复用应用--聊天室程序

### 客户端

客户端程序使用poll同时监听用户输入和网络连接，并利用splice函数将用户输入内容直接定向到网络连接上并发送。

```c
#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>

#define BUFFER_SIZE 64
int main(int argc, char* argv[]){
    if(argc <= 2){
		// 报错
    }
    const char* ip = argv[1];
	int port = atoi(argv[2]);	//atoi: string->int
    
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port = htons(port);
    
    int sockfd = socket(PF_INET,SOCK_STREAM,0);
    assert(sockfd >= 0);
    if( connect(sockfd,(struct sockaddr* )&server_address, sizeof(server_address))<0)
    {
        printf("connection failed\n");
        close(sockfd);
        return 1;
	}
    
	struct pollfd fds[2];
    /*	注册文件描述符0（标准输入） 和文件描述符sockfd上的可读事件	
    	struct pollfd{
            int fd;	//文件描述符
            short events;	//注册的事件 poll需要监听的事件
            short revents;	//实际发生的事件，由内核填充
        }
    */
    fds[0].fd = 0;
    fds[0].events = POLLIN;	// 监听（标准输入）可读事件
    fds[0].revents = 0;
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;	// 监听可读事件 或者 TCP连接关闭事件
    fds[1].revents = 0;
    
    char read_buf[BUFFER_SIZE];
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret!=-1);
    
    while(1){
        /*
        poll(fds,nfds,timeout)
            @param 	fds: 监听的事件集合
            @param	nfds: fds的大小
            @param	timeout: 单位毫秒
                        -1	将永远阻塞直到某个事件发生
                        0	立即返回
            @return:
                和select一样
		*/
		ret = poll(fds,2,-1);
        if(ret < 0)
        {
			printf("poll failure\n");
        	break;
        }
        if(fds[1].revents & POLLRDHUP)
        {
			printf("server close the connection\n");
        	break;
        }
        else if(fds[1].revents & POLLIN)
        {
			memset(read_buf,'\0',BUFFER_SIZE);//清空read_buf,'\0'为空字符
            recv(fds[1].fd,read_buf,BUFFER_SIZE-1,0);
            printf("%s\n",read_buf);
        }
        if(fds[0].revents & POLLIN)
        {
			/*	使用splice将用户输入的数据直接写到sockfd上	*/
			/*		
			pipedfd是管道
			往pipedfd[1]写入的数据，可以从pipedfd[0]读出。且pipedfd[0]只能从管道读数据，pipedfd[1]只能用于往管道写入数据。
			*/
            
            ret = splice(0,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE | SPLICE_F_MOVE);
            ret = splice(pipefd[0],NULL,sockfd,NULL,32768,SPLICE_F_MORE | SPLICE_F_MOVE);
        }
    }
    close(sockfd);
    return 0;
}
```

### 服务器

服务器程序使用poll同时管理 监听socket和连接socket，并且使用牺牲空间换取时间的策略来提高服务器性能。

```cpp
#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>

#define USER_LIMIT 5	/* 最大用户数量 */
#define BUFFER_SIZE 64 	/* 缓冲区大小 */
#define FD_LIMIT 65535	/* 文件描述符数量限制 */

/* 客户数据：客户端socket地址、待写到客户端的数据、从客户端读入的数据	*/
struct client_data
{
    struct sockaddr_in address;
    char* write_buf;
    char buf[BUFFER_SIZE];
};
/* 将文件描述符设置为非阻塞 */
int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);	// 获取文件描述符 旧的状态标志
	int new_option = old_option | O_NONBLOCK;	// 设置非阻塞标志
	fcntl(fd, F_SETFL, new_option);
	return old_option;	// 返回 旧的状态标志，以便日后恢复该状态标志
}

int main(int argc, char* argv[]){
	if(argc <= 2){
		// 报错
    }
    const char* ip = argv[1];
	int port = atoi(argv[2]);	//atoi: string->int
    int ret=0;
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
    
    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);
    
    ret = bind(listenfd, ( struct sockaddr* )&address, sizeof(address) );
    assert(ret != -1);
    
    ret = listen(listenfd, 5);
    assert(ret != -1);
    
    /* 创建users数组*/
    struct client_data* users = new client_data[FD_LIMIT];
    struct pollfd fds[USER_LIMIT+1];
    int user_counter = 0;
    for(int i=1; i<USER_LIMIT;++i){
    	fds[i].fd = -1;
    	fds[i].events = 0;
    }
    fds[0].fd = listenfd;
    fds[0].events = POLLIN | POLLERR;	// 监听可读事件 或 错误
    fds[0].revents = 0;
    
    while(1){
		ret = poll(fds,user_counter+1,-1);
		if(ret < 0){
        	printf("poll failure\n");
        	break;
		}
        for(int i=0; i<user_counter+1;++i){
			if((fds[i].fd == listenfd) && (fds[i].revents & POLLIN))
            {
				struct sockaddr_in client_address;
            	socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd,(struct sockaddr*)&client_address, &client_addrlength );
                if(connfd < 0){
                	printf("errno is: %d\n",errno);
                	continue;
                }
                /* 如果请求太多，关闭新连接 */
                if(user_counter >= USER_LIMIT)
                {
                	const char* info = "too many users\n";
                	printf("%s",info);
                    send(connfd ,info, strlen(info),0);	// 向客户端发送信息
                    close(connfd);
                    continue;
                }
            /* 对于新的连接，同时修改fds和user数组。users[connfd]对应于新连接文件描述符connfd的客户数据 */
                user_counter++;
                users[connfd].address = client_address;
                setnonblocking(connfd);	/* 设置为非阻塞connect */
                fds[user_counter].fd = connfd;
                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_counter].revents = 0;
                printf("comes a new user,now have %d users\n",user_counter);
            }
            else if(fds[i].revents & POLLERR)
            {
				printf("get an error from %d\n", fds[i].fd);
                char errors[100];
                memset(errors,'\0',100);
                socklen_t length = sizeof(errors);
                
                if(getsockopt(fds[i].fd,SOL_SOCKET, SO_ERROR,&errors, &length)<0){
        			printf("get socket option failed\n");
                }
                continue;
            }
            else if(fds[i].revents & POLLRDHUP){
			/*	如果客户端关闭连接，则服务器也关闭对应的连接，并将用户总数减1	*/
                users[fds[i].fd] = users[fds[user_counter].fd];
                close(fds[i].fd);
                fds[i] = fds[user_counter];
                i--;
                user_counter--;
                printf("a client left\n");
            }
            else if(fds[i].revents & POLLIN)
            {
            /*	监听到服务器有可读数据	*/
            	int connfd = fds[i].fd;
            	memset(users[connfd].buf,'\0',BUFFER_SIZE);
                ret = recv(connfd, users[connfd].buf, BUFFER_SIZE-1,0);
                printf("get %d bytes of client data %s from %d\n",ret,users[connfd].buf,connfd);
                if(ret < 0){
					/*	如果读操作出错，则关闭连接	*/
                    if(errno != EAGAIN)
                    {
                    	close(connfd);
                    	users[fds[i].fd] = users[fds[user_counter].fd];
                        fds[i] = fds[user_counter];
                        i--;
                        user_counter--;
                    }
                }
                else if(ret == 0)
                {
            	}
                else{
					/*	如果接收到客户数据，则通知其他socket连接准备写数据	*/
                    for(int j=1;j<user_counter;++j){
                        if(fds[j].fd == connfd){
                    		continue;	// 跳过 当前发送给服务器数据的客户端
                    	}
                        /*	注册fds的可写事件	*/
                    	fds[j].events |= ~POLLIN;
                    	fds[j].events |= POLLOUT;
                    	users[fds[j].fd].write_buf = users[connfd].buf;
                    }
                }
            }
            else if( fds[i].revents & POLLOUT){
                 /*		监听到服务器有写入的数据	*/
				int connfd = fds[i].fd;
            	if(!users[connfd].write_buf){
                	continue;
                }
                ret = send(connfd, users[connfd].write_buf,strlen(users[connfd].write_buf),0);
                users[connfd].write_buf = NULL;
                /*	写完后重新注册fds的可读事件	*/
                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }
        }
	}
    delete[] users;	//delete[]删除整个数组
    close(listenfd);
    return 0;
}
```

`client.c`接收两个参数：IP地址、端口号。

`server.cpp`接收两个参数：IP地址、端口号。

`gcc client.c -o client`生成可执行程序`client`

`g++ server.cpp -o server`生成可执行程序`server`

建立多人聊天室

首先一定先运行server用于建立监听socket

`./server 本地ip 12345`

`./client 本地ip 12345`	第一个用户进入聊天室

`./client 本地ip 12345`	第二个用户进入聊天室

……

`./client 本地ip 12345`	第二个用户进入聊天室

## I/O复用应用--同时处理TCP和UDP服务

> 以下代码未验证是否可执行

```c
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512 
#define UDP_BUFFER_SIZE 1024

int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int	new_option = old_option | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
}
/* 将文件描述符fd上的EPOLLIN(可读事件)注册到epollfd指示的epoll内核事件表中,启用ET模式 */
void addfd(int epollfd, int fd)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
	setnonblocking(fd);
}

int main(int argc, char* argv[])
{
	if(argc < =2){
    	printf("usage: %s ip_address port_number\n",basename(argv[0]));
		return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
    /*	创建TCP socket,并将其绑定到端口port上	*/
    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd >= 0);
    
    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address) );
    assert(ret != -1);
    
    ret = listen(listenfd, 5);
    assert(ret != -1);
    
    /*	创建UDP socket,并将其绑定到端口port上	*/
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
    int udpfd = socket(PF_INET,SOCK_DGRAM,0);
    assert(udpfd >= 0);
    
    ret = bind(udpfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);
    
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
	/*	注册TCP socket 和 UDP socket 上的可读事件	*/
    addfd(epollfd, listenfd);
    addfd(epollfd, udpfd);
    
    while(1)
    {
		int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1);
        if(number < 0)
        {
        	printf("epoll failure\n");
        	break;
        }
        for(int i=0; i<number; i++){
        	int sockfd = events[i].data.fd;
        	if( sockfd == listenfd){
            	struct sockaddr_in client_address;
            	socklen_t client_addrlength = sizeof(client_address);
                int connfd = aceept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
                addfd(epollfd,connfd);
            }
            else if(sockfd == udpfd)
            {
            	char buf[UDP_BUFFER_SIZE];
            	memset(buf,'\0',UDP_BUFFER_SIZE);
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                
                ret = recvfrom(udpfd, buf, UDP_BUFFER_SIZE-1,0,(struct sockaddr*)&client_address, &client_addrlength);
                if( ret>0 ){
					sendto(udpfd, buf, UDP_BUFFER_SIZE-1,0,(struct sockaddr* )&client_address, client_addrlength);
                }
            }
            else if(events[i].events & EPOLLIN)
            {
            	char buf[TCP_BUFFER_SIZE];
            	memset(buf,'\0',UDP_BUFFER_SIZE);
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                
                ret = recvfrom(udpfd, buf, UDP_BUFFER_SIZE-1, 0, (struct sockaddr*)&client_address, &client_addrlength);
                
                if(ret > 0)
                {
					sendto(udpfd, buf, UDP_BUFFER_SIZE-1, 0,(struct sockaddr*)&client_address, client_addrlength);
                }
            }else if(events[i].events & EPOLLIN)
            {
            	char buf[TCP_BUFFER_SIZE];
            	while(1)
                {
                	memset(buf, '\0', TCP_BUFFER_SIZE);
                	ret = recv(sockfd, buf, TCP_BUFFER_SIZE-1,0);
                	if(ret < 0){
                    	if((errno == EAGAIN) || (error == EWOULDBLOCK))
                        {
                        	break;
                        }
                        close(sockfd);
                        break;
                    }
                    else if(ret == 0)
                    {
						close(sockfd);
                    }
                    else
                    {
						send(sockfd,buf,ret,0);
                    }
                }
            }
            else
            {
				printf("something else happened \n");
            } 
        }
    }
    close(listenfd);
	return 0;
}
```



# 信号

信号是由用户、系统或者进程发送给目标进程的信息，已通知目标进程某个状态的改变或系统异常。

- 对于前台进程，用户可以通过输入特殊的终端字符来给它发送信号。比如输入Ctrl+C通常会给进程发送一个中断信号。
- 系统异常。比如浮点异常和非法内存段访问。
- 系统状态变化。比如 alarm定时器到期将引起SIGALRM信号。
- 运行kill命令或调用kill函数。

## Linux信号

### 发送信号

```c
#include <sys/types.h>
#include <signal.h>
/*
把信号 sig 发送给目标进程 pid
*/
int kill(pid_t pid, int sig);
```

![image-20230414144201600](./assets/image-20230414144201600.png)



## 信号函数

```c
#include <signal.h>
_sighandler_t signal(int sig, _sighandler_t _handler)
    

/*
相较于 signal 更好的信号处理函数
@param	sig: 要捕获的信号类型
@param	act: 指定新的信号处理方式
@param	oact: 输出信号先前的处理方式
@return 0: 成功 
        -1: 失败
*/
int sigaction(int sig, const struct sigaction* act, struct sigaction* oact);

/*


struct sigaction
{
#ifdef __USE_POSIX199309
	union
	{
		_sighandler_t sa_handler;
		void (*sa_sigaction) (int, siginfo_t*, void* );
	}
	_sigaction_handler;
# define sa_handler __sigaction_handler.sa_handler	\\	sa_handler 信号处理函数
# define sa_sigaction __sigaction_handler.sa_sigaction	
#else
	_sighandler_t sa_handler;
#endif
	_sigset_t sa_mask;		\\	sa_mask 设置进程的信号掩码，指定哪些信号不能发送给本进程
	int sa_flags;
	void (*sa_restorer)	(void);	
};
*/

```



## 信号集

```c
//	Linux 的 sigset_t 表示一组信号
#include <bits/sigset.h>
#define _SIGSET_NWORDS (1024 / (8 * sizeof (unsigned long int)))
typedef struct
{
    unsigned long int __val[_SIGSET_NWORDS];
}__sigset_t;
//	sigset_t 实际上是一个长整型数组，数组的每个元素的每个为表示一个信号。


//	信号集函数
#include <signal.h>
int sigemptyset (sigset_t* _set)	//	清空信号集
int	sigfillset(sigset_t* _set)	//	在信号集中设置所有信号
int	sigaddset(sigset_t* _set,int _signo)	//	将信号_signo添加至信号集中
int sigdelset(sigset_t*	_set,int _signo)	//	将信号_signo从信号集中删除
int sigismember(_const sigset_t* _set, int _signo)	//	测试_signo是否在信号集中
```

### 进程信号掩码

```c
#include <signal.h>
/*
设置和查看进程的信号掩码
@param _how: 指定设置进程信号掩码的方式
@param _set: 指定新的信号掩码。
@param _oset: 输出原来的信号掩码。_set=NULL时，_oset输出当前信号掩码。
@return	0: 成功
        -1：失败，并设置errno
*/
int sigprocmask( int _how, _const sigset_t* _set, sigset_t* _oset );
```

![image-20230425214422341](./assets/image-20230425214422341.png)

### 被挂起的信号

设置进程信号掩码后，被屏蔽的信号将不能被进程接收。如果给进程发送一个被屏蔽的信号，则操作系统将信号设置为进程的一个被挂起信号。如果我们取消对被挂起信号的屏蔽，则它能立即被进程接收到。如下函数可以获得进程当前被挂起的信号集：

```c
#include <signal.h>
/*
@param set: 用于保存被挂起的信号集。
*/
int sigpending( sigset_t* set );
```

`fork`调用产生的**子进程将继承父进程的信号掩码，但具有一个空的挂起信号集**。

## 信号捕捉流程

1. 用户为某信号注册一个信号处理函数`sighandler`。
2. 当前正在执行主程序，这时候因为中断、异常或系统调用进入内核态。
3. 在处理完异常要返回用户态的主程序之前，检查到有信号未处理，并发现该信号需要按照用户自定义的函数来处理。
4. 内核决定返回用户态执行`sighandler`函数，而不是恢复`main`函数的上下文继续执行！（`sighandler`和`main`函数使用的是不同的堆栈空间，它们之间不存在调用和被调用的关系，是两个独立的控制流程）
5. `sighandler`函数返回后，执行特殊的系统调用`sigreturn`从用户态回到内核态
6. 检查是否还有其它信号需要递达，如果没有 则返回用户态并恢复主程序的上下文信息继续执行。

## 统一事件源

**信号是一种异步事件，将其和IO事件统一起来交给主程序处理叫做统一事件源**

一般方法是：信号发送函数将信号值写到管道一端，然后主程序采用IO复用技术监听管道读端，一旦可读事件发生那么主程序可以在事件处理逻辑中定义信号处理方法，这样就和IO事件处理一致。

```cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>
#define MAX_EVENT_NUMBER 1024
static int pipefd[2];

int setnonblocking( int fd )
{
    int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}
void sig_handler( int sig )
{
    /*	
    保留原来的errno,在函数最后恢复，以保证函数的可重入性
    */
    int save_errno = errno;
    int msg = sig;
    send( pipefd[1], ( char* )&msg, 1, 0 );	/*	将信号值写入管道，以通知主循环	*/
    errno = save_errno;
}
/*	设置信号的处理函数	*/
void addsig( int sig )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );	//	注册要捕获信号的sig。若触发信号sig，交给sa处理
}

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf( "usage: %s ip_address port_number\n", basename( argv[0]) );
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );
    
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
    
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );
    
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    if( ret == -1 )
    {
        printf( "errno is %d\n", errno );
    }
    ret = listen( listenfd, 5 );
    assert( ret != -1 );
    
    epoll_event events[ MAX_EVENT_NUMBER ];
    int epollfd = epoll_create( 5 );
    assert( epollfd != -1 );
    addfd( epollfd, listenfd );
    
	/*	使用 socketpair 创建管道，注册pipefd[0]上的可读事件	*/
    ret = sockpair( PF_UNIX, SOCK_STREAM, 0, pipefd );
    assert( ret != -1 );
    setnonblocking( pipefd[1] );
    addfd( epollfd, pipefd[0] );
    
    /*	设置一些信号的处理函数	*/
    addsig( SIGHUP );
    addsig( SIGCHLD );
    addsig( SIGTERM );
    addsig( SIGINT );
    bool stop_server = false;
    
    while( !stop_server )
    {
    	int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1 );
    	if( ( number < 0 ) && ( errno != EINTR ) )
        {
            printf( "epoll failure\n" );
            break;
        }
        
        for( int i=0; i<number; i++)
        {
            int sockfd = events[i].data.fd;
            /*	如果就绪的文件描述符是listenfd,则处理新的连接	*/
            if( sockfd == listenfd )
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                addfd( epollfd, connfd );
            }
            /*	如果就绪的文件描述符是pipefd[0],则处理信号	*/
            else if( ( sockfd == pipefd[0] ) && ( events[i].events & EPOLLIN ) )
            {
                int sig;
                char signals[1024];
                ret = recv( pipefd[0], signals, sizeof( signals ), 0 );
                if( ret == -1 )
                {
                    continue;
                }
                else if( ret == 0 )
                {
                    continue;
                }
                else
                {
                    /*	因为每个信号值占1个字节，所以按字节来逐个接收信号。我们以SIGTERM为例，来说明如何终止服务器主循环	*/
                    for( int i=0; i<ret; ++i){
                        switch( signals[i] )
                        {
                            case SIGCHLD:
                            case SIGHUP:
                            {
                            	continue;        
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                            	stop_server = true;
                            }
                        }
                    }
                }
            }
            else
        	{
        	}
        }
    }
    printf( "colse fds\n" );
    close( listenfd );
    close( pipefd[1] );
    close( pipefd[0] );
    return 0;
}
```

**为什么要保证信号处理函数`sig_handler`的可重入性。**

中断可能发生在任务执行的任何时间点。一个函数的执行期间被中断后，到重新恢复到断点进行执行的过程中，要保证函数所依赖的环境没有发生改变。所以应当在`sig_handler`处理信号前保存`errno`，并在其后恢复`errno`。 

## 网络编程相关的信号

### SIGHUP

### SIGPIPE

### SIGURG



# 定时器

网络程序需要处理的三类事件：

- I/O复用
- 信号
- 定时事件

![image-20230424145547437](./assets/image-20230424145547437.png)

Linux提供的三种定时机制：

- `socket`选项`SO_RCVTIMEO`和`SO_SNDTIMEO`
- `SIGALRM`信号
- `I/O`复用系统调用的超时参数



## 高性能定时器

### 1.时间轮

![image-20230424151204848](./assets/image-20230424151204848.png)

### 2.事件堆

小顶堆实现



# 多进程

## fork创建新进程

```c
#include <sys/types.h>
#include <unistd.h>
pid_t fork(void);
```

该函数每次返回两次，在父进程中返回的是子进程的**PID(Process ID，表示进程号)**，在子进程中则返回0；

**PPID**是父进程的PID

## exec系列系统调用

## 处理僵尸进程

父进程结束，而子进程还没结束，此时子进程就是僵尸进程。**子进程的PPID会被设置为1，即init进程。init进程接管僵尸进程，并等待它结束。**

```c
#include <sys/types.h>
#include <sys/wait.h>
/*
	将阻塞进程，直到该进程的某个子进程结束运行为止。
*/
pid_t wait(int* stat_loc);

/*
	将阻塞进程，直到该进程指定的pid子进程结束运行为止。pid = -1时和wait相同
*/
pid_t waitpid( pid_t pid, int* stat_loc, int options);
```

## 信号量

**引发进程之间的竞争的代码段称为**关键代码段，或者**临界区**

P:传递，进入临界区

V:释放，退出临界区

![image-20230412150032114](./assets/image-20230412150032114.png)

### semget系统调用

创建一个新的信号量集，或者获取一个已经存在的信号量集。

```c
#include <sys/sem.h>
int semget(key_t key, int num_sems, int sem_flags);
```



## 共享内存

```c
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
创建/打开POSIX共享内存对象
@param	name:创建/打开的共享内存对象
@param	oflag:创建方式。
@param	
*/
int shm_open(const char* name, int oflag, mode_t mode);

/*
关闭POSIX共享内存对象
*/
int shm_unlink(const char* name);
```

编译时需要指定链接选项`-lrt`

## 聊天服务器 (多进程+共享内存版)

一个子进程处理一个客户连接。同时将所有客户socket连接的读缓冲设计为一块共享内存。

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define USER_LIMIT 5
#define BUFFER_SIZE 1024
#define FD_LIMIT 65535
#define MAX_EVENT_NUMBER 1024
#define PROCESS_LIMIT 65536

/*	处理一个客户连接必要的数据	*/
struct client_data
{
	sockaddr_in address;	//	客户端的socket地址
	int connfd;				//	socket 文件描述符
    pid_t pid;				//	处理这个连接的子进程的pid
    int pipefd[2];			//	和父进程通信的管道
};

static const char* sha_name = "/my_shm";
int sig_pipefd[2];
int epollfd;
int listenfd;
int shmfd;
char* share_mem = 0;

/*	客户连接数组。进程用客户连接的编号来索引这个数组，即可取得相关的客户连接数据	*/
client_data* users = 0;
/*	子进程和客户连接的映射关系表。用进程的PID来索引这个数组，即可取得该进程所处理的客户连接的编号	*/
int* sub_process = 0;
/*	当前客户数量	*/
int user_count = 0;
bool stop_child = false;

int setnonblocking(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int	new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd, int fd)
{
	epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnonblocking(fd);
}

void sig_handler(int sig)
{
	int save_errno = errno;
    int msg =sig;
    send(sig_pipefd[1], (char* )&msg, 1, 0);
    errno = save_errno;
}

/* 设置信号的处理函数 */
void addsig(int sig, void(*handler)(int), bool restart = true)
{
	struct sigaction sa;
    memset(&sa, '\0',sizeof(sa));
    sa.sa_handler = handler;
    if(restart)
    {
		sa.sa_flags |= SA_RESTART;
    }
    sigfillset( &sa.sa_mask);
    assert(sogaction(sig,&sa,NULL) != -1);
}

void del_resource()
{
	close(sig_pipefd[0]);
    close(sig_pipefd[1]);
    close(listenfd);
    close(epollfd);
    shm_unlink(shm_name);
    delete [] users;
    delete [] sub_process;
}

void child_term_handler(int sig)
{
	stop_child = true;
}
/*	子进程运行的函数。参数idx指出该子进程处理的客户连接的编号，users是保存所有客户连接数据的数组，users是保存所有客户连接数据的数组，参数share_mem指出共享内存的起始地址	*/
int run_child(int idx, client_data* users, char* share_mem)
{
	epoll_event events[MAX_EVENT_NUMBER];
    /*	子进程使用I/O复用技术同时监听两个文件描述符：客户连接socket、与父进程通信的管道文件描述符	*/
    assert(child_epollfd != -1);
    int connfd = users[idx].connfd;
    addfd(child_epollfd,connfd);
    int pipefd = users[idx].pipefd[1];
    addfd(child_epollfd, pipefd);
    int ret;
    /*	子进程需要设置自己的信号处理函数	*/
    addsig(SIGTERM, child_term_handler, false);
    
    while(!stop_child)
    {
		int number = epoll_wait(child_epollfd, events, MAX_EVENT_NUMBER, -1);
        if((number < 0) && (errno != EINTR))
        {
			printf("epoll failure\n");
            break;
        }
        for(int i=0; i<number;i++){
			int sockfd = events[i].data.fd;
            /*	本子进程负责的客户连接有数据到达	*/
            if((sockfd == connfd ) && (events[i].events & EPOLLIN))
            {
            	memset(share_mem + idx*BUFFER_SIZE,'\0',BUFFER_SIZE);
            	/*	将客户数据读取到对应的读缓存中。读缓存是共享内存的一段，他开始于idx*BUFFER_SIZE处，长度为BUFFER_SIZE字节。因此，各个客户连接的读缓存是共享的	*/
             	ret = recv(connfd, share_mem + idx*BUFFER_SIZE, BUFFER_SIZE-1, 0);
            	if(ret < 0)
                {
                    /*	EAGAIN 表示没有数据可读,请稍后再试 */
					if(errno != EAGAIN)
                    {
						stop_child = true;
                    }
                }
                /*	recv 返回0 表示对方关闭了通信 */
                else if( ret == 0){
					stop_child = true;
                }
                else
                {
                    /*	成功读取客户数据后 (通过管道)通知主进程来处理	*/
					send(pipefd,(char* )&idx, sizeof(idx),0);
                }
            }
            /*	主进程通知本进程（通过管道）将第client个客户的数据发送到本进程负责的客户端	*/
            else if((sockfd == pipefd) && (events[i].events & EPOLLIN))
            {
				int client = 0;
                 /*	接收主进程发送来的数据，即有客户数据到达的连接的编号	*/
				ret = recv(sockfd, (char*)&client, sizeof(client),0);	//	&client 是client的地址， 将接收到的char信息存储到client的地址中。 个人觉得这样写有问题，client地址存储的值发生了类型转换 int->char
                
                if(ret < 0){
                	if(errno != EAGAIN)
                    {
                    	stop_child = true;
                    }
                }else if(ret == 0)
                {
                	stop_child = true;    
            	}
                else
                {
                	send(connfd, share_mem + client * BUFFER_SIZE, BUFFER_SIZE, 0);    
            	}
            }
            else{
				continue;
            }
        }
    }
    close(connfd);
    close(pipefd);
    close(child_epollfd);
    return 0;
}


int main(int argc, char* argv[])
{
	if(argc <= 2){
		printf("usage: %s ip_address port_number\n ",basename(argv[0]));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2]);
    
    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr);
    address.sin_port = htons( port );
    
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    
    ret = bind(listenfd, (struct sockaddr* )&address, sizeof(address));
    assert(ret != -1);
    
    user_count = 0;
    users = new client_data [USER_LIMIT + 1];
    sub_process = new int [ PROCESS_LIMIT ];
    for( int i=0; i<PROCESS_LIMIT; ++i)
    {
		sub_process[i] = -1;
    }
    epoll_event events[ MAX_EVENT_NUMBER ];
    epollfd = epoll_create(5);
    assert( epollfd != -1);
    addfd(epollfd, listenfd);
    /* 创建一组套接字对sig_pipefd,用于全双工通信，信号处理函数和主进程通信
    */
    ret = socketpair( PF_UNIX, SOCK_STREAM, 0, sig_pipefd);	
    assert( ret != -1);
    setnoblocking( sig_pipefd[1] );
    addfd( epollfd, sig_pipefd[0] );	// 注册sig_pipefd[0]上的可读事件
    
    addsig( SIGCHLD, sig_handler);	// SIGCHLD 子进程停止或终止
    addsig( SIGTERM, sig_handler);	// SIGTERM 软件终止信号
    addsig( SIGINT, sig_handler);	// SIGINT 来自键盘的终端
    addsig( SIGPIPE, SIG_IGN);		// SIGPIPE 对一个管道进行写的操作
    bool stop_server = false;
    bool terminate = false;
    
    /* 创建共享内存， 作为所有客户 socket 连接的读缓存 */
    shmfd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    assert(shmfd != -1);
    ret = ftruncate(shmfd, USER_LIMIT * BUFFER_SIZE);
    assert(ret != -1);
    
    share_mem = (char*)mmap(NULL, USER_LIMIT * BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    assert( share_mem != MAP_FAILED);
    close(shmfd);
    
    while( !stop_server){
		int number = epoll_wait( epollfd, events, MAX_EVENT_NUMBER, -1);
		if(( number < 0 ) && (errno != EINTR)){
        	printf("epoll failure\n");
        	break;
        }
        for(int i=0; i<number; i++){
        	int sockfd = events[i].data.fd;
        	/*	新的客户连接到来	*/
            if( sockfd == listenfd ){
            	struct sockaddr_in client_address;
            	socklen_t client_addrlength = sizeof( client_address );
                int connfd = accept( listenfd, ( struct sockaddr* )&client_address, &client_addrlength );
                if( connfd < 0){
                	printf( "errno is: %d\n", errno);
                    continue;
                }
                if( user_count >= USER_LIMIT ){
                	const char* info = "too many users\n";
                    printf("%s",info);
                    send(connfd, info, strlen( info ), 0);
                    close( connfd );
                    continue;
                }
            	/*	保存第user_count个客户连接的相关数据	*/
                users[user_count].address = client_address;
                users[user_count].connfd = connfd;
                /*	主进程和子进程间建立管道，以传递必要的数据	*/
                ret = socketpair( PF_UNIX, SOCK_STREAM, 0, users[user_count].pipefd );
                assert( ret != -1);
                pip_t pid = fork();
                if( pid < 0){
                	close( connfd );
            		continue;
                }else if(pid == 0)
                {
                    /* pid == 0 说明当前在子进程中 */
                    close(epollfd);
                    close(listenfd);
                    close(users[user_count].pipefd[0]);
                    close(sig_pipefd[0]);
                    close(sig_pipefd[1]);
                    run_child( user_count, users, share_mem );
                    munmap( (void*)share_mem, USER_LIMIT* BUFFER_SIZE);
                    exit(0);
                }else
                {
                    close( connfd );
                    close( users[user_count].pipefd[1]);
                    addfd( epollfd, users[user_count].pipefd[0]);
                    users[user_count].pid = pid;
                	/* 记录新的客户连接在数组users中的索引值，建立进程pid和该索引值之间的映射关系*/
                    sub_process[pid] = user_count;
                    user_count++;
                }
            }
            /*	处理信号事件	*/
            else if( (sockfd == sig_pipefd[0]) && (events[i].events & EPOLLIN))
            {
            	int sig;
                char signals[1024];
                ret = recv( sig_pipefd[0], signals, sizeof( signals), 0);
                if( ret == -1)
                {
                    continue;
                }
                else
                {
                    for(int i=0;i<ret;++i)
                    {
                        switch( signals[i] )
                        {
                            /*	子进程退出，表示有某个客户端关闭了连接	*/
                            case SIGCHLD:
                            {
                                pid_t pid;
                                int stat;
                                while( ( pid = waitpid(-1, &stat, WROHANG)) > 0)
                                {
                                    /*	用子进程的pid取得被关闭的客户连接的编号	*/
                                    int del_user = sub_process[pid];
                                    sub_process[pid] = -1;
                                    if( (del_user < 0) || (del_user > USER_LIMIT))
                                    {
                                        continue;
                                    }
                                    /*	清除第 del_user 个客户连接使用的相关数据	*/
                                    epoll_ctl(epollfd, EPOLL_CTL_DEL,users[del_user].pipefd[0],0);
                                    close(users[del_user].pipefd[0]);
                                    users[del_user] = users[--user_count];
                                    sub_process[users[del_user].pid] = del_user;
                                }
                                if( terminate && user_count == 0)
                                {
                                    stop_server = true;
                                }
                                break;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                            	/* 结束服务器程序	*/
                                printf("kill all the child now\n");
                                if(user_count == 0)
                                {
                                    stop_server = true;
                                    break;
                                }
                                for(int i = 0; i<user_count;++i)
                                {
                                    int pid = users[i].pid;
                                    kill(pid, SIGTERM);
                                }
                                terminate = true;
                                break;
                            }
                            default:
                            {
                            	break;        
                            }
                        }
                    }
                }
            }
        }
        /*	某个子进程向父进程写入了数据	*/
     	else if(events[i].events & EPOLLIN)
        {
            int child = 0;
            /*	读取管道数据，child变量记录了是哪个客户连接有数据到达	*/
            ret = recv( sockfd, (char*)&child, sizeof(child), 0);
            printf("read data from child accross pipe\n");
            if(ret == -1)
            {
                continue;
            }
            else if( ret == 0)
            {
                continue;
            }
            else
            {
                /* 向除负责处理第 child 个客户连接的子进程之外的其他子进程发送消息，通知它们有客户数据要写*/
                for(int j=0; j<user_count; ++j)
                {
                    if( users[j].pipefd[0] != sockfd)
                    {
                        printf("send data to child accross pipe\n");
                        send(users[j].pipefd[0],(char*)&child,sizeof(child),0);
                    }
                }
            }
        }
    }
    del_resource();
    return 0;
}
```



## 消息队列

消息队列 **在两个进程之间传递二进制数据块**。每个数据块都有一个特定的类型，接收方可以根据类型来有选择地接收数据，而不一定像管道和命名管道那样必须以先进先出的方式接收数据。

```c
#include <sys/msg.h>

/*	
创建一个消息队列/获取一个已有的消息队列。
@param key: 标识一个全局唯一的消息队列。
@param msgflg: 与semget的参数sem_flags一致
@return: 成功时返回一个正整数值(消息队列标识符)，失败返回-1
*/
int msgget(key_t key, int msgflg );

/*	
把一条消息添加到消息队列中。
*/
int msgsnd(int msqid, const void* msg_ptr, size_t msg_sz, int msgflg);


```

## ⭐Linux下进程间通信方式(IPC)

### 1.管道

无名管道:半双工，只能在具有亲缘关系的进程之间使用（如父子进程）

有名管道：半双工，允许没有亲缘关系的进程之间通信。

### 2.共享内存

映射一段能被其他进程所访问的内存，这段内存由一个进程创建，但多个进程都可以访问。

共享内存是最快的IPC方式。经常与**信号量**配合使用实现进程间的同步和通信。

### 3.消息队列

消息队列是有消息的链表，存放在内核中并由消息队列标识符标识。消息队列克服了信号传递信息少、管道只能承载无格式字节流以及缓冲区大小受限等缺点。

### 4.套接字

可以在不同机器间通信，也能在本地的两个进程间通信。

### 5.信号

用于通知 接收进程 某个事件已经发生，比如按下`ctrl+c`就是信号。

### 6.信号量

信号量是一个计数器，可以用来控制多个进程对共享资源的访问。他常作为一种锁机制，实现**进程、线程的对临界区的同步及互斥访问**。



# 多线程

POSIX线程(`pthread`)

## 线程模型

线程分为两种：内核线程	用户线程

线程实现的方式：

1. 完全在用户空间实现：无需内核支持。**线程库**负责管理所有执行线程，比如优先级、时间片等。M个用户线程对应1个内核线程.
2. 完全由内核调度：M:N=1:1,一个用户线程被映射为一个内核线程。
3. 双层调度：内核调度M个内核线程，线程库调度N个用户线程。



## 线程库



## 创建线程 和 结束线程

```c
#include <pthread.h>
/*	
创建一个线程
@param thread: 线程标识符
@param attr: 线程属性
@param start_routine: 新线程将运行的函数
@param arg: 新线程将运行的函数参数
*/
int pthread_create(pthread_t* thread, const pthread_attr_t* attr,
                  void* ( *start_routine)(void* ), void* arg );




```

## 线程同步机制

### 1.信号量

可以直接理解成计数器，信号量会有初值（>0），每当有进程申请进入临界区，通过一个P操作来对信号量进行-1操作，当计数器减到0的时候就说明没有资源了，其他进程要想访问就必须等待，当该进程脱离临界区之后，就会执行V操作来对信号量进行+1操作。

直到信号量值大于0时进程被唤醒，可以访问该临界区；

### 2.互斥锁

二进制的信号量。当进人关键代码段时，我们需要获得互斥锁并将其加锁,这等价于二进制信号量的Р操作﹔当离开关键代码段时，我们需要对互斥锁解锁，以唤醒其他等待该互斥锁的线程，这等价于二进制信号量的V操作。

```c
#include <pthread.h>

/*
初始化互斥锁
@param mutex: 互斥锁
@param mutexattr: 指定互斥锁的属性
@return: 成功返回0，失败返回错误码。下面的函数也相同。
*/
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* mutexattr );

/*
销毁互斥锁
*/
int pthread_mutex_destroy(pthread_mutex_t* mutex);

/*
给互斥锁加锁，如果mutex已经上锁，则pthread_mutex_lock调用将被阻塞，直到该互斥锁的占有者将其解锁。
*/
int pthread_mutex_lock(pthread_mutex_t* mutex);

/*
给互斥锁加锁，如果mutex已经上锁，则pthread_mutex_trylock调用将立即返回错误码。
*/
int pthread_mutex_trylock(pthread_mutex_t* mutex);

/*
解锁
*/
int pthread_mutex_unlock(pthread_mutex_t* mutex);
```

互斥锁属性

```c
#include <pthread.h>
/*	初始化互斥锁属性对象	*/
int pthread_mutexattr_init( pthread_mutexattr_t* attr );

/*	销毁互斥锁属性对象	*/
int pthread_mutexattr_destroy( pthread_mutexattr_t* attr );

/*	
获取互斥锁的pshared属性               
*/
int pthread_mutexattr_getpshared( const pthread_mutexattr_t* attr, int* pshared );

/*	
设置互斥锁的pshared属性
@param pshared: 
				PTHREAD_PROCESS_SHARED	互斥锁可以被跨进程共享。
				PTHREAD_PROCESS_PRIVATE	互斥锁只能被和锁的初始化线程隶属于同一个进程的线程共享。              
*/
int pthread_mutexattr_setpshared( pthread_mutexattr_t* attr, int pshared );

/*	获取互斥锁的type属性	*/
int pthread_mutexattr_gettype( const pthread_mutexattr_t* attr, int* type);

/*	
设置互斥锁的type属性
@param type: 
			PTHREAD_MUTEX_NORMAL	普通锁
			PTHREAD_MUTEX_ERRORCHECK 检错锁
			PTHREAD_MUTEX_RECURSIVE	嵌套锁
			PTHREAD_MUTEX_DEFAULT	默认锁
*/
int pthread_mutexattr_settype( pthread_mutexattr_t* attr, int type);
```

### 3.条件变量

如果互斥锁用于同步线程对共享数据的访问。条件变量用于在线程之间同步共享数据的值。

**条件变量提供了一种线程间的通信机制：当某个共享数据达到某个值的时候，唤醒等待这个共享数据的线程。**



三种线程同步机制的包装类`locker.h`

```c++
#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

/*	封装信号量的累	*/
class sem
{
public:
    /*	创建并初始化信号量	*/
    sem()
    {
        if(sem_init( &m_sem, 0, 0) != 0 )
        {
            /*	构造函数没有返回值，可以通过抛出异常来报告错误	*/
            throw std::exception();
        }
    }
    /*	销毁信号量	*/
    ~sem()
    {
        sem_destroy( &m_sem );
    }
    /*	等待信号量	*/
    bool wait()
    {
        return sem_wait( &m_sem ) == 0;
    }
    /*	增加信号量	*/
    bool post()
    {
        return sem_post( &m_sem ) == 0;
    }
private:
    sem_t m_sem;
};

/*	封装互斥锁的类	*/
class locker
{
pubilc:
    /*	创建并初始化互斥锁	*/
    locker()
    {
        if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
        {
            throw std::exception();
        }
    }
    /*	销毁互斥锁	*/
	~locker()
    {
		pthread_mutex_destroy( &m_mutex );
    }
    /*	获取互斥锁	*/
	bool lock()
    {
        return pthread_mutex_lock( &m_mutex ) == 0;
    }
    /*	释放互斥锁	*/
    bool unlock()
    {
        return pthread_mutex_unlock( &m_mutex ) == 0;
    }
private:
    pthread_mutex_t m_mutex;
    
};


/*	封装条件变量的类	*/
class cond
{
    public:
    	/*	创建并初始化条件变量	*/
    	cond()
        {
            if( pthread_mutex_init( &m_mutex, NULL ) != 0 )
            {
				throw std::exception();
            }
            if(pthread_cond_init( &m_cond, NULL ) != 0)
            {
                /*	构造函数中一旦出现问题，就应该立即释放已经成功分配了的资源	*/
                pthread_mutex_destroy( &m_mutex );
                throw std::exception();
            }
        }
    	/*	销毁条件变量	*/
    	~cond()
        {
            pthread_mutex_destroy( &m_mutex );
            pthread_cond_destroy( &m_cond );
        }
    	/*	等待条件变量	*/
    	bool wait()
        {
            int ret = 0;
            pthread_mutex_lock( &m_mutex );
            pthread_mutex_cond_wait( &m_cond, &m_mutex );
            pthread_mutex_unlock( &m_mutex );
            return ret == 0;
        }
    	/*	唤醒等待条件变量的线程	*/
    	bool signal()
        {
            return pthread_cond_signal( &m_cond ) == 0;
        }
private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
};
#endif
```





## 多线程环境

### 1.可重入函数

**如果一个函数能被多个线程同时调用且不发生静态条件，则我们称它是线程安全的，或者说它是可重入函数。**

### 2.线程和进程

![image-20230423194511547](./assets/image-20230423194511547.png)

```c
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <wait.h>

pthread_mutex_t mutex;
/*	子线程运行的函数。它首先获得互斥锁mutex，然后暂停5s，在释放该互斥锁	*/
void* another(void* arg)
{
    printf("in child thread, lock the mutex\n");
    pthread_mutex_lock( &mutex );
    sleep( 5 );
    pthread_mutex_unlock( &mutex );
}

int main()
{
    pthread_mutex_init( &mutex, NULL );
    pthread_t id;
    pthread_create( &id, NULL, another, NULL);
    /*	父进程中的主进程暂停1s,以确定在执行fork操作之前，子线程已经开始运行并获得了互斥变量mutex	*/
    sleep(1);
    int pid = fork();
    if(pid < 0)
    {
        pthread_join( id, NULL);
        pthread_mutex_destroy( &mutex );
        return 1;
    }else if( pid == 0 )
    {
        printf("I am in the child, want to get the lock\n");
    	/*	子进程从父进程继承了互斥锁mutex的状态，该互斥锁处于锁住的状态，这是由父进程中的子线程执行pthread_mutex_lock引出的，因此，下面这句加锁操作会一直阻塞，尽管从逻辑上来说它是不应该阻塞的	*/
        pthread_mutex_lock( &mutex );
        printf("I can not run to here, oop...\n");
        pthread_mutex_unlock( &mutex );
        exit(0);
    }
    else
    {
        wait(NULL);
    }
    pthread_join(id, NULL);
    pthread_mutex_destroy( &mutex );
    return 0;
}
```

![image-20230423201150527](./assets/image-20230423201150527.png)

```c
#include <pthread.h>
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void) );

```

![image-20230423202014529](./assets/image-20230423202014529.png)

```c
void prepare()
{
	pthread_mutex_lock( &mutex );
}
void infork()
{
    pthread_mutex_unlock( &mutex );
}
pthread_atfork( prepare, infork, infork );
```

### 3.线程和信号

```c
#include <pthread.h>
#include <signal.h>

/*
设置信号掩码。由于进程中的所有线程共享该进程的信号，线程库根据线程掩码决定把信号发送给哪个具体的线程，所以没必要所有子线程都单独设置信号掩码。
*/
int pthread_sigmask(int how, const sigset_t* newmask, sigset_t* oldmask);
```

我们应该定义一个专门的线程来处理所有信号。通过以下两个步骤：

1. 在主线程创建出其他子线程之前就调用`pthread_sigmask`来设置号信号掩码，所有新创建的子线程都将自动继承这个信号掩码。这样做之后，实际上所有线程都不会响应被屏蔽的信号了。
2. 在某个线程中调用如下函数来等待信号并处理之：

```c
#include <signal.h>
/*
@param set: 需要等待的信号的集合。
@param sig: 用于存储该函数返回的信号值。
*/
int sigwait( const sigset_t* set, int* sig);
```

用一个线程处理所有信号

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); }while(0)

static void *sig_thread( void *arg )
{
    sigset_t *set = (sigset_t *) arg;
    int s, sig;
    for( ;; )
    {
        /*	第二个步骤，调用 sigwait等待信号	*/
        s = sigwait( set, &sig );
        if( s != 0 )
        	handle_error_en( s, "sigwait" );
        printf( "Signal handling thread got signal %d\n", sig );
    }
}
int main( int argc, char* argv[] )
{
    pthread_t thread;
    sigset_t set;
    int s;
    /*	第一个步骤，在主线程中设置信号掩码	*/
	sigemptyset( &set );
    sigaddset( &set, SIGQUIT );
    sigaddset( &set, SIGUSR1 );
    s = pthread_sigmask( SIG_BLOCK, &set, NULL );
    if( s != 0 )
        handle_error_en( s, "pthread_sigmask" );
    s = pthread_create( &thread, NULL, &sig_thread, (void *) &set );
    if( s != 0 )
        handle_error_en( s, "pthread_create" );
    pause();
}
```

`pthread`提供一种 将信号发送给指定线程 的方法：

```c
#include <signal.h>
/*
@param thread: 目标线程
@param sig: 待发送信号。sig为0，则不发送信号，但它仍然执行错误检查，即检查目标线程是否存在。
@return ：成功返回0，失败返回错误码。
*/
int pthread_kill(pthread_t thread, int sig)
```



# 进程池(线程池也类似)

![image-20230424153242240](./assets/image-20230424153242240.png)





![image-20230424153611733](./assets/image-20230424153611733.png)

如果客户任务存在上下文关系的，则最好一直用同一个子进程来为之服务，否则实现起来将比较麻烦，因为我们不得不在各子进程之间传递上下文数据。

`epoll`的`EPOLLONESHOT`事件，这一事件能够确保一个客户连接在整个生命周期中仅被一个线程处理。

## 半同步/半异步进程池的实现

![image-20230502160848410](assets/image-20230502160848410.png)

![image-20230424200329449](./assets/image-20230424200329449.png)

```cpp
// filename: processpool.h
#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>

/*		描述一个子进程的类，m_pid是目标子进程的PID，m_pipefd是父进程和子进程通信用的管道		*/
class process
{
    public:
    	process():m_pid(-1){}
    public:
    	pid_t m_pid;
    	int m_pipefd[2];
}
/*		进程池类，将它定义为模板类是为了代码复用。其模板参数是处理逻辑任务的类		*/
template< typename T >
class processpool
{
private:
    /*	将构造函数定义为私有的，因此我们只能通过后面的create静态函数来创建processpool实例	*/
    processpool( int listenfd, int process_number = 8 );
public:
    /*	单体模式，以保证程序最多创建一个processpool实例，这是程序正确处理信号的必要条件	*/
    static processpool< T >* create( int listenfd, int process_number = 8 )
    {
        if( !m_instance ){
            m_instance = new processpool< T >(listenfd, process_number );
        }
        return m_instance;
    }
    ~processpool()
    {
        delete [] m_sub_process;
    }
    /*	启动进程池	*/
    void run();

private:
    void setup_sig_pipe();
    void run_parent();
    void run_child();

private:
    /*	进程池允许的最大子进程数量	*/
	static const int MAX_PROCESS_NUMBER = 16;
    /*	每个子进程最多能处理的客户数量	*/
    static const int USER_PER_PROCESS = 65536;
    /*	epoll 最多能处理的时间数	*/
    static const int MAX_EVENT_NUMBER = 10000;
    /*	进程池中的进程总数	*/
    int m_process_number;
    /*	子进程在池中的序号，从0开始	*/
    int m_idx;
    /*	每个进程都有一个epoll内核时间表，用m_epollfd标识		*/
    int m_epollfd;
    /*	监听socket	*/
    int m_listenfd;
    /*	子进程通过m_stop来决定是否停止运行	*/
    int m_stop;
    /*	保存所有子进程的描述信息	*/
	process* m_sub_process;
    /*	进程池静态实例		*/
    static processpool< T >* m_instance;
}

template< typename T > 
processpool< T >* processpool< T >::m_instance = NULL;

/*	用于处理信号的管道，以实现统一事件源。后面称之为信号管道	*/
static int sig_pipefd[2];

static int setnonblocking( int fd )
{
	int old_option = fcntl( fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

static void addfd( int epollfd, int fd )
{
	epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

/*	从epollfd标识的epoll内核事件表中删除fd上的所有注册事件	*/
static void removefd( int epollfd, int fd )
{
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
    close( fd ); 
}

static void sig_handler( int sig )
{
    int save_errno = errno;
    int msg = sig;
    send( sig_pipefd[1], (char* )&msg, 1, 0 );
    errno = save_errno;
}

static void addsig( int sig, void( handler )( int ), bool restart = true )
{
    struct sigaction sa;
    memset( &sa, '\0', sizeof( sa ) );
    sa.sa_handler = handler;
    if( restart )
    {
        sa.sa_flags |= SA_RESTART;
    }
    sigfillset( &sa.sa_mask );
    assert( sigaction( sig, &sa, NULL ) != -1 );	//	注册要捕获信号的sig。若触发信号sig，交给sa处理
}

template< typename T >
processpool< T >::processpool( int listenfd, int process_number ):m_listenfd( listenfd ), m_process_number( process_number ), m_idx( -1 ), m_stop( false )
{
    assert( ( process_number > 0 ) && ( process_number <= MAX_PROCESS_NUMBER ) );
    m_sub_process = new process[ process_number ];
    assert( m_sub_process );
    
    /*	创建process_number个子进程，并建立它们和父进程之间的管道	*/
    for( int i=0; i<process_number; i++ )
    {
        int ret = sockpair( PF_UNIX, SOCK_STREAM, 0, m_sub_process[i].m_pipefd );
        assert( ret == 0 );
        
        m_sub_process[i].m_pid = fork();
        assert( m_sub_process[i].m_pid >= 0 );
        if( m_sub_process[i].m_pid > 0 )
        {
            close( m_sub_process[i].m_pipefd[1] );
            continue;
        }
        else
        {
            close( m_sub_process[i].m_pipefd[0] );
            m_idx = i;
            break;
        }
    }
}

/*	统一事件源	*/
template< typename T >
void processpool< T >::setup_sig_pipe()
{
	/*	创建epoll 事件监听表 和 信号管道	*/
    m_epollfd = epoll_create( 5 ); 
    assert( m_epollfd != -1 );
    
    int ret = sockpair( PF_UNIX, SOCK_STREAM, 0, sig_pipefd );
    assert( ret != -1 );
    
    setnonblocking( sig_pipefd[1] );
    addfd( m_epollfd, sig_pipefd[0] );
	/*	设置信号处理函数	*/
    addsig( SIGCHLD, sig_handler );
    addsig( SIGINT, sig_handler );
    addsig( SIGPIPE, SIG_IGN );
}

/*	父进程中m_idx值为-1，子进程中m_idx值大于等于0，我们据此判断接下来要运行的是父进程代码还是子进程代码	*/
template< typename T >
void processpool< T >::run()
{
    if( m_idx != -1 )
    {
        run_child();
        return;
    }
    run_parent();
}

template< typename T >
void processpool< T >::run_child()
{
    setup_sig_pipe();
    
    /*	每个子进程都通过其在进程池中的序号值m_idx找到与父进程通信的管道	*/
    int pipefd = m_sub_process[m_idx].m_pipefd[ 1 ];
    /*	子进程需要监听管道文件描述符pipefd,因为父进程将通过它来通知子进程accept新连接	*/
    addfd( m_epollfd, pipefd );
    
    epoll_event events[ MAX_EVENT_NUMBER ];
    T* users = new T [ USER_PER_PROCESS ];
    assert( users );
    int number = 0;
    int ret = -1;
    
    while( !m_stop )
    {
        number = epoll_wait( m_epollfd, events, MAX_EVENT_NUMBER, -1 );
        if((number < 0 ) && ( errno != EINTR ))
        {
            printf( "epoll failure\n" );
            break;
        }
        
        for( int i=0; i<number; i++)
        {
            int sockfd = events[i].data.fd;
            if( (sockfd == pipefd ) && ( events[i].events & EPOLLIN ) )
            {
                int client = 0;
                /*	从父、子进程之间的管道读取数据，并将结果保存在变量client中。如果读取成功，则表示有新客户连接到来	*/
                ret = recv( sockfd, ( char* )&client, sizeof( client ), 0 );
                if((( ret < 0 ) && (errno != EAGAIN ) ) || ret == 0 )
                {
                    continue;
                }
                else
                {
                    struct sockaddr_in client address;
                    socklen_t client_addrlength = sizeof( client_address );
                    int connfd = accept( m_listenfd, ( struct sockaddr* ) &client_address, &client_addrlength );
                    if( connfd < 0 )
                    {
                        printf( "errno is: %d\n", errno );
                        continue;
                    }
                    
                    addfd( m_epollfd, connfd );	//监听新连接

                    /*	模板类T必须实现init方法(在调用这个头文件的文件里实现)，以初始化一个客户连接。我们直接使用connfd来索引逻辑处理对象(T类型的对象)，以提高程序效率	*/
                    users[connfd].init( m_epollfd, connfd, client_address );
                }
            }
            /*	下面处理子进程接收到的信号	*/
            else if( ( sockfd == sig_pipefd[0] ) && ( events[i].events & EPOLLIN ) )
            {
                int sig;
                char signals[1024];
                ret = recv( sig_pipefd[0], signals, sizeof( signals ), 0 );
                if( ret <= 0 )
                {
                	continue;
                }
                else
                {
                    for( int i = 0; i < ret; i++){
                        switch( signals[i] )
                        {
                            case SIGCHLD:
                            {
                            	pid_t pid;
                                int stat;
                                while((pid = waitpid(-1, &stat, WNOHANG )) > 0 )
                                {
                                    continue;
                                }
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                        		m_stop = true;
                            	break;
                            }
                            default:
                            {
                            	break;        
                            }
                        }
                    }
                }
            }
            /*	如果是其他可读数据，那么必然是客户请求到来。调用逻辑处理对象的process方法处理之	*/
            else if( events[i].events & EPOLLIN )
            {
                users[sockfd].process();
            }
            else
            {
                continue;
            }
        }
    }
    
    delete [] users;
    users = NULL;
    close( pipefd );
    close( m_epollfd );
}

template< typename T >
void processpool< T >::run_parent()
{
    setup_sig_pipe();
    
    /*	父进程监听m_listenfd	*/
    addfd( m_epollfd, m_listenfd );
     
    epoll_event events[ MAX_EVENT_NUMBER ];
    int sub_process_counter = 0;
    int new_conn = 1;
    int number = 0;
    int ret = -1;
    
    while( !m_stop )
    {
        number = epoll_wait( m_epollfd, events, MAX_EVENT_NUMBER, -1 );
        if( ( number < 0 ) && ( errno != EINTR ) )
        {
            printf(	"epoll failure\n" );
            break;
        }
        for( int i=0; i<number; i++ )
        {
            int sockfd = events[i].data.fd;
            if( sockfd == m_listenfd )
            {
        /*	如果有新连接到来，就采用Round Robin方式将其分配给一个子进程处理	*/			int i = sub_process_counter;
                do{
                    if(m_sub_process[i].m_pid != -1 )
                    {
                        break;
                    }
                    i = ( i+1 ) % m_process_counter;
            	}while( i != sub_process_counter );
            
            
            
                if( m_sub_process[i].m_pid == -1 )
                {
                    m_stop = true;
                    break;
                }
                sub_process_counter = (i+1) % m_process_number;
                send( m_sub_process[i].m_pipefd[0], (char*)&new_conn, sizeof(new_conn), 0 );
                pritnf( "send request to child %d\n", i );
        	}
        /*	下面处理父进程接收到的信号	*/
            else if( ( sockfd == sig_pipefd[0] ) ) && ( events[i].events & EPOLLIN ) )
            {
                int sig;
                char signals[1024];
                ret = recv( sig_pipefd[0], signals, sizeof( signals ), 0 );
                if( ret <= 0 )
                {
                    continue;
                }
                else
                {
                    for( int i=0; i<ret; i++ )
                    {
                        switch( signals[i] )
                        {
                            case SIGCHLD:
                            {
                                pid_t pid;
                                int stat;
                                while( ( pid = waitpid(-1, &stat, WNOHANG) ) )//wait_pid非阻塞，如果任意子进程还没有结束，则也立即返回0，任意子进程正常退出，则返回该子进程PID。
                                {
                                    for(int i=0; i<m_process_number;i++)
                                    {
                                    /*	如果进程池中第i个子进程退出了，则主进程关闭相应的通信管道，并设置相应的m_pid为-1，以标记该子进程已经退出	*/
                                        if(m_sub_process[i].m_pid == pid)
                                        {
                                            printf("child %d join\n", i);
                                            close( m_sub_process[i].m_pipefd[0] );
                                            m_sub_process[i].m_pid = -1;
                                        }
                                    }
                                }
                                /*	如果所有子进程都已经退出了，则父进程也退出	*/
                                m_stop = true;
                                for(int i = 0; i < m_process_number; ++i)
                                {
                                    if( m_sub_process[i].m_pid != -1 )
                                    {
                                        m_stop = false;
                                    }
                                }
                                break;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                            /*	如果父进程收到终止信号，那么就杀死所有子进程，并等待它们全部结束。当然，通知子进程结束更好的方法是向父、子进程之间的通信管道发送特殊数据。	*/		
                                printf("kill all the child now\n");
                                for(int i=0; i<m_process_number;++i)
                                {
                                    int pid = m_sub_process[i].m_pid;
                                    if( pid != -1 )
                                    {
                                        kill( pid, SIGTERM );
                                    }
                                }
                                break;
                            }
                            default:
                            {
                                break;        
                            }
                        }
                    }
                }
            }
            else
        	{
            	continue; 
        	}
    	}
    }
    close(m_epollfd);
}
```

## 用进程池实现的简单CGI服务器

```cpp
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "processpool.h"	

class cgi_conn
{
public:
    cgi_conn(){}
    ~cgi_conn(){}
    /*	初始化客户连接，清空读缓冲区	*/
    void init( int epollfd, int sockfd, const sockaddr_in& client_addr )
    {
    	m_epollfd = epollfd;
    	m_sockfd = sockfd;
    	m_address = client_addr;
        memset( m_buf, '\0', BUFFER_SIZE );
        m_read_idx = 0;
    }
    
    void process()
    {
        int idx = 0;
        int ret = -1;
        /*	循环读取和分析客户数据	*/
        while( true )
        {
            idx = m_read_idx;
            ret = recv( m_sockfd, m_buf + idx, BUFFER_SIZE-1-idx, 0 );
            /*	如果读操作发生错误，则关闭客户连接。但如果是暂时无数据可读，则退出循环	*/
            if( ret < 0 )
            {
                if( errno != EAGAIN )
                {
                    removefd( m_epollfd, m_sockfd );
                }
                break;
            }
            /*	如果对方关闭连接，则服务器也关闭连接	*/
            else if( ret == 0 )
            {
                removefd( m_epollfd, m_sockfd );
                break;
            }
            else
            {
            	m_read_idx += ret;
                printf( " user content is: %s\n ", m_buf );
                for( ; idx < m_read_idx; ++idx )
                {
            		if( ( idx >= 1 ) && ( m_buf[idx-1] == '\r' ) && ( m_buf[idx] == '\n' ) )
                    {
                        break;
                    }
                }
                /*	如果没有遇到字符"\r\n",则需要读取更多客户数据	*/
                if( idx == m_read_idx )
                {
                    continue;
                }
                m_buf[idx-1] = '\0';
                
                char* file_name = m_buf;
                /*	判断客户需要要运行的CGI程序是否存在	*/
                if( access( file_name, F_OK ) = -1 )
                {
                    removefd( m_epollfd, m_sockfd );
                    break;
                }
                /*	创建子进程来执行CGI程序	*/
                ret = fork();
                if( ret == -1 )
                {
                    removefd( m_epollfd, m_sockfd );
                    break;
                }
                else if( ret > 0 )
                {
               		/* 在父进程中只需要关闭连接	*/
                    removefd( m_epollfd, m_sockfd );
                    break;
                }
                else
                {
                    /*	子进程将标准输出定向到m_sockfd,并执行CGI程序  */
                    close( STDOUT_FILENO );
                    dup( m_sockfd );
                    execl( m_buf, m_buf, 0 );
                    exit( 0 );
                }
            }
        }
    }
private:
    /*	读缓冲区的大小	*/
    static const int BUFFER_SIZE = 1024;
    static int m_epollfd;
    int m_sockfd;
    sockaddr_in m_address;
    char m_buf[BUFFER_SIZE];
    /*	标记读缓冲中已经读入的客户数据的最后一个字节的下一个位置	*/
    int m_read_idx;
};
int cgi_conn::m_epollfd = -1;

int main( int argc, char* argv[] )
{
    if( argc <= 2 )
    {
        printf("usage: %s ip_address port_number\n", basename( argv[0] ));
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi( argv[2] );
    
    int listenfd = socket( PF_INET, SOCK_STREAM, 0 );
    assert( listenfd >= 0 );
    
    int ret = 0;
    struct sockaddr_in address;
    bzero( &address, sizeof( address ));
    address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );
    
    ret = bind( listenfd, ( struct sockaddr* )&address, sizeof( address ) );
    assert( ret != -1 );
    ret = listen( listenfd, 5 );
    assert( ret != -1 );
    
    processpool< cgi_conn >* pool = processpool< cgi_conn >::create( listenfd );
    if( pool )
    {
        pool->run();
        delete pool;
    }
    close( listenfd );	// main函数创建了文件描述符listenfd, 那么就由它来亲自关闭
    return 0;
}


```



## 半同步/半反应堆模式

![image-20230502161312506](assets/image-20230502161312506.png)

```cpp
// filename: threadpool.h
#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <list>
#include <cstdio>
#include <exception>
#include <pthread.h>
/*	引用第 14 章介绍的线程同步机制的包装类	*/
#include "locker.h"

template< typename T >
class threadpool
{
public:
    /*	参数thread_number是线程池中线程的数量，max_requests是请求队列中最多允许的处理的请求的数量	*/
    threadpool( int thread_number = 8, int max_requests = 10000 );
    ~threadpool();
    /*	往请求队列中添加任务	*/
	bool append( T* request );

private:
    /*	工作线程运行的函数， 它不断从工作队列中取出任务并执行之	*/
    static void* worker( void arg );
    void run();
    
private:
    int m_thread_number;	/*	线程池中的线程数	*/
    int m_max_requests;		/*	请求队列中允许的最大请求数	*/
    pthread_t* m_threads;	/*	描述线程池的数组，其大小为m_thread_number	*/
    std::list< T* > m_workqueue;	/*	请求队列	*/
    locker m_queuelocker;	/*	保护请求队列的互斥锁	*/
    sem m_queuestat;	/*	是否有线程需要处理	*/
    bool m_stop;	/*	是否结束线程	*/
};

template< typename T >
threadpool< T >::threadpool( int thread_number, int max_requests ):m_thread_number( thread_number ),m_max_requests( max_requests ), m_stop( false ), m_threads( NULL )
{
	if(( thread_number <= 0 ) || ( max_requests <= 0 ) )
    {
        throw std::exception();
    }
    
    m_threads = new pthread_t[ m_thread_number ];
    if( !m_threads )
    {
        throw std::exception();
    }
    
    /*	创建thread_number个线程，并将它们都设置为脱离线程	*/
    for( int i=0; i<thread_number; i++ )
    {
        printf( " create the %dth thread\n ", i ); 
        if( pthread_create( m_threads + i, NULL, worker, this ) != 0 )
        {
            delete [] m_threads;
            throw std::exception;
        }
        if( pthread_detach( m_threads[i] ) )
        {
            delete [] m_threads;
            throw std::exception;
        }
    }
}

template< typename T >
threadpool< T >::~threadpool()
{
    delete [] m_threads;
    m_stop = true;
}

template< typename T >
bool threadpool< T >::append( T* request )
{
	/*	操作工作队列时一定要加锁，因为它被所有线程共享	*/
    m_queuelocker.lock();
    if( m_workqueue.size() > m_max_requests )
    {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back( request );
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template< typename T >
void* threadpool< T >::worker( void* arg )
{
    threadpool* pool = ( threadpool* )arg;
    pool->run();
    return pool;
}

template< typename T >
void threadpool< T >::run()
{
    while( !m_stop ){
        m_queuestat.wait();
        m_queuelocker.lock();
        if( m_workqueue.empty() )
        {
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();
        if( !request )
        {
            continue;
        }
        request->process();
    }
}
#endif
```



## 实现简单的Web服务器

```cpp
//filename: http_conn.h
#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include "locker.h"

class http_conn
{
public:
/*	文件名的最大长度	*/
static const int FILENAME_LEN = 200;
/*	读缓冲区的大小		*/
static const int READ_BUFFER_SIZE = 2048;
/*	写缓冲区的大小		*/
static const int WRITE_BUFFER_SIZE = 1024;
/*	HTTP的请求方法，但我们仅支持GET	*/
enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATCH	};
/*	解析客户请求时，主状态机所处的状态	*/
enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0,
                 	CHECK_STATE_HEADER,
                  	CHECK_STATE_CONTENT
                 };
/*	服务器处理HTTP请求的可能结果	*/	
enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, NO_RESOURCE,
               FORBIDDEN_REQUEST, FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };
/*	行的读取状态	*/	
enum LINE_STATUS { LINE_OK = 0, LINE_BAD }
    
public:
    http_conn(){};
    ~http_conn(){};
public:
	/*	初始化新接受的连接 */
    void init( int sockfd, const sockaddr_in& addr );
    /*	关闭连接	*/
    void close_conn( bool real_close = true );
    /*	处理客户请求	*/
    void process();
    /*	非阻塞读操作	*/
    bool read();
    /*	非阻塞写操作	*/
    bool write();
    
private:
    /*	初始化连接	*/
    void init();
    /*	解析HTTP请求	*/
	HTTP_CODE process_read();
    /*	填充HTTP应答	*/
    bool process_write( HTTP_CODE ret );
    /*	下面这一组函数被process_read调用以分析HTTP请求	*/
    HTTP_CODE parse_request_line( char* text );
    HTTP_CODE parse_headers( char* text );
    HTTP_CODE parse_content( char* text );
    HTTP_CODE do_request();
    char* get_line(){ return m_read_buf + m_start_line; };
    LINE_STATUS parse_line();
    
    /*	下面这一组函数被process_write调用以填充HTTP应答	*/
	void unmap();
	bool add_response( const char* format, ... );
	bool add_content( const char* content );
	bool add_status_line( int status, const char* title );
	bool add_headers( int content_length );
	bool add_content_length( int content_length );
	bool add_linger();
	bool add_blank_line();

public:
    /*	所有socket上的事件都被注册到同一个epoll内核事件表中，所以将epoll文件描述符设置为静态的	*/
    static int m_epollfd;
    /*	统计用户数量	*/
    static int m_user_count;

private:
    /*	该HTTP连接的socket和对方的socket地址	*/
    int m_sockfd;
    sockaddr_in m_address;
    
    /*	读缓冲区	*/
    char m_read_buf[ READ_BUFFER_SIZE ];
    /*	标识读缓冲中已经读入的客户数据的最后一个字节的下一个位置	*/
    int m_read_idx;
    /*	当前正在分析的字符在读缓冲区中的位置	*/
    int m_checked_idx;
    /*	当前正在解析的行的起始位置	*/
    int m_start_line;
    /*	写缓冲区	*/
    char m_write_buf[ WRITE_BUFFER-SIZE ];
    /*	写缓冲区中待发送的字节数	*/
    int m_write_idx;
    
    /*	主状态机当前所处的状态	*/
    CHECK_STATE m_check_state;
    /*	请求方法	*/
    METHOD m_method;
    
    /*	客户请求的目标文件的完整路径，其内容等于doc_root + m_url, doc_root是网站根目录	*/
    char m_real_file[ FILENAME_LEN ];
    /*	客户请求的目标文件的文件名	*/
    char* m_url;
    /*	HTTP协议版本号，我们仅支持HTTP/1.1	*/
    char* m_version;
    /*	主机名	*/
    char* m_host;
    /*	HTTP请求的消息体的长度	*/
    int m_content_length;
    /*	HTTP请求是否要求保持连接	*/
    bool m_linger;
    
    /*	客户请求的目标文件被mmap到内存中的起始位置	*/
    char* m_file_address;
    /*	目标文件的状态。通过它我们可以判断文件是否存在、是否为目录、是否可读、并获取文件大小等信息	*/
    struct stat m_file_stat;
    /*	我们将采用writev来执行写操作，所以定义下面两个成员，其中m_iv_count表示被写内存块的数量	*/
    struct iovec m_iv[2];
    int m_iv_count;
};
#endif
```

```cpp
//filename: http_conn.cpp
#include "http_conn.h"

/*	定义HTTP响应的一些状态信息	*/
const char* ok_200_title = "OK";
const char* error_400_title = "Bad Request";
const char* error_400_form = "Your request has bad syntax or is inherently impossible to satisfy.\n";
const char* error_403_title = "Forbidden";
const char* error_403_form = "You do not have permission to get file from this server.\n";
const char* error_404_title = "Not Found";
const char* error_404_form = "The requested file was not found on this server.\n";
const char* error_500_title = "Internal Error";
const char* error_500_form = "There was an unusual problem serving the requested file.\n";
/*	网站的根目录	*/
const char* doc_root = "/var/www/html";

int setnonblocking( int fd )
{
    int old_option = fcntl(fd, F_GETFL );
    int new_option = old_option | O_NONBLOCK;
    fcntl( fd, F_SETFL, new_option );
    return old_option;
}

void addfd( int epollfd, int fd, bool one_shot )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    if( one_shot )
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl( epollfd, EPOLL_CTL_ADD, fd, &event );
    setnonblocking( fd );
}

void removefd( int epollfd, int fd )
{
    epoll_ctl( epollfd, EPOLL_CTL_DEL, fd, 0 );
    close( fd );
}

void modfd( int epollfd, int fd, int ev )
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl( epollfd, EPOLL_CTL_MOD, fd, &event );
}

int http_conn::m_user_count = 0;
int http_conn::m_epollfd = -1;

void http_conn::init( int sockfd, const sockaddr_in& addr )
{
    m_sockfd = sockfd;
    m_address = addr;
    /*	如下两行是为了避免TIME_WAIT状态，仅用于调试，实际使用时应该去掉	*/
    int reuse = 1;
    setsockopt( m_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof( reuse ) );
    addfd( m_epollfd, sockfd, true );
    m_user_count++;
    
    init();
}

void http_conn::init()
{
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_linger = false;
    
    m_method = GET;
    m_url = 0;
    m_version = 0;
    m_content_length = 0;
    m_host = 0;
    m_start_line = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    memset( m_read_buf, '\0', READ_BUFFER_SIZE );
    memset( m_write_buf, '\0', WRITE_BUFFER_SIZE );
    memset( m_real_file, '\0', FILENAME_LEN );
}

/*	从状态机	*/
http_conn::LINE_STATUS http_conn::parse_line()
{
    char temp;
    for(;m_checked_idx < m_read_idx; ++m_checked_idx ){
        temp = m_read_buf[ m_checked_idx ];
        if( temp == '\r' )
        {
            if( ( m_checked_idx + 1 ) == m_read_idx )
            {
                return LINE_OPEN;
            }
            else if ( m_read_buf[ m_checked_idx + 1 ] == '\n' )
            {
                m_read_buf[ m_checked_idx++ ] = '\0';
                m_read_buf[ m_checked_idx++ ] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if( temp == '\n' )
        {
            if( ( m_checked_idx > 1 ) && ( m_read_buf[ m_checked_idx - 1 ] == '\r' ) )
            {
                m_read_buf[ m_checked_idx-1 ] = '\0';
                m_read_buf[ m_checked_idx++ ] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

/*	循环读客户数据，直到无数据可读或者对方关闭连接	*/
bool http_conn::read()
{
    if( m_read_idx >= READ_BUFFER_SIZE )
    {
        return false;
    }
    int bytes_read = 0;
    while( true )
    {
        bytes_read = recv( m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0 );
        if( bytes_read == -1 )
        {
            if( errno == EAGAIN || errno == EWOULDBLOCK )
            {
                break;
            }
            return false;
        }
        else if( bytes_read == 0 )
        {
            return false;
        }
        m_read_idx += bytes_read;
    }
    return true;
}

/*	解析HTTP请求行，获得请求方法、目标url,以及HTTP版本号	*/
http_conn::HTTP_CODE http_conn::parse_request_line( char* text )
{
    m_url = strpbrk( text, " \t" );
    if( !m_url )
    {
        return BAD_REQUEST;
    }
    *m_url++ = '\0';
    
    char* method = text;
    if( strcasecmp( method, "GET" ) == 0 )
    {
        m_method = GET;
    }
    else
    {
        return BAD_REQUEST;
    }
    m_url += strspn( m_url, " \t" );
    m_version = strpbrk( m_url," \t" );
    if( !m_version )
    {
        return BAD_REQUEST;
    }
    *m_version++ = '\0';
    m_version += strspn( m_version, " \t" );
    if( strcasecmp( m_version, "HTTP/1.1" ) != 0 )
    {
        return BAD_REQUEST;
    }
    if( strncasecmp( m_url, "http://", 7 ) == 0 )
    {
        m_url += 7;
        m_url = strchr( m_url, '/' );
    }
    if( !m_url || m_url[0] != '/' )
    {
        return BAD_REQUEST;
    }
    m_check_state = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

/*	解析HTTP请求的一个头部信息	*/
http_conn::HTTP_CODE http_conn::parse_headers( char* text )
{
    /*	遇到空行，表示头部字段解析完毕	*/
    if( text[0] == '\0' )
    {
        /*	如果HTTP请求有消息体，则还需要读取m_content_length字节的消息体，状态机转移到CHECK_STATE_CONTENT状态	*/
        if( m_content_length != 0 )
        {
            m_check_state = CHECK_STATE_CONTENT;
            return NO_REQUEST;
        }
        
        /*	否则说明我们已经得到了一个完整的HTTP请求	*/
        return GET_REQUEST;
    }
    /*	处理Connection头部字段	*/
    else if( strncasecmp( text, "Connection:", 11 ) == 0 )
    {
        text += 11;
        text += strspn( text, " \t" );
        if( strcasecmp( text, "keep-alive" ) == 0 )
        {
            m_linger = true;
        }
    }
    /*	处理Content-Length头部字段	*/
    else if( strncasecmp( text, "Content-Length:", 15 ) == 0 )
    {
        text += 15;
        text += strspn( text, " \t" );
        m_content_length = atol( text );
    }
    /*	处理Host头部字段	*/
	else if( strncasecmp( text, "Host:", 5 ) == 0 )
    {
        text += 5;
        text += strspn( text, " \t" );
        m_host = text;
    }
    else
    {
        printf("oop! unknow header %s\n", text );
    }
    return NO_REQUEST;
}

/*	我们没有真正解析HTTP请求的消息体，只是判断它是否被完整地读入了	*/
http_conn::HTTP_CODE http_conn::parse_content( char* text )
{
    if( m_read_idx >= ( m_content_length + m_checked_idx ) )
    {
        text[ m_content_length ] = '\0';
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

/*	主状态机。其分析请参考8.6节，这里不在赘述	*/
http_conn::HTTP_CODE http_conn::process_read()
{
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char* text = 0;
    
    while( ( ( m_check_state == CHECK_STATE_CONTENT ) && ( line_status == LINE_OK ) ) || 
         (( line_status = parse_line() ) == LINE_OK ))
    {
        text = get_line();
        m_start_line = m_checked_idx;
        printf( "got 1 http line: %s\n", text );
        
        switch( m_check_state )
        {
            case CHECK_STATE_REQUESTLINE:
            {
            	ret = parse_request_line( text );
                if( ret == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:
            {
            	ret = parse_headers( text );
                if( ret == BAD_REQUEST )
                {
                    return BAD_REQUEST;
                }
                else if( ret == GET_REQUEST )
                {
                    return do_request();
                }
                break;
            }
            case CHECK_STATE_CONTENT:
            {
            	ret = parse_content( text );
            	if( ret == GET_REQUEST )
                {
                    return do_request();
                }
                line_status = LINE_OPEN;
                break;
            }
            default:
            {
            	return INTERNAL_ERROR;        
            }
        }
    }
    return NO_REQUEST;
}
/*	当得到一个完整、正确的HTTP请求时，我们就分析目标文件的属性。如果目标文件存在、对所有用户可读，且不是目录，则使用mmap将其映射到内存地址m_file_address处。并告诉调用者获取文件成功	
*/
http_conn::HTTP_CODE http_conn::do_request()
{
    strcpy( m_real_file, doc_root );
    int len = strlen( doc_root );
    strncpy( m_real_file + len, m_url, FILENAME_LEN - len - 1 );
    if( stat( m_real_file, &m_file_stat ) < 0 )
    {
        return NO_RESOURCE;
    }
    if( !( m_file_stat.st_mode & S_IROTH ) )
    {
        return FORBIDDEN_REQUEST;
    }
    if( S_ISDIR( m_file_stat.st_mode ) )
    {
        return BAD_REQUEST;
    }
    
    int fd = open( m_real_file, O_RDONLY );
    m_file_address = ( char* )mmap( 0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0 );
    close( fd );
    return FILE_REQUEST;
}

/*	对内存映射区执行munmap操作	*/
void http_conn::unmap()
{
    if(m_file_address)
    {
        munmap( m_file_address, m_file_stat.st_size );
        m_file_address = 0;
    }
}

/*	写HTTP响应	*/
bool http_conn::write()
{
    int temp = 0;
    int bytes_have_send = 0;
    int bytes_to_send = m_write_idx;
    if( bytes_to_send == 0 )
    {
        modfd( m_epollfd, m_sockfd, EPOLLIN );
        init();
        return true;
    }
    
    while( 1 )
    {
        temp = writev( m_sockfd, m_iv, m_iv_count );
        if( temp <= -1 )
        {
            
        }
    }
}
```

