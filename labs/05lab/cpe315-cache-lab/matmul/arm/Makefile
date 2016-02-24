CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = matmul.cpp
BIN = mm mm_opt

all: mm mm_opt

mm: mm.o hrt-arm.o driver-arm.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm.o: matmul.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

mm_opt: mm_opt.o hrt-arm.o driver-arm.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

mm_opt.o: matmul_opt.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

.PHONY: clean
comp: og_mm_arm.txt opt_mm_arm.txt
	diff -s output.gold og_mm_arm.txt
	diff -s output.gold opt_mm_arm.txt

og_mm_arm.txt: mm
	./$^ | tee og_mm_arm.txt

opt_mm_arm.txt: mm_opt
	./$^ | tee opt_mm_arm.txt

clean:
	rm -fv computeavg.o computeavg_opt.o *~ *.txt $(BIN)
