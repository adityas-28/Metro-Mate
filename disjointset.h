#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <QVector>

class DisjointSet {
private:
    QVector<int> size;
    QVector<int> parent;

public:
    DisjointSet(int n);                // Constructor
    int findUParent(int node);         // Find with path compression
    void unionBySize(int u, int v);    // Union by size
    int findSize(int n);               // Return size of component
    bool isConnected(int u, int v);    // Check if two nodes are in same set
};

#endif // DISJOINTSET_H
