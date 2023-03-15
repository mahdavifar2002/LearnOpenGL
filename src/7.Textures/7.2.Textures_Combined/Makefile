LINKFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
CFLAGS    = -g -Wall -ansi
CC        = g++

C_SRCS    = $(wildcard ../../*.c)
CPP_SRCS  = $(wildcard *.cpp)
OBJS      = $(CPP_SRCS:.cpp=.o) $(C_SRCS:.c=.o)
PROG      = a.out

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

depend:
	makedepend -Y $(C_SRCS) $(CPP_SRCS)

clean:
	rm $(OBJS) $(PROG)
