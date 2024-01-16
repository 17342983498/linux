.PHONY:all
all:mykill loop

loop:loop.cc
	g++ -o $@ $^ -std=c++11

mykill:mykill.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -f mykill loop