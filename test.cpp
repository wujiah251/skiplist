#include <iostream>
#include <string>
#include <pthread.h>
// #include "skiplist.h"

using namespace std;

// 压力测试
#define NUM_THREADS 10
#define TEST_COUNT 10000

void *insertThread(void *threadId)
{
    long tid;
    tid = (long)threadId;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i)
    {
        for (int i = tid * tmp, count = 0; count < tmp; ++i)
        {
            count++;
        }
    }
}

int main()
{
    pthread_t threads[10];
    int r;
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if (r = pthread_create(&threads[i], NULL, insertThread, (void *)i) < 0)
        {
            std::cout << "Error:unable to create thread," << r << std::endl;
            exit(-1);
        }
    }
    void *ret;
    return 0;
}