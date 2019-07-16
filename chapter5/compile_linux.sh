#!/bin/sh

cmd="gcc -Wall -I $HOME/Programming/TLPI/lib $HOME/Programming/TLPI/lib/*.c $@"
echo  "$cmd"
sh -c "$cmd"
