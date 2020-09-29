all: build_all link_all run

build_all:
	g++ -c test_LIRS.cpp tests.cpp

link_all:
	g++ test_LIRS.o tests.o -o main

run:
	./main
