override CFILES := $(shell find ./ -type f -name '*.c' -not -wholename '*example*')
override HFILES := $(shell find ./ -type f -name '*.h' -not -wholename '*example*')

.PHONY: run
run: a.out
	@./a.out
 
a.out: main.c $(CFILES) $(HFILES) Makefile
	@gcc -fanalyzer -Og -ggdb -std=gnu11 $(CFILES)
#	-Wall -fsanitize=address,pointer-compare,leak,undefined,pointer-overflow

.PHONY: clean
clean:
	rm a.out