#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <cstdlib>
#include <time.h>
#include "skiplist.h"


// 压力测试
#define NUM_THREADS 10
#define TEST_COUNT 1000000

skipList<int, std::string> skipL(18);

void *insertThread(void *threadId)
{
    long tid;
    tid = (long)threadId;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i)
    {
        skipL.insert(rand()%TEST_COUNT,"a");
        count++;
    }
    pthread_exit(NULL);
}

void *findThread(void *threadId)
{
    long tid;
    tid = (long)threadId;
    int tmp = TEST_COUNT / NUM_THREADS;
    for(int i=tid * tmp, count = 0;count < tmp; ++i){
        count++;
        skipL.find(rand()%TEST_COUNT)->getValue();
    }
    pthread_exit(NULL);
}

int main()
{
    time_t start = time(NULL);
    pthread_t writeThreads[10];
    int r;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        std::cout << "main() : creating thread, " << i << std::endl;
        if ((r = pthread_create(&writeThreads[i], NULL, insertThread, (void *)i)) < 0)
        {
            std::cout << "Error:unable to create thread," << r << std::endl;
            exit(-1);
        }
    }
    void *ret;
    for(int i = 0; i< NUM_THREADS; ++i){
        if(pthread_join(writeThreads[i], &ret)!=0){
            perror("pthread_create() error");
            exit(3);
        }
    }
    pthread_t readThreads[10];
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if ((r = pthread_create(&readThreads[i], NULL, insertThread, (void *)i)) < 0)
        {
            std::cout << "Error:unable to create thread," << r << std::endl;
            exit(-1);
        }
    }
    for(int i = 0; i< NUM_THREADS; ++i){
        if(pthread_join(readThreads[i], &ret)!=0){
            perror("pthread_create() error");
            exit(3);
        }
    }
    std::cout << "size: " << skipL.size() << std::endl;
    time_t end=time(NULL);
    long Time = (long)end-(long)start;
    std::cout << Time << std::endl;
    return 0;
}