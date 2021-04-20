#include "skiplist.h"
#include <iostream>
#include "pthread.h"

using namespace std;

// 压力测试

int main()
{
    skipList<int, int> s;
    s.display();
    s.insert(1, 1);
    s.insert(2, 2);
    s.insert(3, 3);
    s.insert(4, 4);
    s.insert(5, 5);
    s.insert(6, 6);
    s.displayKey();
    return 0;
}