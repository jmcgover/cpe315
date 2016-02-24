CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = structs.cpp
BIN = computeavg computeavg_opt

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

.PHONY: run run_og run_opt clean
run: run_og run_opt

run_og: computeavg
	./computeavg

run_opt: computeavg_opt
	./computeavg_opt

clean:
	rm -fv computeavg.o computeavg_opt.o *~ $(BIN)
