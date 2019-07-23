# Chapter 17 ACL 控制訪問列表

ACL provide precise and complicated control for file permission.

ACL is an implementation option for file system, most fo modern file system have implement it.
Also your kernel have to support it.

There are different type of ACL for choice, there is no standard for it.
But most of the UNIX system have implement it, and each implementation are different.
So ACL portability is freaking bad.

## ACE 

each entry in ACL called ACE

* ACL\_USER\_OBJ : user owner's permission
* ACL\_GROUP\_OBJ: group owner's permission
* ACL\_OTHER     : permission for other user
* ACL\_USER      : permission for specific users
* ACL\_GROUP     : permission for specific groups
* ACL\_MASK      : permission mask, the maximum permission for ACL\_GROUP\_OBJ ACL\_GROUP ACL\_USER

Minimal ACL composed of 1 2 3 
If file's ACL record has ACL\_GROUP or ACL\_USER, ACL\_MASK must exist.

## Mask

For very compilcated reason, when we change permission by chmod, the value get changed is ACL\_MASK

ACL\_MASK will become the union of all(usr/grp/oth) permission.

## Default ACL

This is second type of ACL - Deafult ACL, Deafult ACL is for directory only.

ACL store in `system.posix_acl_default` EA

Default ACL decide the visiting ACL(first type of ACL) permission for new created dir or file under it.
Since sub-directory will inherited ther permission. The Default ACL setting will spread all the way down.

These permission will change: 

1. new file ACL-USER-OBJ = dir deafult ACL-USER-OBJ & CREAT-PERMISSION & USUAL-PERMISSION
2. new file ACL-MASK     = dir default MASK & CREAT-PERMISSION & USUAL-PERMISSION
If there is no ACL_-MASK
2. new file ACL-_GROUP-OBJ= dir deafult ACL-GROUP-OBJ & CREAT-PERMISSION & USUAL-PERMISSION
3. new file ACL-OTHER    = dir default ACL-OTHER & CREAT-PERMISSION & USUAL-PERMISSION

I know this is freaking compilcated, which genius create this thing?

By the way, umask won't take part of the calculation if Deafult ACL is on.

If Default ACL is clear, the calculation of permission will stay the same.
