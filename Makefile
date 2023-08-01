CC=gcc
LIBS=-lSDL2
CFLAGS=-Wall -ggdb

chip8: chip8.c api.c  instruction.c soc.c options.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)
test: test.c api.c instruction.c soc.c 
	$(CC) $(CFLAGS) $^ -o $@
headless: headless.c api.c instruction.c soc.c options.c
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f test chip8 headless
