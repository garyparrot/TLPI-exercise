# Chapter 28

## Kernel scheduling entity (KES)

Despite a few difference, Thread and Process are almost the same.

They are both Kernel scheduling entity(the instance for job scheduling)

The difference is Thread and Process share different property with each other(other KES).

## futex - fast user-space locking

This system call provide a method for waiting until a certain condition becomes true.

The synchronization operations is performed in user space .

