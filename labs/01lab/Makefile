CC= gcc
FLAGS=
LIBS=
EXES= hello_world piku

all: $(EXES)

hello_world: hello_world.s
	$(CC) $(FLAGS) $(LIBS) -o $@ $^

piku: piku.s
	$(CC) $(FLAGS) $(LIBS) -o $@ $^

clean:
	rm -fv $(EXES) *.o *~
