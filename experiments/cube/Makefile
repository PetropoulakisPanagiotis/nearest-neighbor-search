# Petropoulakis Panagiotis
CC = g++
FLAGS = -g -Wall

cube: cube.o utils.o hashFunction.o item.o fileHandler.o hypercubeEuclidean.o hypercubeCosine.o exhaustiveSearch.o
	$(CC) -o cube $(FLAGS) cube.o utils.o hashFunction.o item.o fileHandler.o hypercubeEuclidean.o hypercubeCosine.o exhaustiveSearch.o -std=c++11

cube.o: cube.cc
	$(CC) -c  $(FLAGS) cube.cc -std=c++11

utils.o: ../../neighborsProblem/utils/utils.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/utils/utils.cc -std=c++11

hashFunction.o: ../../neighborsProblem/hashFunction/hashFunction.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/hashFunction/hashFunction.cc -std=c++11

item.o: ../../neighborsProblem/item/item.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/item/item.cc -std=c++11

fileHandler.o: ../../neighborsProblem/fileHandler/fileHandler.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/fileHandler/fileHandler.cc -std=c++11

hypercubeEuclidean.o: ../../neighborsProblem/model/hypercube/hypercubeEuclidean.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/model/hypercube/hypercubeEuclidean.cc -std=c++11

hypercubeCosine.o: ../../neighborsProblem/model/hypercube/hypercubeCosine.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/model/hypercube/hypercubeCosine.cc -std=c++11

exhaustiveSearch.o: ../../neighborsProblem/model/exhaustiveSearch/exhaustiveSearch.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/model/exhaustiveSearch/exhaustiveSearch.cc -std=c++11

.PHONY:
	clean
	check

clean:
	rm -rf cube cube.o utils.o hashFunction.o item.o fileHandler.o hypercubeEuclidean.o hypercubeCosine.o exhaustiveSearch.o

check:
	g++ -o cube cube.cc ../../neighborsProblem/utils/utils.cc ../../neighborsProblem/hashFunction/hashFunction.cc ../../neighborsProblem/item/item.cc ../../neighborsProblem/fileHandler/fileHandler.cc ../../neighborsProblem/model/hypercube/hypercubeEuclidean.cc ../../neighborsProblem/model/hypercube/hypercubeCosine.cc ../../neighborsProblem/model/exhaustiveSearch/exhaustiveSearch.cc -std=c++11 && valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --vgdb-error=1 ./lsh 
