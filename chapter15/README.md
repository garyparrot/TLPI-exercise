# Chapter 15

## File ownership

Once a file get created, its owner uid inherited from the process's effective id(more precisely, fsid)

There is two way to decide the file's group id

1. System V's default behavior, gid decided by process's effective group id
2. BSD's default behavior, gid decided by parent dir's group id

There is the behavior for ext2 system 

| Situation                                                                           | Behavior       |
| ---                                                                                 | ---            |
| fs mount with -o grpid or -o bsdgroups                                              | parent dir gid |
| fs mount with -o nogrpid or -o sysvgroups (default), and parent dir's SGID is clear | process's gid  |
| fs mount with -o nogrpid or -o sysvgroups (default), and parent dir's SGID is set   | parent dir gid |

Linux with default behavior.
If we want group id inherited from the parent dir.
Set SGID bit for parent directory will do the trick.

WARNING : directory with SGID will makes all its sub-directories have SGID setted by default.

If we set SGID for one directory, all the files under that directory will inherited parent dir gid from it.

WARNING : when situation 1 or 3 satisfied, sub-directory will inherited SGID expect the file(directory) creator didn't match parent directory's group. this prevent bad user try to create a SGID bad program.

## Chown

Chown will reset set-user-id and set-group-id for file.
Chown won't reset set-group-id for directory.

## Permission of directory

Directory just like a list

r: read the content of that list ( browse file under the directory )
w: add or remove item from the list ( you don't need the write permission of that file )
x: visit the item in the list, see what is inside that item ( actually open / interact with the file )

Add/ Remove file, you need both writing and executing permission of that directory.
w for write new entry into the list
x for visit the list

Open file, you need at least executing permission.
x for interact with files.
Without reading permission, you cannot read the content of the list. So You don't know that's inside it.
but if you somehow know there its file name, you can actually open it.

## opened file

Once a file is opened, kernel won't check the permission form the later on write/read operation.
So if the permission of a file was changed after the file being open by a program.
The program still can interact with it even though he didn't have permission.

## Exception for privilege user

For Linux, privilege user cannot execute file without any executing permission setted.
That is, if there is a file with rw-rw-rw- permission, privilege user cannot execute it since nobody can execute it.

For some Unix system this is not true, privilege user can do whatever they what.

## Sticky Bit

Directory with sticky bit, user can delete or rename a file(dir) if and only if 
(1) he has the writing permission for this directory.
and.
(2) he is the owner of that file(dir).

With this restriction we can create a simple directory that people can share file with
each other but cannot modify other's file.

## umask

Umask is a property belongs to each process.

Umask is a mask that masks the permission of any file that you created.

Usually umask is inherited from the shell, and you can use some shell built-in function to modify it.

## i-node flags ( extension file attributes )

This is not part of Standard (?)

Some file system provide flags for file to provide more functionality.

`chattr` and `lsattr` can read these flags.

## Exercise

### problem 1

a) check [p1.sh](p1.sh)
b) check [p1.sh](p1.sh)
c)
    * create file : directory wx, user whatever
    * read file : directory wx, user r
    * write file : directory wx, user w
    * delete file : directory wx, user whatever
    * reanme file : directory wx, user whatever

| operation | directory | user                          |
| ---       | ---       | ---                           |
| creat     | wx        |                               |
| reading   | wx        | r                             |
| writing   | wx        | w                             |
| delete    | wx        |                               |
| rename    | wx        |                               |
| replace   | wx        |                               |
| (T)rename | wx        | owner of file or owner of dir |
| (T)delete | wx        | owner of file or owner of dir |

### problem 2

Nope

* atime: the last time the file was read
* mtime: the last time the file was modified(has benn modified)
* ctime: the last time file meta data was changed

`stat` read the meta data but don't change it.
And there is no content access so atime and mtime stay the same.

### problem 3

check [p3.c](p3.c)

### problem 4

check [p4.c](p4.c)

### problem 5

### New answer

Since Linux 4.7, umask was record in /proc/[PID]/status file.

### Old answer

According to man page, there is no way to fetch process's umask without change it at the same time

We can retrieve umask by two or more step, but this probably cause race condition.

### problem 6

check [p6.c](p6.c)

### problem 7

check [p7.c](p7.c)
