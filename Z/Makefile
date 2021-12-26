COMPILER = gcc
FLAGS = -O2 -ftrapv -fsanitize=address,undefined -Wall  -Wextra -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body  -Wstrict-prototypes  -Wold-style-definition  -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -g

SRCS=$(wildcard *.c)

OBJS=$(SRCS:.c=)

all: $(OBJS)

test: $(SRC)
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -f test