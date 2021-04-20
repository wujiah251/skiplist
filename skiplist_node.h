#ifndef SKIPLIST_NODE_H_
#define SKIPLIST_NODE_H_

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
    void setValue(Value value)
    {
        this->value = value;
    }

    Node<Key, Value> **forward;
    int node_level;

private:
    Key key;
    Value value;
};

#endif