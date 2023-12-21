#! /bin/bash
# ./PPD --sync

export LD_LIBRARY_PATH=${MYGCC}lib64

gdb  -ex=run --args Debug/Oxyz3d.exe -lua -load "toto.oxyz" --sync $*

# valgrind -v --tool=memcheck --leak-check=yes  --show-reachable=yes --error-limit=no --num-callers=50  --freelist-vol=20000000  --log-file=mem.err ./PPD --sync $*
