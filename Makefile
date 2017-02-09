DIRINCLUDE=./include
DIROBJETO=./obj
CC=g++
CFLAGS=-I$(DIRINCLUDE) -g -lSDL2_net

_DEPEND = cs_network.h
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = cs_network.o client.o proxy.o server.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.cpp $(DEPEND)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

all: client.out proxy.out server.out

client.out: ./obj/cs_network.o ./obj/client.o
	@mkdir -p client
	$(CC) -o ./client/$@ $^ $(CFLAGS)

proxy.out: ./obj/cs_network.o ./obj/proxy.o
	@mkdir -p proxy
	$(CC) -o ./proxy/$@ $^ $(CFLAGS)

server.out: ./obj/cs_network.o ./obj/server.o
	@mkdir -p server
	$(CC) -o ./server/$@ $^ $(CFLAGS) -pthread
