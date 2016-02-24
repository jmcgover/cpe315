CXX = g++
CFLAGS = -Wall -O2
LDFLAGS = -lrt
SRCS = matmul.cpp 
BIN = mm

all:
	$(CXX) $(CFLAGS) $(LDFLAGS) -c -o $(BIN).o $(SRCS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $(BIN) *.o

clean:
	rm -f $(BIN).o $(BIN)
