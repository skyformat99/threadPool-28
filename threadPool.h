#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<iostream>
#include<pthread.h>
#include<assert.h>
#include<vector>
#include<queue>

#include "lock.h"

using namespace std;

template<class T>
class threadPool{
private:
    int threadNum;
    int maxRequestNum;
    pthread_t* threads = nullptr;
    queue<T*> workQueue;
    locker poolLocker;
    sem queueState; // whether has tasks to do ? P/V operation

    static void *worker(void *arg);// pull task from workQueue
    void run();
public:
    threadPool(int thread_number = 8, int max_request = 10000);
    ~threadPool();
    bool append(T *request, int state);
    bool append_p(T *request);
};

template<class T>
threadPool<T>::threadPool(int _threadNum, int _maxRequestNum){
    if(_threadNum == 0 || _maxRequestNum == 0)
        assert(0);
    threadNum = _threadNum;
    maxRequestNum = _maxRequestNum;
    
    threads = new pthread_t[_threadNum];
    assert(threads != nullptr);

    for(int i = 0; i < _threadNum; ++i){
        int ret = pthread_create(threads + i, NULL, worker, this); // public static void worker(){...}
        if(ret != 0){
            delete[] threads;
            throw std::exception();
        }
        ret = pthread_detach(threads[i]);
        if(ret != 0){
            delete[] threads;
            throw std::exception();
        }
    }
    //std::cout << "init finished" << endl;
}

template<class T>
threadPool<T>::~threadPool(){
    delete[] threads;
}

template<class T>
bool threadPool<T>::append(T *request, int state){
    poolLocker.lock();
    if(workQueue.size() >= threadNum){
        poolLocker.unlock();
        return false;
    }
    workQueue.push(request);
    //std::cout << request->getId() << ": V++" << endl;
    //std::cout << "workQueue size:" << workQueue.size() << std::endl;
    queueState.post(); // V: +1
    poolLocker.unlock();

    return true;
}

template<class T>
void* threadPool<T>::worker(void *arg){
    //std::cout << "thread:" << pthread_self() << std::endl;
    //std::cout << "pool addr:" << arg << std::endl;
    threadPool* pool = (threadPool*) arg; // this
    pool->run();
    return pool;
}

template<class T>
void threadPool<T>::run(){
    while(true){
        /*
        * P operation. if > 0, reduce 1 and return; else block until it > 0
        * if there are no task to do, all threads will block here.
        * else some threads will reduce and continue to run the following code.
        */
        queueState.wait(); 

        poolLocker.lock();
        if(workQueue.empty()){ // there are no task in the workQueue, loop until workQueue is not empty
            poolLocker.unlock();
            continue;
        }
        T* request = workQueue.front();
        workQueue.pop();
        poolLocker.unlock();
        if (!request){
            std::cout << "request is null" << std::endl;
            continue;
        }
        
        request->work(); // request is workinng ! 
    }
}
#endif