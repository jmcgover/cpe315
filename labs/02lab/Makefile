CC= gcc
CPP=g++
FLAGS= -g
LIBS=
EXES= c_towers towers

all: $(EXES)

towers: towers.s
	$(CC) $(FLAGS) $(LIBS) -o $@ $^

c_towers: c_towers.o
	$(CPP) $(FLAGS) $(LIBS) -o $@ $^

c_towers.o: c_towers.cpp
	$(CPP) $(FLAGS) $(LIBS) -c $^

.PHONY: test
test: towers test_files
	for i in {1..8}; do ./towers < test/in$$i.txt > test_files/out$$i.txt; done
	for i in {1..8}; do diff -s test/out$$i.txt test_files/out$$i.txt; done

.PHONY: c_test
c_test: c_towers c_test_files
	for i in {1..8}; do ./c_towers < test/in$$i.txt > c_test_files/out$$i.txt; done
	for i in {1..8}; do diff -s test/out$$i.txt c_test_files/out$$i.txt; done

test_files:
	mkdir -p $@

c_test_files:
	mkdir -p $@

clean:
	rm -fv $(EXES) *.o *~
