#include<iostream>
#include<vector>

#include "threadPool.h"
#include "worker.h"
#include <unistd.h>

using namespace std;

int main(){
    threadPool<worker>* pool = new threadPool<worker>(8, 10000);

    cout << "init worker..." << endl;
    vector<worker*> workers;
    for(int i = 0; i < 20; ++i){
        workers.push_back(new worker(i));
    }

    for(worker* w: workers){
        cout << "append worker..." << w->getId() <<  endl;
        pool->append(w, 0);
    }
    
    pause();
    return 0;
}