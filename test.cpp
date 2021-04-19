#include "skiplist.h"
#include <iostream>

using namespace std;

int main()
{
    skipList<int,int> s;
    s.display();
    s.insert(1,1);
    s.insert(2,2);
    s.insert(3,3);
    s.insert(4,4);
    s.insert(5,5);
    s.insert(6,6);
    s.displayKey();
    return 0;
}