#ifndef skiplist_skiplist_H_
#define skiplist_skiplist_H_

// 用cpp实现跳表的模板类

#include <random>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <mutex>

template <class Key, class Value>
class skipNode
{
public:
    skipNode() {}
    skipNode(const Key k, const Value v, int level)
    {
        this->key = k;
        this->value = v;
        this->node_level = level;
        forward = new skipNode *[level + 1];
        for (int i = 0; i <= level; ++i)
            forward[i] = nullptr;
    }
    ~skipNode() {}
    Key getKey() const
    {
        return key;
    }
    Value getValue() const
    {
        return value;
    }
    void setValue(Value value)
    {
        this->value = value;
    }

    skipNode **forward;
    int node_level;

private:
    Key key;
    Value value;
};


// 跳表实现k-v存储

template <class Key, class Value>
class skipList
{
    typedef skipNode<Key, Value> Node;
    typedef skipNode<Key, Value>* NodePtr;

private:
    NodePtr head;
    int maxLevel;
    int skipListLevel;
    int currentElement;
    std::mutex locker;

public:
    static NodePtr createNode(Key k, Value v, int l);
    skipList(int maxLevel = 32);
    ~skipList();
    // 随机获取一个层数
    int getRandomLevel();
    // 创建一个节点

    int size() const
    {
        return currentElement;
    }
    // 插入
    bool insert(const Key key, const Value value);
    // 查找
    NodePtr find(Key key);
    // 删除
    bool erase(const Key key);
    // 打印
    void display();
    void displayKey();
    void displayValue();
};

template <class Key, class Value>
typename skipList<Key, Value>::NodePtr skipList<Key, Value>::createNode(Key k, Value v, int l)
{
    NodePtr res = new Node(k, v, l);
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
    NodePtr temp = head;
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
    NodePtr cur = head;
    // update存储搜索路径，每一层的最右侧
    // 之后插入新的节点后，update的每个节点要指向新的节点
    NodePtr *update = new NodePtr [maxLevel + 1];
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
    NodePtr insertedNode = createNode(key, value, randomLevel);
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
typename skipList<Key, Value>::NodePtr skipList<Key, Value>::find(Key key)
{
    NodePtr cur = head;
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
    NodePtr cur = head;
    NodePtr *update = new NodePtr [maxLevel + 1];
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
        NodePtr node = head->forward[i];
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
        Node *node = head->forward[i];
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
        NodePtr node = head->forward[i];
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