LINKFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi
CFLAGS    = -g -Wall -ansi
CC        = g++

SRCS      = $(wildcard *.cpp)
OBJS      = $(SRCS:.cpp=.o)
PROG      = a.out

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

depend:
	makedepend -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)
