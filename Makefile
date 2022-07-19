PROGRAM = croguelike

CFLAGS  = -g -Wall -Wextra -Wpedantic
LDFLAGS = -lncurses

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

PREFIX     ?= $(DESTDIR)/usr/local
BIN_PREFIX ?= $(PREFIX)/bin

.PHONY: all
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DEPS)

.PHONY: clean
clean:
	$(RM) $(OBJS) $(DEPS) $(PROGRAM)

.PHONY: install
install: all
	install $(PROGRAM) $(BIN_PREFIX)/$(PROGRAM)

.PHONY: uninstall
uninstall:
	$(RM) $(BIN_PREFIX)/$(PROGRAM)
