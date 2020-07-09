all: threadPool.o lock.o worker.o main.o
	g++ threadPool.o lock.o worker.o main.o -o all

main.o: main.cpp
	g++ -c main.cpp -o main.o
threadPool.o: threadPool.h 
	g++ -c threadPool.h -o threadPool.o
lock.o: lock.h
	g++ -c lock.h -o lock.o
worker.o: worker.h
	g++ -c worker.h -o worker.o


app: main.cpp threadPool.h worker.h lock.h
	g++ -o app  $^ -g -lpthread

clean:
	rm -r app