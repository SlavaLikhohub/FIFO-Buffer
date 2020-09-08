DEBUG ?= 0
TARGET := fifo_buffer
TARGET_SHARED := $(TARGET).sh
SRCDIR = ./src
INCDIRS = ./include

SRCS = fifo_buffer.c

LIBS = 

BUILDDIR = .build


CFLAGS = -O2 -std=gnu17 -Wall -Wextra -Wpedantic
CFLAGS += $(addprefix -I,$(INCDIRS))
CFLAGS += $(shell pkg-config --cflags $(LIBS))
CFLAGS += -DDEBUG=$(DEBUG)
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -Wl,--as-needed
CC := gcc

.PHONY: all clean tidy run

all: $(BUILDDIR)/$(TARGET_SHARED)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) -c $(CFLAGS)  -fpic $< -o $@

$(BUILDDIR)/$(TARGET_SHARED): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(CC) -shared $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILDDIR):
	mkdir -p $@

clean:
	-rm -rf $(BUILDDIR)

tidy: clean
	-rm -f $(TARGET_SHARED)
