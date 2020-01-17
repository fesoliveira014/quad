CC = gcc
CFLAGS = -O3 -Wall -fPIC
LDFLAGS = -shared
INCLUDE = -I include
SRCS = $(shell find src -maxdepth 2 -name "*.cpp" -printf %f'\n')
OBJS = $(SRCS:.cpp=.o)
BIN = bin

.PHONY: all

test: libquad
	$(CC) -L$(BIN) -lquad.lib test/main.c -o $(BIN)/test

libquad: $(addprefix $(BIN)/, $(OBJS))
	$(CC) $(LDFLAGS) $^ $(INCLUDE) -o ./$(BIN)/$@ 

$(BIN)/%.o: %.cpp | directories
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ 

clean:
	rm -rf $(BIN)

directory:
	@echo=off mkdir -p $(BIN)