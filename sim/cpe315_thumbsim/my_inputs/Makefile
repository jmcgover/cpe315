SIMS += test_mov.sim
SIMS += test_add.sim

all: $(SIMS)

%.sim: %
	objdump -D $* | ./gensimcode4 > $@

%: %.s
	gcc -mthumb -msoft-float -o $* $<

.PHONY: clean
clean:
	rm -fv *.sim
