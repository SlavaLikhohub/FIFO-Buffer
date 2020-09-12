TARGET ?= py_fifo_buff

INCLUDE ?= ../include
INCLUDE += /usr/include/python3.8
BUILD_DIR = build
SRC_LIB = ../src
SRC_FILES = fifo_buffer.c

FIFO_DATA_TYPE = uint16_t

CC = gcc

CFLAGS := -shared -pthread -fPIC -fwrapv -O2 -Wall -fno-strict-aliasing \
		  $(addprefix -I,$(INCLUDE)) -D BUFF_STORED_T=$(FIFO_DATA_TYPE)

all: $(TARGET).so

.PHONY = all clear


$(TARGET).so: $(addprefix $(BUILD_DIR)/,$(SRC_FILES:.c=.o)) $(BUILD_DIR)/$(TARGET).o $(BUILD_DIR)
	$(CC) $(CFLAGS) $(addprefix $(BUILD_DIR)/,$(SRC_FILES:.c=.o)) $(BUILD_DIR)/$(TARGET).o -o $@

$(BUILD_DIR)/$(TARGET).c:$(TARGET).pyx
	cython -3 $(addprefix -I,$(INCLUDE)) $^
	mv $(TARGET).c $@

$(BUILD_DIR)/$(TARGET).o: $(BUILD_DIR)/$(TARGET).c
	$(CC) $(CFLAGS) -c $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_LIB)/%.c $(BUILD_DIR)
	gcc $(CFLAGS) -o $@ -c $<

$(BUILD_DIR): 
	mkdir $@

clear:
	rm -rf $(BUILD_DIR)