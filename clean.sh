#!/bin/sh

# file to remove build results from the current dir.

find . -name *.o -print | xargs rm
