CC=gcc
CFLAGS=-Wall -Wextra -O2
LDFLAGS=-Wl,--gc-sections

TARGET=humanos

SRCS=kernel/scheduler.c \
     drivers/example/anti_inflammation.c

OBJS=$(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
