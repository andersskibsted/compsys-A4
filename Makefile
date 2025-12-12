# GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 -O
TESTS=sim-test.sh
.PHONY: test

all: sim
rebuild: clean all

# sim nedds simulate and disassemble to work!
sim: *.c *.h
	$(GCC) *.c -o sim 

test: $(TESTS)
	@set e; for test in $(TESTS); do echo ./$$test; ./$$test; done

zip: ../src.zip

../src.zip: clean
	cd .. && zip -r src.zip src/Makefile src/*.c src/*.h

clean:
	rm -rf *.o sim  vgcore*
