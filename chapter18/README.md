# Chapter 18

## i node

i-node number from 2 instead of 1 or 0.
i-node 0 stands for available i-node(not used)
i-node 1 stands for bad blocks
i-node 2 always be the root of this file system

directory similar to a table.

i-node doesn't maintain file name, file name was record on the directory table. 

## link

### hard link

Hard link is just an entry in directory which points to specific i-node data.

Since it points to an i-node data, it cannot point to other filesystem

### symbolic link

Symbolic link is a special type of file, the data it store is a path to specific file

Since it store a path, it can point to anyway even another file system.

Kernel will extract symbolic link file at most N(check standard) time.
It the extraction is exceed its limitation, the extract failed.
This prevent kernel from stack overflow and other issue.

Function will always extract symbolic link if symbolic link points to a directory.
But if the symbolic link ponits to a file, the behavior depend on the function (e.g. rmdir, lchown, lstat, lsetxattr ...).

## chroot

chroot change process's root to specific directory.

chroot can be used to setup chroot jail, any process under the jail to force them under specific sub-tree.

    Some of UNIX implementation allow hard link on directory, if there is a directory hard link points to outside the jail.
    This is possible for process to escape the jail.

Most of the time not much program can run under the jail since they require shared library for linking.
If really want to run a program under the jail.
You can compile the as static link program or provide shared library under the jail.

chroot never be a safe jail mechanism.

* Better change working directory to target directory before chroot.
* If process has a directory file descriptor point to outside the jail, fchdir() can escape the jail.
* non-privilege user can receive file descriptor from other process via UNIX socket, that is one way to escape.

## Exercise

### Exercise 1

```shell
$ cc -o program code.c

$ ls -i program
5410459 program

$ ./program &
$ cc -o program code.c

$ ls -i program
5410460 program
```

``cc`` unlink  ``program`` file from the directory, and ``cc`` create another inode entry with the same name ``program``.
The file ``program`` didn't open the executeable file and rewrite it.
So there is no such problem.

### Exercise 2

```c
symlink("myfile", "../mylink");
```

According to man page : 

    > symlink() creates a symbolic link named linkpath which contains the string target.

So that will be creat a symbolic link file at ../mylink, and its content is myfile. 
Example: /path/whatever/mylink -> /path/whatever/myfile

The code confuse people because we thought first argument is evaluated from working directory.

### Exercise 3

Implement realpath()

[p3.c](p3.c)

### Exercise 4

I won't do this exercise since readdir\_r() already been deprecated.

### Exercise 5

Implement getcwd()

[p5.c](p5.c)

### Exercise 6

[p6.c](p6.c)

### Exercise 7

[p7.c](p7.c)

### Exercise 8

Implement nftw()

[p8.c](p8.c)

### Exercise 9

[p9.c](p9.c)

$ time ./a.out 10000000 fchdir / .

greyparrot@weed:~/Programming/TLPI/chapter18$ time ./a.out 10000000 fchdir / 

real	0m2.160s
user	0m0.561s
sys	    0m1.599s
greyparrot@weed:~/Programming/TLPI/chapter18$ time ./a.out 10000000 chdir / 

real	0m7.387s
user	0m0.761s
sys	    0m6.625s

I guess chdir have to eval the whole string, and checking permission and linking problem balabala(Can be solve by kernel buffer cache)
Also chdir have to copy whole path string from userspace to kernelspace so it will be slower
