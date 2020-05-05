# Chapter 56 - Socket 介紹

Sockets 允許相同或不同 Host 上的應用程式互相交換資訊

後續將提到 Socket 的用法

## 56.1 概述

Application 使用 Socket 的步驟如下

1. 建立 Socket，他是一個允許通訊的 Device，兩個 Application 都會需要用到他
2. Server 將自己的 Socket 綁定到一個眾所皆知的地址(名稱)， Client 可以定位到那個位置

Socket() 系統調用將會建立一個 Socket ，且回傳一個後續用來通訊用的 File Descriptor

### Communication Domain

Socket 將存在於一個 Communication Domain ，有點類似於他通訊的空間。他將會決定

* Socket 地址的格式
* 通訊的範圍(Remote or Local)

絕大多數的系統至少支援這些 Communicatoin Domain

* UNIX Domain ``(AF_UNIX)``

  允許在同一個 Host 上的 Application 互相溝通(POSIX 規定 ``AF_LOCAL`` 和 ``AF_UNIX`` 同義)

* IPv4 Domain ``(AF_INET)`` 

  使用 IPv4 網路協議連接的應用程序

* IPv6 Domain ``(AF_INET6)``

  使用 IPv6 網路協議連接的應用程序

在程式碼中可能偶爾會看到 ``PF_UNIX`` 而非 ``AF_UNIX`` 這類的命名，其意思分別為

* PF, Protoocl Family
* AF, Address Family

Socket 在設計時，設計的人認為單一個 Protocol Family 可以支援多個 Address Family，但是實際上沒這回事，目前大多數的實現，都讓 Protocol Family 變成 Address Family 的別名，而在 SUSv3 中只有規定 ``AF_`` 沒有 ``PF_`` 。使用時用 AF 就好。

| Domain   | 通訊執行於 | 通訊對象            | 地址格式               | 地址結構     |
| -------- | ---------- | ------------------- | ---------------------- | ------------ |
| AF_UNIX  | Kernel 內  | 同主機              | 路徑名                 | sockaddr_un  |
| AF_INET  | 透過 IPv4  | 用 IPv4 連結的 Host | IPv4 地址 + 16Bit Port | sockaddr_in  |
| AF_INET6 | 透過 IPv6  | 用 IPv4 連結的 Host | IPv6 地址 + 16Bit Port | sockaddr_in6 |

### Socket 類型

大多 Socket 實現至少提供兩種型態的 Socket： Stream 和 Datagram 。

| 屬性                 | Socket(Stream) | Socket(Datagram) |      |      |
| -------------------- | -------------- | ---------------- | ---- | ---- |
| 傳輸可靠             | Yes            | No               |      |      |
| 傳輸資料有邊界       | No             | Yes              |      |      |
| Connection-Oriented? | Yes            | No               |      |      |

* Stream Socket ``(SOCK_STREAM)``
  * 可靠
  * 雙向連接
  * Byte Stream
  * 使用 TCP
* Datagram Socket ``(SOCK_DGRAM)``
  * 訊息邊界會保留
  * 傳輸不可靠
  * 訊息到達沒有順序，可能重複到達，甚至無法到達
  * 使用 UDP

### Socket System call

* ``socket()`` 建立新 Socket
* ``bind()`` 將 Socket 綁定到地址上。通常這是 Server 用來將 Socket 綁到某各大家都知道的地址上， Client 可以來戳他
* ``listen()`` Stream Socket 用來接受其他 socket 的連線
* ``accept()`` 在 listening Stream Socket 上，用來接受一對等應用程序的連線
* ``connect()`` 與另外一個 Socket 進行連線

## 56.2 建立一個 Socket: socket()

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```

* Domain 指定 Communicatoin Domain
* Type 指定 Socket 型態，通常是 ``SOCK_STREAM`` 或是 ``SOCK_DGRAM`` 
* Protocol ，指定特殊型態的 Socket，如果沒有就 0，以 裸 Socket(SOCK_RAW) 來說，會將 Protocol 指定為 ``IPPROTO_RAW`` 。

>  Type 可以用 OR 搭配 ``SOCK_CLOEXEC`` 或是 ``SOCK_NONBLOCK`` 

## 56.3 將 Socket 綁定到地址: bind()

```c
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

* addr 指向地址的 struct

> 可以不進行 ``bind()`` 直接 ``listen()`` ，這時 Kernel 會自動為 Socket 選擇一個臨時端口，後續可以使用 ``getsockname()`` 來得知位置

## 56.4 通用 socket 地址結構: struct sockaddr

由於 ``bind()`` 要支援各種型式的地址，所以他設計得比較複雜， Socket API 有特別定義一種通用的地址結構 ``sockaddr`` 。他的唯一用途為將各種 Domain 的特定地址結構轉換為單個類型，供後續參數使用，其內容大致上多定義為

```c
struct sockaddr {
    sa_damily_t sa_family;
    char sa_data[14];
};
```

## 56.5 Stream Socket

* Socket() 系統調用會建立一個 socket。
* Socket 通訊前要先將 Socket 連接到另外一端
  * 應用程式透過 ``bind()`` 將 Socket 綁定到某個大家都知道的位置上(IP, Domain)
  * 伺服器透過 ``listen()`` 告訴 Kernel ，Socket 接受連線接入
  * 其他 Client 應用透過 ``connect()`` 連入，此時需指定對方的 Socket 地址
  * Server 透過 ``accept()`` 來接受連線，預設 ``accept()`` 是堵塞的


