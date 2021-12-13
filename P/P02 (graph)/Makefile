COMPILER = gcc
FLAGS = -O2 -ftrapv -fsanitize=address,undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm

all: 1 2 3 4 5 clean

1:
	$(COMPILER) bin_tree_lib.c -c -g $(FLAGS)
2:
	$(COMPILER) bin_tree_input_output.c -c -g $(FLAGS)
3:
	$(COMPILER) queue_lib.c -c -g $(FLAGS)
4:
	$(COMPILER) main.c -c -g $(FLAGS)
5:
	$(COMPILER) main.o bin_tree_lib.o bin_tree_input_output.o queue_lib.o -o myprog -g $(FLAGS)

clean:
	rm -f bin_tree_lib.o 
	rm -f bin_tree_input_output.o 
	rm -f queue_lib.o 
	rm -f main.o