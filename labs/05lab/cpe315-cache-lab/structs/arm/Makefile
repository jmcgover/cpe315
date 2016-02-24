CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = structs.cpp
BIN = computeavg computeavg_opt
OBJ = computeavg.o computeavg_opt.o

all: computeavg computeavg_opt
#	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(BIN).o $(SRCS)
#	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN) *.o

computeavg: computeavg.o hrt-arm.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

computeavg.o: structs.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

computeavg_opt: computeavg_opt.o hrt-arm.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^

computeavg_opt.o: structs_opt.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $@ $^

.PHONY: run run_og run_opt clean cleanall
run: run_og run_opt

run_og: computeavg
	perf stat -e cache-misses ./computeavg 2>&1 | tee og_time_struct_arm.txt

run_opt: computeavg_opt
	perf stat -e cache-misses ./computeavg_opt 2>&1| tee opt_time_struct_arm.txt

clean:
	rm -fv computeavg.o computeavg_opt.o *~ $(BIN)

cleanall: clean
	rm -fv *.txt
