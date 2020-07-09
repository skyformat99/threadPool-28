#ifndef WORKER_H_
#define WORKER_H_

#include<iostream>

class worker{
public:
    worker(int i):id(i){}
    void work(){
        std::cout << "worker: " << id << std::endl;
    }
    int getId(){return id;}
private:
    int id;
};

#endif