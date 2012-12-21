CFLAGS = -Wall -Werror -DDEBUG
CC = gcc
prog = "aes128"
sources = $(wildcard *.c)
OBJECTS=$(sources:.c=.o)

all: $(sources) $(prog)
  $(clean)

$(prog): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *o $(prog)
