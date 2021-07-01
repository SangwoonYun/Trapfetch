CC=gcc
OBJS=ptrace.c
TARGET=ptrace.out

all: $(TARGET)

install:
	cp lib/ptrace-shared.h /usr/include/x86_64-linux-gnu/bits/ptrace-shared.h

clean:
	rm -f *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
