all: build_all link_all run

build_all:
	g++ -c test_cache.cpp io_handle.cpp

link_all:
	g++ test_cache.cpp io_handle.cpp -o main

run:
	./main $(cache)
