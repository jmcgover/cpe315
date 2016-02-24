CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = matmul.cpp 
BIN = mm mm_opt
OBJ = mm.o mm_opt.o

all: mm mm_opt

mm: mm.o hrt-intel.o driver-intel.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm.o: matmul.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

mm_opt: mm_opt.o hrt-intel.o driver-intel.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm_opt.o: matmul_opt.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

run: og_mm_intel.txt opt_mm_intel.txt
	diff -qs output.gold og_mm_intel.txt
	diff -qs output.gold opt_mm_intel.txt

og_mm_intel.txt: mm
	perf stat -e cache-misses ./$^  1> og_mm_intel.txt 2>og_time_mm_intel.txt

opt_mm_intel.txt: mm_opt
	perf stat -e cache-misses ./$^  1> opt_mm_intel.txt 2>opt_time_mm_intel.txt

.PHONY: clean cleanall

clean:
	rm -fv $(OBJ) $(BIN)

cleanall: clean
	rm -fv *~ *.txt
