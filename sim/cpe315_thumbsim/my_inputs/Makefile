SIMS += test_mov.sim
SIMS += test_add.sim
SIMS += test_sub.sim
SIMS += test_cmp.sim
SIMS += test_cmpi.sim
SIMS += test_pushpop.sim
SIMS += test_stmldm.sim
SIMS += test_strldr.sim

.PHONY: all
all: $(SIMS)

%.sim: %.s
	gcc -g -mthumb -msoft-float -o $* $<
	objdump -D $* | ./gensimcode4 > $@

.PHONY: clean
clean:
	rm -fv *.sim
