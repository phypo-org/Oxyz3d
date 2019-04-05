include ${FLTK_PATH}/makeinclude

TARGET=Test

SRCS= Test.cpp

OBJS =  $(SRCS:.cpp=.o)

HDRS = $(SRCS:.cpp=.h)


CFLAGS = -g -std=c++17 -O3   -Wall  -pthread -D_POSIX_PTHREAD_SEMANTICS  -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT

# CINCLUDE=-I. -IShape -DLINUX -I/usr/include/python3.6
CINCLUDE=-I. -I../Lua -DLINUX 
CC=/usr/bin/clang
CC=g++




#  LIBS=  Shape/*.o -L$(FLTK_PATH)/lib -lfltk_gl -lfltk -lfltk_images $(GLDLIBS) -L/usr/lib/python3.6/onfig-3.6m-x86_64-linux-gnu -lpython3.6m -ljpeg -lpng  -lXinerama -lpthread -ldl -lm

LIBS=  ../Lua/*.o -L/usr/local/lib  -llua  -lpthread -ldl -lm  



# ---------------------------------------------------


all::			start; make -C ../Lua  ;

all::		$(OBJS)

all::		$(TARGET)


start:
		rm -f  $(TARGET)


clean::
		rm     -f *.o    *.a  core mem.err $(TARGET)

cleanall:
		cd Shape; make clean ;cd ..;\
		rm     -f *.o    *.a  core $(TARGET)



# ---------------------------------------------------
# Compilation
.cpp.o:
	$(CC)   $(CFLAGS)  $(CINCLUDE) -c $*.cpp



$(TARGET):
	echo Linking $@...
	rm -f $(TARGET)
	$(CC) -g -Wall -Wunused -Wno-format-y2k  -fno-exceptions -fno-strict-aliasing -ffunction-sections -fdata-sections -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_THREAD_SAFE -D_REENTRANT -I/usr/include/freetype2  -I/usr/include/freetype2    -Os -Wall -Wunused -Wno-format-y2k  -fno-exceptions -fno-strict-aliasing -ffunction-sections -fdata-sections  -o $(TARGET) $(OBJS) $(LIBS)

