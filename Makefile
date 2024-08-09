CC := g++

SRCS := ./colorBasePort.cpp ./colorBase.cpp
SRCS += ./example/main.cpp

CFLAGS := -I./include
CFLAGS += -I./third_party/include

all: 
	@mkdir -p ./build
	$(CC) $(CFLAGS) -o ./build/test $(SRCS)

clean:
	@rm -r ./build

.PHONY: all clean
