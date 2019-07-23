#!/usr/bin/env sh

# a)
echo  "test" > file     # create file
chmod 077 file          # remove every permission for owner
ls -al file             # list permission
cat file || echo "SUCCESS"           # cannot visit file

echo "y" | rm file > /dev/null 2>&1

# b)
mkdir directory         > /dev/null 2>&1 
touch directory/file    > /dev/null 2>&1
chmod 666 directory     # no executing permission
ls directory            # able to show the content
cat directory/file      # cannot visit it
echo "apple" > directory/file || echo "SUCCESS" # cannot visit it

chmod 766 directory     # 
rm -rf directory

rm -rf directory

# c)
mkdir cdir
echo "apple" > cdir/file

# creat file 
chmod 300 cdir
touch cdir/ooo && echo "YYY" || echo no

# read file -> directory at least executing permission, file at least read permission
chmod 400 cdir/file ; chmod 100 cdir
cat cdir/file && echo "SUCCESS" || echo no

# write file -> directory at least executing permission, file at least writing permission
chmod 200 cdir/file ; chmod 100 cdir
echo "apple" >> cdir/file && echo "SUCCESS" || echo "NO"

# delete file -> directory at least executing and writing permission.
chmod 000 cdir/file; chmod 300 cdir
echo "y" | rm cdir/file && echo "SUCCESS" || echo "NO"

# rename file -> directory at least executing and writing permission.
echo "apple pie" > cdir/file
chmod 000 cdir/file; chmod 300 cdir
mv cdir/file cdir/good && echo "SUCCESS" || echo "NO"

# rename file and file exist -> directory at least executing and writing permission
echo "apple a" > cdir/cool
chmod 300 cdir; chmod 000 cdir/cool; chmod 000 cdir/good;
echo y | mv cdir/cool cdir/good && echo "SUC" || echo "NO"

# rename file -> directory at least executing and writing permission, and you have to be the owner of file or owner of this dir
echo "pineapple pie" > cdir/file
chmod 000 cdir/file ; chmod 333 cdir ; chmod +t cdir
sudo chown root cdir/file
sudo chown root cdir
mv cdir/file cdir/weed && echo "no" || echo "YES, We cannot rename it"

# detect file -> like above
rm cdir/weed || echo "YES, we cannot delete it"

# I am the owner of this dir
sudo chown greyparrot cdir

# rename and delete
echo "y" | mv cdir/good cdir/file && echo "OK" || echo "no"
echo "y" | rm cdir/file && echo "OK" || echo "no"

rm -rf cdir
