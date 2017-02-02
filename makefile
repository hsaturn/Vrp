GCC=g++ -std=c++11

all: test.cpp
	$(GCC) test.cpp -o test

run: all
	./test

help:
	echo "make all build the executable"
	echo "run the executable"

clean:
	rm -f test
