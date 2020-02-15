# Chapter 42 共享庫高級特性



## 42.1 動態加載庫

在 Runtime 載入 Library 藉由 dlopen API 做到，他起源自 Solaris ，後來成為 SUSv3 規範之一。

要在 Linux 使用 dlopen API 需要指定 ``-ldl`` ，這樣才能夠和 libdl 的 library link 起來。

### 42.1.1 打開 Library: dlopen()

```c
#include <dlfcn.h>

void *dlopen(const char *libfilename, int flags);
```

* 給定的第1個參數是該 Library 的名稱，架設其內容有包含一個斜線，則整個字串會以路徑來解釋。反之如同前章節的方式下去分析要加入的 Library。
* 執行成功時返回一個 Handler。
* 同一個 Library 可以打開多次，但是要關閉同樣的次數，其內部是使用一個 Counter 來紀錄有多少依賴次數，當 Counter 歸 0 時可能會釋放相關的資源

#### RTLD_LAZY

由於 Library 可能會互相相依，最後塞進來可能會變成一坨粽子。使用 LAZY 可以使其 Library 內的未定義函數 Symbol 被執行時才會解析他。這可以延後 Delay。

#### RTLD_NOW

在 ``dlopen`` 結束之前把所有 Symbol 都解析過，能夠立即發現任何未定義函數符號錯誤。測試時好用。

#### RTLD_GLOBAL

這個 Library 的 symbol 由 目前的其他 lib 的 Symbol 中進行查找，混在一起的概念

#### RTLD_LOCAL

和上者相反，每個  Library 和其 Dependencies Tree 的項目，彼此 symbol 不可見

#### RTLD_NODELETE

即使 Reference Counter 為 0 也不要移除他。這意味著其 Static Variable 也不會被更動

#### RTLD_NOLOAD

如果該目標不在記憶體內，不要加載他。假設目標已經在記憶體內，會直接回傳其 Handler，反之 NULL。這可以用無意義地提升其 Reference Counter。

#### RTLD_DEEPBIND

先搜尋自己(也許包含 Dependencies Tree)的 Symbols ，沒找到再嘗試搜尋目前已經加載的 Library 的 Symbol。可以做到類似 ``-Bsymbolic`` 的效果。此為 Linux 特有



### 42.1.2 錯誤診斷: dlerror()

```c
#include <dlfcn.h>

const char *dlerror(void)l 
```

假設從上次 ``dlerror()`` 呼叫到這次都沒有發生過任何錯誤，則回傳 NULL



### 42.1.3 取得 symbol address: dlsym()

```c
#include <dlfcn.h>

void *dlsym(void *handler, char *symbol);
```

如果找到 Symbol 會回傳其 Address，假設找不到則回傳 NULL。有可能遇到該 Symbol 的 Address 就是 0(NULL) 的情況，這個時候要透過 ``dlerror()`` 測試，如果回傳非 0 則這個 NULL 是真的找不到。

書本表示最標準的賦予和執行方法

```c
int (*funcp)(int);
*(void **) (&funcp) = dlsym(handle);
int result = (*funcp)(0);
```

除了 ``dlopen()`` 的 Handler 可以拿來用之外，還有提供下面這些有特殊功能的偽 Handler

#### RTLD_DEFAULT

從主程序中尋找該 Symbol，之後再查找以加載庫的 Symbol。類似於 Dynamic Linking 時尋找的規則

#### RTLD_NEXT

找到下一個同名的 Symbol 之 Address。

### 42.1.4 關閉: dlclose()

```c
#include <dlfcn.h>

int dlclose(void *handler);
```

> glibc 2.2.3 之後，你可以在 shared library 中使用 atexit() 來指定在 shared library 要從記憶體釋放前要調用的 function。

### 42.1.5 取得 Symbol 相關訊息: dladdr()

他本身沒有被 UNIX 實現提供

```c
#define _GNU_SOURCE
#include <dlfcn.h>

int dladdr(const void *addr, Dl_info *info);
```

### 42.1.6 訪問訊號

通常呼叫 Shared Library 內的 x(), 而他再呼叫 y(), 最後該 y() 應該是來自其他 library 的。有時候你會希望他從主程序這裡開始搜尋 y()。你可以給出 link 參數，這樣就可以讓 Dynamic Linker 參考 Global 主程序的Global Symbol

```shell
$ gcc -Wl,--export-dynamic main.c
$ gcc -export-dynamic main.c
```

## 42.2 控制訊號的可見性