### 主動與被動

Socket 的世界，連線有分主動與被動

* Server 通常是被動
* Client 通常是主動

### 監聽 Connection: listen()

```c
#include <sys/socket.h>

int listen(int sockfd,  int backlog);
```

* 當呼叫 ``listen()`` 時， Kernle 會將 sockfd 標記為被動
* 無法對一個已經連接的 socket 進行 ``listen()``
* 在呼叫 ``listen()`` 之後， Socket 就開始接受連線了
  * 連線可能在 ``accept()`` 之前進來，這時對應 Client 在收到確認之前都會一直 Block 住
  * backlog 這個參數用於標記， Kernel 能夠接住多少個等待 ``accept`` 的連線，超出的部份會直接拒絕
  * 在 ``<sys/socket.h>`` 中的 ``SOMAXCONN`` 這個常數定義其最大值，也可以透過 ``/proc/sys/net/core/somaxconn`` 來調整最大上線

### 接受 Connection: accept()

```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

* ``accept()`` 建立一個新的 sockfd ，這個 Socket 和 Client 呼叫 ``connect()`` 的 Socket 是彼此一對的

* 返回的 ``struct sockaddr *addr`` 會是對等連線 Socket 的連線資訊，包含其地址等

  * 如果不想知道的話，可以將 addr 和 addrlen 設定為 NULL 和 0
  * 後許可以使用 ``getpeername()`` 撈回相關資訊

  > Linux 支援 ``accept4()`` ，他多接受一個 Flag，可以用來指定新建立的 Socket 為 ``SOCK_CLOEXEC`` 或 ``SOCK_NONBLOCK`` 。

### 連線到對等 socket: connect()

```c
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

這個呼叫會將 Socket 設定為主動，連接的對應地址會放在 ``addr`` 和 ``addrlen`` 。

假設 Socket 的連線失敗，且希望重新進行連接， SUSv3 給定的可移植方法是關閉 Socket 後重開一個新的

### Stream Socket I/O

Socket 的兩個端點是雙向通訊的通道。Kernel 中間會設定 Buffer 儲存資料


* I/O 透過 ``read()`` 和 ``write()`` 。
* Socket 可以使用 ``close()`` 關閉 Socket。
  * 對關閉的 Socket 讀取資料會收到 EOF。
  * 對關閉的 Socket 寫入資料會收到 ``SIGPIPE`` ，且 Systemcall 會回傳 EPIPE 錯誤。常見的處理情境為忽略 ``SIGPIPE`` 且透過 ``EPIPE`` 錯誤找出被關閉的連線

### 離線中止: close()

* ``close()`` 會關閉對應的 Socket
* 假設一個 FD 參照到多個 Socket，那麼當所有 FDs 都關閉後，該 Socket 才會關閉
* 後續會提到 ``shutdown()`` systemcall ，可以用來提供更精確

## 56.6 Datagram Socket

 Socket Datagram 的運作類似於郵政系統

1. Socket Systemcall 等於建立一個郵箱
2. 所有發送和接收的應用程序都要透過 ``socket()`` 建立一個 Datagram Socket
3. 欲接收方必須要 ``bind()`` 其 Socket 到一個眾所階知的地方
4. 發送透過 ``sendto()``
5. 接收透過 ``recvfrom()`` 
6. 不使用時要 ``close()``


### 交換 Datagram: recvfrom() 和 sendto()

```c
#include <sys/socket.h>

ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags,
                const sockaddr *src_addr, socklen_t *addrlen);
ssize_t sendto(int sockfd, const void *buffer, size_t length, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen);
```

* ``recvfrom`` 接收資料時，假設開的 buffer 不夠大會直接裁斷
  * 使用 ``recvmsg()`` ，其將會返回一個 ``msghdr`` struct，可以透過檢查有沒有 ``MSG_TRUNC`` 來得知資料有沒有裁斷
* 假設不關係對方的傳送位置，可以將 ``src_addr`` 和 ``addrlen`` 留 NULL 和 0 ，這等同於使用 ``recv`` 或 flags 為 0 的 ``read`` 。

### 在 Datagram Socket 上使用 Connect()

* 儘管 Datagram Socket 是無連接的，還是會需要呼叫 ``connect()`` ，這會使 Kernel 紀錄該 Socket 對應的對等 Socket 位置。
  * 已連接的 Datagram Socket -> 有呼叫 ``connect()``
  * 未連接的 Datagram Socket -> 沒呼叫 ``connect()``
* 已經連接的 Datagra Socket 可透過重新呼叫 ``connect()`` 來更改對等 Socket 的位置
* 如果要明確地解除對等關係，可以透過使用指定地址的 ``sa_family`` 為 ``AF_UNSPEC`` 做到
* 設定對等 Socket 有一個好處
  * 後續的寫入可以使用 ``write()`` 做到，可以省去給定 ``dest_addr`` 和 ``addrlen`` 的麻煩，等於得到更簡易的 IO 調用。
  * 某些 TCP/IP 的實作中，使 Datagram Socket 設定對等 Socket 可以得到效能提升
