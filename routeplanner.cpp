#include "routeplanner.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSet>
#include <QQueue>
#include "MetroDatabaseHandler.h"
#include <QMap>
#include <queue>
#include <vector>
#include <QQueue>
#include "DisjointSet.h"
#include <QDebug>
#include <QMessageBox>

RoutePlanner::RoutePlanner() {
    loadGraphFromDB();
}

void RoutePlanner::loadGraphFromDB() {
    QSqlQuery query("SELECT Source, Destination, Distance FROM metro_edges");

    if (!query.exec()) {
        qDebug() << "Error loading graph:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString from = query.value(0).toString();
        QString to = query.value(1).toString();
        int distance = query.value(2).toInt();

        adjList[from].append(qMakePair(to, distance));
        adjList[to].append(qMakePair(from, distance)); // bidirectional
    }
}

DisjointSet::DisjointSet(int n) {
    parent.resize(n + 1);
    size.resize(n + 1, 1);

    for (int i = 0; i <= n; i++) {
        parent[i] = i;
    }
}

int DisjointSet::findUParent(int node) {
    if (parent[node] == node)
        return node;
    return parent[node] = findUParent(parent[node]);
}

void DisjointSet::unionBySize(int u, int v) {
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

int DisjointSet::findSize(int n) {
    return size[findUParent(n)];
}

bool DisjointSet::isConnected(int u, int v) {
    return findUParent(u) == findUParent(v);
}


// MetroDatabaseHandler& handler = MetroDatabaseHandler::instance();
QList<int> RoutePlanner::findShortestTimePath(int src, int dest) {
    qDebug() << "Finding path from " << src << " to " << dest;

    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();
    QMap<int, QList<std::pair<int, double>>> graph = handler.createDelhiMetroGraph();

    const double INF = 1e9;
    QMap<int, double> dist;
    QMap<int, int> parent;
    QSet<int> visited;

    // Disjoint Set: Check if src and dest are connected
    DisjointSet ds(1000);
    for (auto u : graph.keys()) {
        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            ds.unionBySize(u, v);
        }
    }

    if (!ds.isConnected(src, dest)) {
        qDebug() << "Stations are not connected!";
        QMessageBox::warning(nullptr, "Path Error", "Selected stations are not connected!");
        return {};
    }

    // Dijkstra's algorithm
    for (auto key : stationMap.keys()) dist[key] = INF;
    dist[src] = 0.0;

    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (visited.contains(u)) continue;
        visited.insert(u);

        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            double weight = neighbor.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct path safely
    QList<int> path;
    if (src == dest) {
        path.append(src);
        return path;
    }

    if (!parent.contains(dest)) {
        qDebug() << "No valid path found.";
        QMessageBox::warning(nullptr, "Path Error", "No valid path found between the stations.");
        return {};
    }

    for (int at = dest; at != src;) {
        path.prepend(at);
        if (!parent.contains(at)) {
            qDebug() << "Failed to reconstruct path at station " << at;
            QMessageBox::warning(nullptr, "Path Error", "Path reconstruction failed.");
            return {};
        }
        at = parent[at];
    }

    path.prepend(src);
    return path;
}

QList<int> RoutePlanner::findShortestDistancePath(int src, int dest) {
    qDebug() << "Finding path from " << src << " to " << dest;

    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();
    QMap<int, QList<std::pair<int, double>>> graph = handler.createDelhiMetroGraph();

    const double INF = 1e9;
    QMap<int, double> dist;
    QMap<int, int> parent;
    QSet<int> visited;

    // Disjoint Set: Check if src and dest are connected
    DisjointSet ds(1000);
    for (auto u : graph.keys()) {
        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            ds.unionBySize(u, v);
        }
    }

    if (!ds.isConnected(src, dest)) {
        qDebug() << "Stations are not connected!";
        QMessageBox::warning(nullptr, "Path Error", "Selected stations are not connected!");
        return {};
    }

    // Dijkstra's algorithm
    for (auto key : stationMap.keys()) dist[key] = INF;
    dist[src] = 0.0;

    using PDI = std::pair<double, int>;
    std::priority_queue<PDI, std::vector<PDI>, std::greater<>> pq;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (visited.contains(u)) continue;
        visited.insert(u);

        for (const auto& neighbor : graph[u]) {
            int v = neighbor.first;
            double weight = neighbor.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Reconstruct path safely
    QList<int> path;
    if (src == dest) {
        path.append(src);
        return path;
    }

    if (!parent.contains(dest)) {
        qDebug() << "No valid path found.";
        QMessageBox::warning(nullptr, "Path Error", "No valid path found between the stations.");
        return {};
    }

    for (int at = dest; at != src;) {
        path.prepend(at);
        if (!parent.contains(at)) {
            qDebug() << "Failed to reconstruct path at station " << at;
            QMessageBox::warning(nullptr, "Path Error", "Path reconstruction failed.");
            return {};
        }
        at = parent[at];
    }

    path.prepend(src);
    return path;
}

QList<int> RoutePlanner::findMinInterchangePath(int src, int dest) {
    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();
    QMap<int, QList<std::pair<int, double>>> graph = handler.createDelhiMetroGraph();

    QMap<int, int> parent;
    QSet<int> visited;
    QQueue<int> q;

    visited.insert(src);
    q.enqueue(src);

    bool found = false;

    while (!q.isEmpty()) {
        int u = q.dequeue();
        if (u == dest) {
            found = true;
            break;
        }

        for (auto v0 : graph[u]) {
            int v = v0.first;
            if (!visited.contains(v)) {
                visited.insert(v);
                parent[v] = u;
                q.enqueue(v);
            }
        }
    }

    QList<int> path;

    if (src == dest) {
        path.append(src);
        return path;
    }

    if (!found || !parent.contains(dest)) {
        qDebug() << "No path found between" << src << "and" << dest;
        QMessageBox::warning(nullptr, "Path Error", "No valid path found between the stations.");
        return {};
    }

    // Reconstruct path safely
    for (int at = dest; at != src; ) {
        path.prepend(at);
        if (!parent.contains(at)) {
            qDebug() << "Path reconstruction failed at station" << at;
            QMessageBox::warning(nullptr, "Path Error", "Path reconstruction failed.");
            return {};
        }
        at = parent[at];
    }

    path.prepend(src);
    return path;
}