一個優質的 Shared library 應該只公開其 ABI 的符號和變數，因為

* 作者不小心公開的 Interface ，他本來就沒打算分享給你們使用，未來他可以隨意的變動這些東西，這可能導致未來升級時的問題
* Shared library 導出時，他自己的 symbol 優先度比其他的 library 高
* 非必要的 symbol 會增加 dynamic symbol table 的大小

為避免這些問題，最好使用 static 宣告非公開的東西。

* 使用公開的 Symbol ，可能會產生 library 執行時插入其他不相關 function 的可能性。

GCC 提供一個特性，類似於 static

```c
void __attribute__ ((visibility("hidden"))) func(void) {
}
```

static 限制可見度於單一個檔案，而他可以使該 function 在構成自己的 library 的所有 source code 中可見，但是對於外部不可見。

* 透過 Version Script 你可以精確控制 Symbol 的可見性，且將 Reference 綁定到特定版本的符號

## 42.3 Linker  Version Script

版本腳本是一個包含 Linker ``ld`` 執行的指令文本。要使用版本腳本請指定  ``--version-script`` 。

```shell
$ gcc -Wl,--version-script,myscriptfile.map ...
```

### 42.3.1 使用 Version Script 控制 Symbol 的可見性

Version Script 要解決的一個問題是，某些 Symbol 可能會無意間變成 Global 可見的 Symbol，而 Version script 則對這個問題做妥當的處理。

```versionscript
VER_1 {
	global:
		vls_f1;
		vls_f2;
	local:
		*;
};
```

上面這個 Version Script ，會使 Global 只有 ``vls_f1`` 和 ``vls_f2`` ，其餘的都是 local symbol。

``VER_1`` 是他的版本編號，你可以選擇什麼編號都不寫，這樣他會當作他是匿名版本，且不容許其他版本的存在。

### 42.3.2 符號版本化

透過給 Symbol 上版本，你可以給同一個函數提供多個版本，每個程式將會使用他與 Shared Library 進行靜態連結時的版本。透過這個技巧，你可以在不提升主版本編號的情況下釋出不兼容的 library ，更極端的說法這貨可以完全取代 主/次 版本編號。

透過在 source code 裏面寫上一些特殊的 Linking Instruction 可以給函數編號版本，有 @@ 代表，在靜態連結時，預設要使用這個規則當作默認定義

```c
#include <stdio.h>

__asm__(".symver xyz_old,xyz@VER_1");
__asm__(".symver xyz_new,xyz@@VER_2");
```

而 Map 內，可以給標籤個相鄰關係，某方將會繼承對方的所有規則

```versionscript
VER_1 {
    global: xyz;
    local: *;
};

VER_2 {
    global: pqr;
} VER_1;
```

### 42.4 初始化和終止函數

你可以設定在 Library 載入和釋放時會執行的函數，這是透過 GCC 的 constructor 和 destructor 特性做到

```c
void __attribute__ ((constructor)) some_name_load(void){
    
}

void __attribute__ ((destructor)) some_name_unload(void){
    
}
```

#### _init() 和 _fini()

早期這些工作是透過 ``void _init(void)`` 和 ``void _fini(void)`` 辦到的，假設你想要使用這種方法來做到 constructor 和 destructor ，在編譯時要指定 ``gcc --nostartfiles`` 來避免 linker 塞入這些函數的 default implementation ，有需要的話也可以透過 linker 參數 ``-Wl,-init`` 和 ``-Wl,-fini`` 來自定義這些函數的名稱。

基本上不建議使用 ``_init`` 和 ``_fini`` ， GCC 的 feature 可以讓你定義多個 constructor 和 destructor 。

## 42.55 預加載 Shared Library

可以透過 ``LD_PRELOAD`` 環境參數，使某些 library 載入的優先權最高。也可以修改 ``/etc/ld.so.preload`` 內容來做到全域地改變。出於安全問題，Set-User-ID 和 Set-Group-ID 的程式將會忽略 ``LD_PRELOAD`` 

## 42.6 監控 Dynamic Linker: LD_DEBUG

可以透過 ``LD_DEBUG`` 環境變數來對 shared library 的載入做除錯

```shell
$ env LD_DEBUG=help date			# 檢視 Help 頁面
$ env LD_DEBUG=libs date			# 檢視載入的過程
```

這個 ``LD_DEBUG`` 對於 Dynamic Linker 偷偷塞入或是 ``dlopen`` 打開的 library 都有效，然後他會忽略 SUID SGID 的程式。





