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
协议族		地址族		描述			    地址值含义和长度
PF_UNIX    	AF_UNIX	    UNIX本地域协议族    文件的路径名
PF_INET    	AF_INET	    TCP/IPv4协议族    16bit端口号和32bitIPv4地址
PF_INET6    AF_INET6	TCP/IPv6协议族    16bit端口号和32bit流标识，
                                        128bitIPv6地址，32bit范围ID
*/


/*TCP/IP协议族有 sockaddr_in 和 sockaddr_in6 两个专用socket地址结构体，
分别用于IPv4和IPv6*/
struct sockaddr_in
{
	sa_family_t sin_family;	// 地址簇: AF_INET
	u_int16_t sin_port;		// 端口号
	struct in_addr sin_addr;	// IPv4地址结构体
};
struct in_addr
{
	u_int32_t s_addr;	//IPv4地址
};
struct sockaddr_in6
{
	sa_family_t sin6_family;	// 地址族: AF_INET6
	u_int16_t sin6_port;		//
	u_int32_t sin6_flowinfo;	//
	struct in6_addr sin6_addr;	//
	u_int32_t sin_scope_id;	//
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
	int port = atoi(argv[2]);	// atoi:把字符串转化为整形
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
        printf("usage: %s ip_address port_number\n", basename(argv[0]);
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    
    /* 创建一个IPv4 socket地址 */
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = hton(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert( sock >= 0);
    
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret != -1);
    
    ret = listen(sock, 5);
    assert(ret != -1);

    /* 暂停20秒以等待客户端连接和相关操作（掉线或者退出）完成 */
    sleep(20);
    struct sockaddr_in client;
    int connfd = accept(sock, (struct sockaddr* )&client, 
                &client_addrlength);
    if( connfd < 0){
        printf("errno is %d\n",errno);
    }else{
        /* 接受连接成功则打印出客户端的IP地址和端口号 */
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip: %s and port: %d\n",
        inet_notp(AF_INET,&client.sin_addr,remote,INET_ADDRSTRLEN),
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

int main(int argc, char* argv[]){
    if(argc <= 2){
        printf("usage: %s ip_address port_number\n",
        basename((argv[0]);
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons( port );
    
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock,(struct sockaddr*))&address, sizeof(address));
    assert( ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(soc)

}
```












