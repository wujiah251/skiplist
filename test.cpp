#include <iostream>
#include <string>
#include <pthread.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include "skiplist.h"

using namespace std::chrono;

// 压力测试
#define NUM_THREADS 10
#define TEST_COUNT 1000000

skipList<int, std::string>
    skipL(18);

void *insertThread(void *threadId)
{
    long tid;
    tid = (long)threadId;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++count, ++i)
    {
        skipL.insert(i, "a");
    }
    pthread_exit(NULL);
    return NULL;
}

void *findThread(void *threadId)
{
    long tid;
    tid = (long)threadId;
    int tmp = TEST_COUNT / NUM_THREADS;
    for (int i = tid * tmp, count = 0; count < tmp; ++i)
    {
        count++;
        auto res = skipL.find(i);
        if (res != nullptr)
            res->getValue();
    }
    pthread_exit(NULL);
    return NULL;
}

int main()
{
    system_clock::time_point nowStart = system_clock::now();            // 获取当前时间点
    system_clock::duration durationStart = nowStart.time_since_epoch(); // 从1970-01-01 00:00:00到当前时间点的时长
    time_t start = duration_cast<milliseconds>(durationStart).count();  // 将时长转换为微秒数
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
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if (pthread_join(writeThreads[i], &ret) != 0)
        {
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
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        if (pthread_join(readThreads[i], &ret) != 0)
        {
            perror("pthread_create() error");
            exit(3);
        }
    }
    system_clock::time_point nowEnd = system_clock::now();          // 获取当前时间点
    system_clock::duration durationEnd = nowEnd.time_since_epoch(); // 从1970-01-01 00:00:00到当前时间点的时长
    time_t end = duration_cast<milliseconds>(durationEnd).count();  // 将时长转换为微秒数
    long Time = (long)end - (long)start;
    std::cout << "总共花费时间：" << Time << "微秒" << std::endl;
    return 0;
}