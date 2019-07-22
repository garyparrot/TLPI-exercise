#!/bin/sh

# Test script for exercise 1
# this script require at least 5GB available space on your current file system.

# Testing ext2 filesystem

order=$3
dir=$2
files=$1

echo "Creat" $files "files" 

# creat specific file system image
echo "ext2"
echo "y" | mkfs -t ext2 image 5g > /dev/null 2> /dev/null
# mount it to specific directory
mount -o loop ./image $dir > /dev/null
# test it
./a.out $files $dir $order
#umount
umount $dir

# creat specific file system image
echo "ext3"
echo "y" | mkfs -t ext3 image 5g > /dev/null 2> /dev/nyll
# mount it to specific directory
mount -o loop ./image $dir > /dev/null
# test it
./a.out $files $dir $order
#umount
umount $dir

# creat specific file system image
echo "ext4"
echo "y" | mkfs -t ext4 image 5g > /dev/null 2> /dev/null
# mount it to specific directory
mount -o loop ./image $dir > /dev/null
# test it
./a.out $files $dir $order
#umount
umount $dir

# creat specific file system image
echo "xfs"
echo "y" | mkfs -t xfs -f image > /dev/null 2> /dev/null
# mount it to specific directory
mount -o loop ./image $dir > /dev/null
# test it
./a.out $files $dir $order
#umount
umount $dir

# creat specific file system image
echo "vfat"
echo "y" | mkfs -t vfat image > /dev/null 2> /dev/null
# mount it to specific directory
mount -o loop ./image $dir > /dev/null
# test it
./a.out $files $dir $order
#umount
umount $dir

