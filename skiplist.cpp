#include "skiplist.h"

// 静态成员函数，用于创建节点
template <class T>
typename skipList<T>::skipNode *skipList<T>::createSkipNode(int level, double score, T obj)
{
    skipNode *res = (skipNode *)malloc(sizeof(skipNode) + level * (sizeof(typename skipNode::skipNodeLevel)));
    res->score = score;
    res->value = obj;
    return res;
}

template <class T>
skipList<T>::skipList()
    : level(1), length(0), header(createSkipNode(SKIPLIST_MAXLEVEL, 0, NULL))
{
    for (int i = 0; i < SKIPLIST_MAXLEVEL; ++i)
    {
        header->level[i].forward = NULL;
        header->level[i].span = 0;
    }
    header->backward = NULL;
    tail = NULL;
}
template <class T>
skipList<T>::~skipList()
{
    skipNode *node = this->header->level[0].forward, *next;
    free(this->header);
    while (node)
    {
        next = node->level[0].forward;
        freeNode(node);
        node = next;
    }
}