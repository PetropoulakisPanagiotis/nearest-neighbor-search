CC = g++
FLAGS = -g -Wall

lsh: lsh.o utils.o hashFunction.o item.o models.o 
	$(CC) -o lsh $(FLAGS) lsh.o utils.o hashFunction.o item.o models.o -std=c++11

lsh.o: lsh.cc
	$(CC) -c  $(FLAGS) lsh.cc -std=c++11

utils.o: ../../neighborsProblem/utils/utils.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/utils/utils.cc -std=c++11

hashFunction.o: ../../neighborsProblem/hashFunction/hashFunction.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/hashFunction/hashFunction.cc -std=c++11

item.o: ../../neighborsProblem/item/item.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/item/item.cc -std=c++11

models.o: ../../neighborsProblem/models/models.cc
	$(CC) -c $(FLAGS) ../../neighborsProblem/models/models.cc -std=c++11

.PHONY:
	clean
	check


clean:
	rm -rf lsh lsh.o utils.o hashFunction.o item.o models.o

check:
	g++ -o lsh lsh.cc ../../neighborsProblem/utils/utils.cc ../../neighborsProblem/hashFunction/hashFunction.cc ../../neighborsProblem/item/item.cc ../../neighborsProblem/models/models.cc -std=c++11 && valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --vgdb-error=1 ./lsh 
