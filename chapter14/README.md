# Chapter 14

## VFS 虛擬文件系統

Virtual filesystem defined an universal interface for file management. 
Every program will use the interface for accessing file.
And filesystem have to provide implementation for these specific interface.

So program won't need to care about the underlying filesystem detail. Cool.

## Journaling 日誌

Once your system crash, this might cause file corruption.
Before we fix the problem we cannot use the filesystem, otherwise might blow the whole fs.
There is two thing we can do

1. consistency check (fsck) 
2. Journaling

### consistency check

Check the whole filesystem, looking for inconsistency corruption.
This will take time. 

### Journaling

When the system is running, before update the storage system. 
Journaling system will record these operation at journal file.

Once the system crashed. We can recovery(or at least fix) the corruption.
The disadvantage of journal is it increase the time for update file.

Some of the journaling file system only guarantee the consistency of metadata.
Ext3 and Ext4 support consistency for data but it migth decrease the performance of I/O.

## Mount namespace

Each Process could have their own mount namespace.
So every process's system might looks different.

* /proc/mounts shows process's mounting devices
* /proc/PID/mounts shows specific process's mounting devices
* mount and unomunt command maintain /etc/mtab file
* /etc/fstab 

## Exercise

| File system | 100 (random) | 100 (sequential) | 10000 (random) | 10000 (sequential) |
| ---         | ---          | ---              | ---            | ---                |
| ext2        | 0.39         | 0.29             | 1.79           | 1.79               |
| ext3        | 0.34         | 0.44             | 2.01           | 2.18               |
| ext4        | 0.36         | 0.42             | 2.34           | 2.16               |
| xfs         | 0.32         | 0.35             | 6.76           | 8.87               |
| vfat        | 0.26         | 0.48             | 36.19          | 45.90              |

```shell

$ sudo sh p1.sh 100 mount
Creat 100 files
ext2
Total Time   0.39
ext3
Total Time   0.34
ext4
Total Time   0.36
xfs
Total Time   0.32
vfat
Total Time   0.26


$ sudo sh p1.sh 10000 mount
Creat 10000 files
ext2
Total Time   1.79
ext3
Total Time   2.01
ext4
Total Time   2.34
xfs
Total Time   6.76
vfat
Total Time  36.19

# filename was ordered

Creat 100 files
ext2
Total Time   0.29
ext3
Total Time   0.44
ext4
Total Time   0.42
xfs
Total Time   0.35
vfat
Total Time   0.48

Creat 10000 files
ext2
Total Time   1.79
ext3
Total Time   2.18
ext4
Total Time   2.16
xfs
Total Time   8.87
vfat
Total Time  45.90

```
