all:            rpistat
 
rpistat:        rpistat.c test_common.o rpi_pmu.o
	gcc -ggdb -O2 -o rpistat rpistat.c test_common.o rpi_pmu.o
 
test_common.o:  ../test_common/test_common.h ../test_common/test_common.h
	gcc -O2 -c ../test_common/test_common.c
 
rpi_pmu.o:      ../test_common/rpi_pmu.h ../test_common/rpi_pmu.c
	gcc -O2 -c ../test_common/rpi_pmu.c
 
clean:

