CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = student.c course.c school.c file_io.c interactive.c

all: school_management

school_management: main.c $(SRC)
	$(CC) $(CFLAGS) -o school_management main.c $(SRC)

test: tests.c $(SRC)
	$(CC) $(CFLAGS) -o run_tests tests.c $(SRC)
	./run_tests

clean:
	rm -f school_management run_tests *.o test_roundtrip.txt ucf_save.txt

.PHONY: all test clean
