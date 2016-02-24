SIMS += test_mov.sim
SIMS += test_add.sim
SIMS += test_sub.sim
SIMS += test_cmp.sim
SIMS += test_cmpi.sim

all: $(SIMS)

%.sim: %.s
	gcc -mthumb -msoft-float -o $* $<
	objdump -D $* | ./gensimcode4 > $@

.PHONY: clean
clean:
	rm -fv *.sim
