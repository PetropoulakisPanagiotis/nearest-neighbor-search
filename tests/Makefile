CC = g++
FLAGS = -g -Wall

lsh: lsh.o utils.o hashFunction.o item.o neighborsProblem.o 
	$(CC) -o lsh $(FLAGS) lsh.o utils.o hashFunction.o item.o neighborsProblem.o -std=c++11

lsh.o: lsh.cc
	$(CC) -c  $(FLAGS) lsh.cc -std=c++11

utils.o: ../utils/utils.cc
	$(CC) -c $(FLAGS) ../utils/utils.cc -std=c++11

hashFunction.o: ../hashFunction/hashFunction.cc
	$(CC) -c $(FLAGS) ../hashFunction/hashFunction.cc -std=c++11

item.o: ../item/item.cc
	$(CC) -c $(FLAGS) ../item/item.cc -std=c++11

neighborsProblem.o: ../neighborsProblem/neighborsProblem.cc
	$(CC) -c $(FLAGS) ../neighborsProblem/neighborsProblem.cc -std=c++11

.PHONY:
	clean
	check


clean:
	rm -rf lsh lsh.o utils.o hashFunction.o item.o neighborsProblem.o

check:
	g++ -o lsh lsh.cc ../utils/utils.cc ../hashFunction/hashFunction.cc ../item/item.cc ../neighborsProblem/neighborsProblem.cc -std=c++11 && valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all --vgdb-error=1 ./lsh 
