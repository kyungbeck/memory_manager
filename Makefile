all:
	g++ -g -o malloc malloc.cpp
run:
	./malloc
view:
	cat ./result.out
