override CFILES := $(shell find ./ -type f -name '*.c')

.PHONY: run
run: a.out
	@./a.out

a.out: main.c $(CFILES)
	@gcc -std=gnu11 $(CFILES)

.PHONY: clean
clean:
	rm a.out