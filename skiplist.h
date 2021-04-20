#ifndef skiplist_skiplist_H_
#define skiplist_skiplist_H_

// 用cpp实现跳表的模板类

#include <random>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <mutex>

// 跳表实现k-v存储

// 跳表结点
template <class Key, class Value>
class Node
{
public:
    Node() {}
    Node(const Key k, const Value v, int level)
    {
        this->key = k;
        this->value = v;
        this->node_level = level;
        forward = new Node *[level + 1];
        for (int i = 0; i <= level; ++i)
            forward[i] = nullptr;
    }
    ~Node() {}
    Key getKey() const
    {
        return key;
    }
    Value getValue() const
    {
        return value;
    }
    void setValue(Value)
    {
        this->value = value;
    }

    Node<Key, Value> **forward;
    int node_level;

private:
    Key key;
    Value value;
};

// template <class Key, class Value>
// class skipList
// {
// private:
//     Node<Key, Value> *head, *tail;
//     int maxLevel;
//     int skipListLevel;
//     int currentElement;
//     std::mutex locker;

// public:
//     static Node<Key, Value> *createNode(Key, Value, int);
//     skipList(int maxLevel = 32);
//     ~skipList();
//     // 随机获取一个层数
//     int getRandomLevel();
//     // 创建一个节点

//     int size() const
//     {
//         return currentElement;
//     }
//     // 插入
//     bool insert(const Key key, const Value value);
//     // 查找
//     Node<Key, Value> *find(Key key);
//     // 删除
//     bool erase(const Key key);
//     // 打印
//     void display();
//     void displayKey();
//     void displayValue();
// };

template <class Key, class Value>
Node<Key, Value> *skipList<Key, Value>::createNode(Key k, Value v, int l)
{
    Node<Key, Value> *res = new Node<Key, Value>(k, v, l);
    return res;
}
template <class Key, class Value>
skipList<Key, Value>::skipList(int maxLevel)
    : maxLevel(maxLevel), currentElement(0), skipListLevel(0)
{
    Key k;
    Value v;
    head = createNode(k, v, maxLevel);
    // 初始化随机种子
    srand((unsigned)time(NULL));
}
template <class Key, class Value>
skipList<Key, Value>::~skipList()
{
    Node<Key, Value> *temp = head;
    while (head)
    {
        temp = head->forward[0];
        delete head;
        head = temp;
    }
}
template <class Key, class Value>
int skipList<Key, Value>::getRandomLevel()
{
    int res = 0;
    while (rand() % 2 && res < maxLevel)
    {
        res++;
    }
    return res;
}
template <class Key, class Value>
bool skipList<Key, Value>::insert(const Key key, const Value value)
{
    locker.lock();
    Node<Key, Value> *cur = head;
    // update存储搜索路径，每一层的最右侧
    // 之后插入新的节点后，update的每个节点要指向新的节点
    Node<Key, Value> **update = new Node<Key, Value> *[maxLevel + 1];
    for (int i = 0; i <= maxLevel; ++i)
        update[i] = nullptr;

    for (int i = skipListLevel; i >= 0; --i)
    {
        // 水平搜索
        while (cur->forward[i] != nullptr && cur->forward[i]->getKey() < key)
        {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    // 已经存在key
    if (cur != nullptr && cur->getKey() == key)
    {
        locker.unlock();
        return false;
    }
    // 插入新的结点
    int randomLevel = getRandomLevel();
    if (randomLevel > skipListLevel)
    {
        for (int i = skipListLevel + 1; i <= randomLevel; ++i)
        {
            // 高出的高度由头结点指向新的结点
            update[i] = head;
        }
        skipListLevel = randomLevel;
    }
    Node<Key, Value> *insertedNode = createNode(key, value, randomLevel);
    for (int i = 0; i <= randomLevel; ++i)
    {
        insertedNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = insertedNode;
    }
    ++currentElement;
    locker.unlock();
    return true;
}
template <class Key, class Value>
Node<Key, Value> *skipList<Key, Value>::find(Key key)
{
    Node<Key, Value> *cur = head;
    for (int i = skipListLevel; i >= 0; --i)
    {
        while (cur->forward[i] != nullptr && cur->forward[i]->getKey() < key)
        {
            cur = cur->forward[i];
        }
    }

    cur = cur->forward[0];
    if (cur != nullptr && cur->getKey() == key)
    {
        return cur;
    }
    else
        return nullptr;
}
template <class Key, class Value>
bool skipList<Key, Value>::erase(const Key key)
{
    locker.lock();
    Node<Key, Value> *cur = head;
    Node<Key, Value> **update = new Node<Key, Value> *[maxLevel + 1];
    for (int i = 0; i <= maxLevel; ++i)
        update[i] = nullptr;
    for (int i = skipListLevel; i >= 0; --i)
    {
        while (cur->forward[i] != nullptr && cur->forward[i]->getKey() < key)
        {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur != nullptr && cur->getKey() == key)
    {
        for (int i = 0; i <= skipListLevel; ++i)
        {
            if (update[i]->forward[i] != cur)
                break;
            update[i]->forward[i] = cur->forward[i];
        }
        while (skipListLevel > 0 && head->forward[skipListLevel] == nullptr)
        {
            --skipListLevel;
        }
        --currentElement;
        locker.unlock();
        return true;
    }
    else
    {
        locker.unlock();
        return false;
    }
}
template <class Key, class Value>
void skipList<Key, Value>::display()
{
    std::cout << "\nSkipList:\n";
    for (int i = 0; i <= skipListLevel; ++i)
    {
        Node<Key, Value> *node = head->forward[i];
        std::cout << "Level" << i << ": ";
        while (node != NULL)
        {
            std::cout << node->getKey() << ":" << node->getValue() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}
template <class Key, class Value>
void skipList<Key, Value>::displayKey()
{
    std::cout << "\nSkipList:\n";
    for (int i = 0; i <= skipListLevel; ++i)
    {
        Node<Key, Value> *node = head->forward[i];
        std::cout << "Level" << i << ": ";
        while (node != NULL)
        {
            std::cout << node->getKey() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}
template <class Key, class Value>
void skipList<Key, Value>::displayValue()
{
    std::cout << "\nSkipList:\n";
    for (int i = 0; i <= skipListLevel; ++i)
    {
        Node<Key, Value> *node = head->forward[i];
        std::cout << "Level" << i << ": ";
        while (node != NULL)
        {
            std::cout << node->getValue() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}

#endif