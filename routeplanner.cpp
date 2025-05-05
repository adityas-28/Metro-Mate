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
    int counter = 0;

    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();

    QMap<int, QList<int>> graph = handler.createDelhiMetroGraph();

    // QPriorityQueue<std::pair<double, int>,
    //                QVector<std::pair<double, int>>,
    //                std::greater<>> pq;
    const double speed = 30.0; // km/h
    const double INF = 1e9;
    QMap<int, double> dist;
    QMap<int, int> parent;
    QSet<int> visited;

    std::priority_queue<
        std::pair<double, int>,
        std::vector<std::pair<double, int>>,
        std::greater<>
        > pq;

    bool flag = true;

    for (auto key : stationMap.keys()) dist[key] = INF;
    dist[src] = 0.0;

    pq.push({0.0, src});

    while (!pq.empty()) {
        counter++;
        if(counter > 300) {
            flag = false;
            break;
        }
        auto [time, u] = pq.top(); pq.pop();
        if (visited.contains(u)) continue;
        visited.insert(u);

        for (int v : graph[u]) {
            double weight = stationMap[v].second;  // distance in km
            double t = weight / speed;
            if (dist[u] + t < dist[v]) {
                dist[v] = dist[u] + t;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    QList<int> path;
    counter = 0;
    if(!flag) {
        return path;
    }
    for (int at = dest; at != src; at = parent[at]) path.prepend(at);
    path.prepend(src);
    return path;
}
QList<int> RoutePlanner::findShortestDistancePath(int src, int dest) {
    qDebug() << "Finding path from " << src << " to " << dest;

    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();
    QMap<int, QList<int>> graph = handler.createDelhiMetroGraph();

    const double INF = 1e9;
    QMap<int, double> dist;
    QMap<int, int> parent;
    QSet<int> visited;

    // Step 1: Build Disjoint Set
    DisjointSet ds(1000); // assume max node ID < 1000
    for (auto u : graph.keys()) {
        for (int v : graph[u]) {
            ds.unionBySize(u, v);
        }
    }

    // Step 2: Check connectivity
    if (!ds.isConnected(src, dest)) {
        qDebug() << "Stations are not connected!";
        QMessageBox::warning(nullptr, "Path Error", "Selected stations are not connected!");
        return {};
    }

    // Step 3: Dijkstra's algorithm
    for (auto key : stationMap.keys()) dist[key] = INF;
    dist[src] = 0.0;

    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;
    pq.push({0.0, src});

    int counter = 0;
    bool flag = true;

    while (!pq.empty()) {
        counter++;
        if (counter > 1000) {
            flag = false;
            break;
        }

        auto [d, u] = pq.top(); pq.pop();
        if (visited.contains(u)) continue;
        visited.insert(u);

        for (int v : graph[u]) {
            double weight = stationMap[v].second;
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Step 4: Construct path
    QList<int> path;
    if (!flag || !parent.contains(dest)) {
        qDebug() << "No valid path found.";
        QMessageBox::warning(nullptr, "Path Error", "No valid path found between the stations.");
        return {};
    }

    for (int at = dest; at != src; at = parent[at]) {
        if (!parent.contains(at)) {
            QMessageBox::warning(nullptr, "Path Error", "Path reconstruction failed.");
            return {};
        }
        path.prepend(at);
    }

    path.prepend(src);
    return path;
}

QList<int> RoutePlanner::findMinInterchangePath(int src, int dest) {

    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();

    QMap<int, QList<int>> graph = handler.createDelhiMetroGraph();

    // QPriorityQueue<std::pair<double, int>,
    //                QVector<std::pair<double, int>>,
    //                std::greater<>> pq;
    QMap<int, int> parent;
    QSet<int> visited;
    QQueue<int> q;

    visited.insert(src);
    q.enqueue(src);

    bool flag = true;
    int counter = 0;
    while (!q.isEmpty()) {
        counter++;
        if(counter > 300) {
            flag = false;
            break;
        }
        int u = q.dequeue();
        if (u == dest) break;
        for (int v : graph[u]) {
            if (!visited.contains(v)) {
                visited.insert(v);
                parent[v] = u;
                q.enqueue(v);
            }
        }
    }
    counter = 0;
    QList<int> path;
    if(!flag) {
        return path;
    }
    for (int at = dest; at != src; at = parent[at]) path.prepend(at);
    path.prepend(src);
    return path;
}
