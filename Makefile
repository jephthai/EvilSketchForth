CC=gcc
CFLAGS=-Wall -Os 
LDFLAGS=

OBJ = 	forth.o \
	stacks.o \
	dictionary.o \
	cstring.o \
	utility.o \
	global.o \
	core_words.o \
	interpreter.o \
	core.o \
	compiler.o

%.o: %.c %.h Makefile 
	$(CC) -c $(CFLAGS) $< -o $@

forth: $(OBJ) 
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)
	strip forth

clean:
	rm *.o forth

stats:
	echo
	sloccount *.c *.h | grep -A4 ^Totals
	cat core.fth | grep -av '^\s*\\' | grep -a '\S' | wc
