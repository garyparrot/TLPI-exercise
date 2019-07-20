# Chapter 12

## Content of /proc/PID

| File    | Description                                                                             |
| ---     | ---                                                                                     |
| cmdline | command line arguments split by \0.                                                     |
| cwd     | A symbolic link to current working directory.                                           |
| Environ | NAME=value environment variable table split by \0.                                      |
| exe     | A symbolic link to executable.                                                          |
| fd      | A directory. contains symbolic link to all the file opening by process.                 |
| maps    | Mapping of memory.                                                                      |
| mem     | Process's virtual memory.                                                               |
| mounts  | This file lists all the filesystems currently mounted in the process's mount namespace. |
| root    | Symbolic link to root directory.                                                        |
| status  | A lot of information.                                                                   |
| task    | Thread's subdirectory.                                                                  |

balabala better check ``man 5 proc``

## /proc Sub-directory

| Directory        | Description                             |
| ---              | ---                                     |
| /proc            | System information                      |
| /proc/net        | Internet status, Socket status j        |
| /proc/sys/fs     | Kernel variables related to filesystems |
| /proc/sys/kernel | Kernel setting                          |
| /proc/sys/net    | Internet kernel setting                 |
| /proc/sys/vm     | Files for memory management tuning      |
| /proc/sysvipc    | Stuff about System V IPC                |


