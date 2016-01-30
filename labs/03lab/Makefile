CC= gcc
CPP=g++
FLAGS=-O1 -g -mthumb -msoft-float
CFLAGS=-O2 -g
LIBS=
EXES= mymult mult

all: $(EXES)

mymult: mymult.s
	$(CC) $(FLAGS) $(LIBS) -o $@ $^

mult: mult.o
	$(CPP) $(FLAGS) $(LIBS) -o $@ $^

mult.o: mult.cpp
	$(CPP) $(CFLAGS) $(LIBS) -c $^

.PHONY: test
test: mymult test_infiles test_outfiles
	./mymult < test_files/in/in1.txt > test_files/out/my_out1.txt
	diff -sw test_files/out/out1.txt test_files/out/my_out1.txt

.PHONY: c_test
c_test: mult c_test_infiles c_test_outfiles
	./mult < c_test_files/in/in1.txt > c_test_files/out/my_out1.txt
	diff -sw c_test_files/out/out1.txt c_test_files/out/my_out1.txt

test_files/in:
	mkdir -p $@
test_files/out:
	mkdir -p $@

.PHONY: test_infiles
test_infiles: test_files/in
	echo "$$input_file" > $^/in1.txt
.PHONY: test_outfiles
test_outfiles: test_files/out
	echo "$$output_file" > $^/out1.txt

c_test_files/in:
	mkdir -p $@
c_test_files/out:
	mkdir -p $@

.PHONY: c_test_infiles
c_test_infiles: c_test_files/in
	echo "$$input_file" > $^/in1.txt
.PHONY: c_test_outfiles
c_test_outfiles: c_test_files/out
	echo "$$output_file" > $^/out1.txt

clean:
	rm -rfv $(EXES) *.o *~ c_test_files test_files

export input_file
define input_file
17
9
y
8
9
y
11
10
n
endef

export output_file
define output_file
Enter Number 1: Enter Number 2: Product is: 153
Again? Enter Number 1: Enter Number 2: Product is: 72
Again? Enter Number 1: Enter Number 2: Product is: 110
Again? 
endef
