# Config, just variables
CC=gcc
CFLAGS=-Wall -g
LFLAGS=-lm
TARGET=out
OUTFILE=months.txt

# Generate source and object lists by using wildcards to find .c and .h files in the project folders
C_SRCS := \
	$(wildcard *.c) \
	$(wildcard src/*.c) \
	$(wildcard src/**/*.c)

HDRS := \
	$(wildcard *.h) \
	$(wildcard src/*.h) \
	$(wildcard src/**/*.h) 

# Take the list of sources and generate an appropriate list of object files. Uses patsubst to generate a list of files to turn from .c to .o 
OBJS := $(patsubst %.c, bin/%.o, $(wildcard *.c))
OBJS += $(filter %.o, $(patsubst src/%.c, bin/%.o, $(C_SRCS)))

# default make command leads to build and to say All Done!
all: build
	@echo "All Done!"

# Link all built objects
build: $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LFLAGS)

# special build rule
# Catch root directory src files
bin/%.o: %.c $(HDRS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# special build rule
# Catch all nested directory files
bin/%.o: src/%.c $(HDRS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Allow for fresh builds and a fresh workspace without removing source code 
clean:
	rm -f $(TARGET)
	rm -rf bin

# Quick build and run output command
run: build
	./$(TARGET) $(OUTFILE)

# List found sources, headers, and target objects
which:
	@echo "FOUND SOURCES: ${C_SRCS}"
	@echo "FOUND HEADERS: ${HDRS}"
	@echo "TARGET OBJS: ${OBJS}"

