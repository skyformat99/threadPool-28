all: main.o
	g++ -o all main.o -g -lpthread  

main.o: main.cpp
	g++ -c main.cpp -o main.o

app: main.cpp threadPool.h worker.h lock.h
	g++ -o app  $^ -g -lpthread

clean:
	rm -rf *.o app all 