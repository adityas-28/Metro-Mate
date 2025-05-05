#ifndef DISJOINTSET_H
#define DISJOINTSET_H

#include <QVector>

class DisjointSet {
private:
    QVector<int> size, parent;

public:
    DisjointSet(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);

        for (int i = 0; i <= n; i++) {
            parent[i] = i;
        }
    }

    int findUParent(int node) {
        if (parent[node] == node) return node;
        return parent[node] = findUParent(parent[node]);
    }

    void unionBySize(int u, int v) {
        int u_p = findUParent(u);
        int v_p = findUParent(v);
        if (u_p == v_p) return;

        if (size[u_p] > size[v_p]) {
            parent[v_p] = u_p;
            size[u_p] += size[v_p];
        } else {
            parent[u_p] = v_p;
            size[v_p] += size[u_p];
        }
    }

    int findSize(int n) {
        return size[findUParent(n)];
    }

    bool isConnected(int u, int v) {
        return findUParent(u) == findUParent(v);
    }
};

#endif // DISJOINTSET_H
