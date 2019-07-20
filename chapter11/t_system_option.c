/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_system_option.c
*
* Author:           garyparrot
* Created:          2019/07/20
* Description:      System Option
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// SUSv3 除了規定系統資源的限制外，也給出了可實現的選項，如 POSIX共享內存, 任務控制
// POSIX Threading, Real time Signal 等等。
//
// 在 unistd.h 中會定義相關的 marco 常量，可以透過測試其值來知道目前的環境是否支援該選項
//
// 沒定義或是-1 不支援
//            0 也許支援, 透過 sysconf 或 pathconf 檢查
//           >0 支援，且有定義該選項相關的 header file, struct, function, 行為符合標準。 
//              其實際值會對應到支援的標準版本(200112L 為 SUSv3)

int main(int argc, const char *argv[]){

#if _POSIX_ASYNCHRONOUS_IO
    printf("%s\n", (_POSIX_ASYNCHRONOUS_IO > 0) ? "Support asynchronous IO." : "Doesn't support asynchronous IO.");
#else
    printf("Doesn't support asynchronous IO.\n");
#endif

#if _POSIX_SHARED_MEMORY_OBJECTS
    printf("%s\n", (_POSIX_SHARED_MEMORY_OBJECTS> 0) ? "Support shared memory obj." : "Doesn't support shared memory obj.");
#else
    printf("Doesn't support shared memory obj.\n");
#endif

#if _POSIX_MEMLOCK
    printf("%s\n", (_POSIX_MEMLOCK > 0) ? "Support memory lock." : "Doesn't support memory lock.");
#else
    printf("Doesn't support memory lock.\n");
#endif
    
    return 0;
}
