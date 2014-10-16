#!/bin/sh

find | egrep '\.c$|\.h$' | xargs wc -l
