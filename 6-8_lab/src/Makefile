all: main_prog counting

main_prog:
	g++ main_prog.cpp -lzmq -o main_prog -Wall -pedantic

counting:
	g++ count_node.cpp -lzmq -o count_node -Wall -pedantic

clean:
	rm -rf main_prog count_node
