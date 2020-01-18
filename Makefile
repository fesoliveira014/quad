CC = gcc
CFLAGS = -std=c11 -O0 -Wall
LDFLAGS = 
INCLUDE = -I include
SRCS = $(shell find src -maxdepth 2 -name "*.c" -printf %f'\n')
TESTSRCS = $(shell find test -maxdepth 2 -name "*.c" -printf %f'\n')
SRCDIR = src
TESTDIR = test
OBJS = $(SRCS:.c=.o)
OBJS += $(TESTSRCS:.c=.o)
BIN = bin
# VPATH = test ../src ../include

build: quadtest

.PHONY: build clean

all: quadtest

quadtest: 
	$(CC) $(CFLAGS) $(INCLUDE) $(addprefix $(SRCDIR)/, $(SRCS)) $(addprefix $(TESTDIR)/, $(TESTSRCS)) -o $(BIN)/$@

clean:
	rm -rf $(BIN)

directory:
	@echo=off mkdir -p $(BIN)