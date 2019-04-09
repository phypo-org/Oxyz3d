

TARGET=Shape

SRCS= PLua.cpp 



OBJS =  $(SRCS:.cpp=.o)


HDRS = $(SRCS:.cpp=.h)


CFLAGS = -g -std=c++17 -O3  -Wall  -pthread -D_POSIX_PTHREAD_SEMANTICS  -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT 

CINCLUDE=-I.  -DLINUX -I lua
CC=/usr/bin/clang
CC=g++



# ---------------------------------------------------
all::		start;	


all::		$(OBJS)

all::		$(TARGET)


start:
		rm -f  $(TARGET)


clean::
		rm     -f *.o    *.a  core $(TARGET)



# ---------------------------------------------------
# Compilation
.cpp.o:
	$(CC)   $(CFLAGS)  $(CINCLUDE) -c $*.cpp


$(TARGET):
