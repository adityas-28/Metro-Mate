#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <QVector>

class DisjointSet {
private:
    QVector<int> size;
    QVector<int> parent;

public:
    DisjointSet(int n);
    int findUParent(int node);
    void unionBySize(int u, int v);
    int findSize(int n);
    bool isConnected(int u, int v);
};

#endif // DISJOINTSET_H
