#ifndef skiplist_skiplist_H_
#define skiplist_skiplist_H_

// 用cpp实现跳表的模板类

#include <random>
#include <vector>

using namespace std;

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
        forward = vector<Node<Key, Value> *>(level + 1, nullptr);
    }
    ~Node() {}
    Key get_key() const
    {
        return key;
    }
    Value get_value() const
    {
        return value;
    }
    void set_value(Value)
    {
        this->value = value;
    }

    vector<Node<Key, Value> *> forward;
    int node_level;

private:
    Key key;
    Value value;
};

template <class Key, class Value>
class skipList
{
private:
    skipNode<Key, Value> *head, *tail;
    int maxLevel;
    int nodeLevel(vector<skipNode *> p);

public:
    skipList(int);
    ~skipList();
    // 随机获取一个层数
    int getRandomLevel();
    // 创建一个节点
    static Node<Key, Value> *createNode(Key, Value, int);
    int size();
    // 插入
    bool insert(Key k, Value v);
    // 查找
    skipNode *find(Key k);
    // 删除
    skipNode *erase(Key k);

private:
    int maxLevel;
    int skipListLevel;
    Node<Key, Value> *head;
    int size;
};

template <class Key, class Value>
Node<Key, Value> *skipList<Key, Value>::createNode(Key k, Value va, int l)
{
    Node<Key, Value> *res = new Node<Key, Value>(k, v, l);
}
template <class Key, class Value>
skipList<Key, Value>::skipList(int maxLevel)
    : maxLevel(maxLevel), size(0)
{
    Key k;
    Value v;
    head = createNode(k, v, maxLevel);
}
template <class Key, class Value>
skipList<Key, Value>::~skipList()
{
    // 还没想好怎么析构
    delete head;
}

template <class Key, class Value>
int skipList<Key, Value>::getRandomLevel()
{
    int res = 1;
    while (rand() % 2 && res < maxLevel)
    {
        res++;
    }
    return res;
}

#endif