.PHONY: exe
exe: main.c
	@gcc main.c
	@./a.out

.PHONY: clean
clean:
	rm a.out