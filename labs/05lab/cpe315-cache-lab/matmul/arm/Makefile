CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = matmul.cpp 
BIN = mm mm_opt
OBJ = mm.o mm_opt.o

all: mm mm_opt

mm: mm.o hrt-arm.o driver-arm.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm.o: matmul.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

mm_opt: mm_opt.o hrt-arm.o driver-arm.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm_opt.o: matmul_opt.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

run: og_mm_arm.txt opt_mm_arm.txt
	diff -qs output.gold og_mm_arm.txt
	diff -qs output.gold opt_mm_arm.txt

og_mm_arm.txt: mm
	rpistat ./$^  1> og_mm_arm.txt 2>og_time_mm_arm.txt
	cat rpistat.txt  >> og_time_mm_arm.txt
	rm -fv rpistat.txt

opt_mm_arm.txt: mm_opt
	rpistat ./$^  1> opt_mm_arm.txt 2>opt_time_mm_arm.txt
	cat rpistat.txt  >> opt_time_mm_arm.txt
	rm -fv rpistat.txt

.PHONY: clean cleanall

clean:
	rm -fv $(OBJ) $(BIN)

cleanall: clean
	rm -fv *~ *.txt
