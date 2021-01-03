CC = gcc
COBJS = main.o
CFLAGS = -Wall -Wextra -pedantic-errors -Wshadow -Wpointer-arith -Wwrite-strings -Wredundant-decls -Wnested-externs -Ofast -lcurl

all: $(COBJS)
	@$(CC) -o mailvin $< $(CFLAGS)

run: all
	@./mailvin

%.o: %.c
	@$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -f *.o
