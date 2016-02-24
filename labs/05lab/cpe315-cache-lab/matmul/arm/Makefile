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

clean:
	rm -fv mm.o mm_opt.o $(BIN)
