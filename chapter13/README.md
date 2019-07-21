# Chapter 13

## Buffer

Every time we call write() and read(), usually it won't actually visit your storage device directly.
Instead, kernel read and write your data between your buffer(userspace) and kernel buffer cache(kernelspace).

There is a lot factors which affect the speed of storage access, file system and your buffer size might affect speed.

## keyword

* Vanilla kernel : unpacted mainline kernel, unlike most of the distribution. 

## Synchronized IO

SUSv3 Defined term "Synchronized" as: A specific I/O operation has been perform(on storage device), or it is failed.

Two kind of Synchronized I/O was defined in SUSv3

### Synchronized I/O Data Integrity Completion

### Synchronized I/O File Integrity Completion

Flush every update to storage device even though it is not necessary.


## Exercise

### Exercise 3

```c 
fflush(fp);         // flush stdio buffer to kernel buffer cache.
fsync(fileno(fd))   // flush kernel buffer cache to storage device.
```

### Exercise 4

stdio function has its own buffer, 

when we output to terminal, it will be line buffered(flush when newline encountered)
when we output to file, it will be fully buffered(flush when buffer is full)

In contrast, write system call just write the data to kernel buffer cache directly.

to fix the problem, we can make stdio no buffered

```c 
setvbuf(stdout, NULL, _IONBF, 0);
```
