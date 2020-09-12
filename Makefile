DEBUG ?= 0
TARGET_FIFO := libfifo
TARGET_SHARED := $(TARGET_FIFO).sh
TARGET_STATIC := $(TARGET_FIFO).a
SRCDIR = ./src
INCDIRS = ./include

SRCS = fifo_buffer.c

BUILDDIR = .build/

AR ?= ar
CC ?= gcc

CFLAGS ?= -O2 -std=gnu17 -Wall -Wextra -Wpedantic
ARFLAGS ?= rcs
INCL := $(addprefix -I,$(INCDIRS)) 
DEF  := -DDEBUG=$(DEBUG)
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -Wl,--as-needed


.PHONY: all clean shared static static_stm32

all: shared

shared : $(BUILDDIR)/$(TARGET_SHARED)

static_stm32 : $(BUILDDIR)/$(TARGET_STATIC)

$(BUILDDIR)/$(TARGET_STATIC): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(AR) -rcs $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCL) -c $< -o $@

$(BUILDDIR)/$(TARGET_SHARED): $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o))
	$(CC) -shared $(CFLAGS) $(INCL) $(DEF) $(LDFLAGS) $^ -o $@

$(BUILDDIR):
	mkdir -p $@

clean:
	-rm -rf $(BUILDDIR)

