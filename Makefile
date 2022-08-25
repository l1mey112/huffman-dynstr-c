override CFILES := $(shell find ./ -type f -name '*.c' -not -wholename '*example*')

.PHONY: run
run: a.out
	@./a.out

a.out: main.c $(CFILES)
	@gcc -ggdb -std=gnu11 $(CFILES)

.PHONY: clean
clean:
	rm a.out