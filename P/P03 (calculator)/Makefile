COMPILER = gcc
FLAGS = -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body  -Wstrict-prototypes  -Wold-style-definition  -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11

solution: main.c stack.c stack.h
	$(COMPILER) $(FLAGS) main.c stack.c -o solution

clean:
	rm -f solution