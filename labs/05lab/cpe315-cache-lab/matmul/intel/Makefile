CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = matmul.cpp 
BIN = mm mm_opt

all: mm mm_opt

mm: mm.o hrt-intel.o driver-intel.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm.o: matmul.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

mm_opt: mm_opt.o hrt-intel.o driver-intel.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm_opt.o: matmul_opt.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

.PHONY: clean
comp: og_mm_intel.txt opt_mm_intel.txt
	diff -s output.gold og_mm_intel.txt
	diff -s output.gold opt_mm_intel.txt

og_mm_intel.txt: mm
	./$^ | tee og_mm_intel.txt

opt_mm_intel.txt: mm_opt
	./$^ | tee opt_mm_intel.txt

clean:
	rm -fv computeavg.o computeavg_opt.o *~ *.txt $(BIN)
