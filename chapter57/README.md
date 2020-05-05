# Chapter 57 

# Socket: Unix Domain

* 這裡提到在同一個 System 上， 使用 UNIX Domain Socket 做到 Host Processes 互相溝通的方法
* 如何透過文件權限控制來限制對 UNIX Domain Socket 的訪問
* 使用 ``socketpair()`` 建立一對互相連接的 UNIX Domain Socket
* Linux abstract socket namespace

## 57.1 UNIX Domain Socket 地址: struct sockaddr_un

UNIX Domain 中， Socket 地址以路徑名稱表示

```c
struct sockaddr_un {
    sa_family_t sun_family;
    char sun_path[108];
}
```

* ``sun`` 意指 ``Socket Unix``， 和 Sun Microsystem 無關

```c
const char *SOCKNAME = "/tmp/mysock";
struct sockaddr_un addr;

int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
if(sockfd == -1)
    errExit("socket");

memset(&addr, 0, sizeof(struct sockaddr_un));
addr.sun_family = AF_UNIX;
strncpy(addr.sun_path, SOCKNAME, sizeof(addr.sun_path) - 1);

if(bind(sockfd, (struct sockaddr*) &addr), sizeof(struct sockaddr_un) == -1)
    errExit("bind")
```

* 當對 UNIX Domain Socket 進行 ``bind()`` 時，會在系統內建立一個條目(該目錄必須要可訪問和寫入)
* 透過 ``stat()`` 檢視該目錄，會發現其返回值為 ``S_IFSOCK`` 。
* UNIX Domain Socket 以路徑名稱來標示，但是實際讀寫過程不會觸碰到底部的儲存設備
* 注意
  * 無法將 Socket 綁定到一個既有的路徑名稱上
  * 一般都是使用固定路徑名稱
  * 一個 Socket 只能綁到一個路徑名稱
  * 一個路徑名稱只能被一個 Socket 綁定
  * 無法透過 ``open()`` 打開 Socket
  * 當不需要某個 Socket 時，可以使用 ``unlink()`` 刪除其路徑名稱條目

由於 UNIX Domain Socket 需要在 Filesystem 內有一個名子，所以需要找一個地方放，放在 ``/tmp`` 很方便但是非常不安全，只要在裏面塞一個同名的檔案就可以做到拒絕服務攻擊，建議是放在一個被控管，只有特定權限可以塞的資料夾。

## 57.2 UNIX Domain 中的 Stream Socket

資料夾內有簡易的 UNIX Domain Stream Socket 範例  中的 Datagram Socket

## 57.3 UNIX Domain 的 Datagram Socket

看書

## 57.4 UNIX Domain socket 權限

Socket file 的 Ownership 決定了哪些 Process 可以跟這個 Socket 進行通訊

* 要 connect 一個 UNIX Domain Stream Socket 需要在該 socket 的寫入權限
* 要送資料到 UNIX Domain Datagram Socket 需要擁有該 socket 的寫入權限
* 有前往該 socket 的所有資料夾的 execute 權限

預設情況下會給予所有的權限，可以透過呼叫 ``bind()`` 之前修改 ``umask`` 來改變產生的 socket 權限

## 57.5 建立互連的 Socket Pair： socketpair()

``socketpair()`` 可以讓你建立一對連接的 UNIX Domain Socket，有時候這招會非常有用，比如後續會 fork 一個 Subprocess，可以透過這方式建立互通的管道

```c
#include <sys/socket.h>

int socketpair(int domain, int type, int protocol, int sockfd[2]);
```

* 在 BSD 中 ``pipe()`` 被實作為 ``socketpair()`` 
* 不需要指定 Filesystem 內的名稱
* 由於沒有名稱，他對於其他 Process 是匿名看不見的
* 只能作用再 UNIX Domain
* protocol 為 0
* Type 可以是 ``SOCK_DGRAM`` 或 ``SOCK_STREAM``
  * 可以搭配 ``SOCK_CLOEXEC`` 或 ``SOCK_NONBLOCK`` 

## 57.6 Linux 抽象 Socket 名空間

Abstract Namespace 是 Linux 的一項特性，他可以使 UNIX Domain Socket 建立一個名稱，但是不會出現在該 Filesystem 內

* 無需擔心和 Filesystem 的檔案名稱衝突(但可能在 Abstract Namespace 衝突)
* 沒有必要刪除該 socket 路徑名稱，socket 被關閉後會自動刪除這個抽象名稱
* 不需要建立 Filesystem Name，這對於 Chroot 環境或是在不俱備 filesystem 的 write 權限時有用

只要將 address 的 file 名稱第1個字指派為 ``\0`` 就可以做到，比如 ``\0foo`` 。這種 Socket 只在 Linux 支援，他不會建立任何檔案。

## 57.7 總結

