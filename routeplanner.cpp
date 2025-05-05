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
#include <QDebug>


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

// MetroDatabaseHandler& handler = MetroDatabaseHandler::instance();

QList<int> RoutePlanner::findShortestTimePath(int src, int dest) {

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


    for (auto key : stationMap.keys()) dist[key] = INF;
    dist[src] = 0.0;

    pq.push({0.0, src});

    while (!pq.empty()) {
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
    for (int at = dest; at != src; at = parent[at]) path.prepend(at);
    path.prepend(src);
    return path;
}

QList<int> RoutePlanner::findShortestDistancePath(int src, int dest) {

    qDebug() << "Finding path from " << src << " to " << dest;


    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();

    QMap<int, QList<int>> graph = handler.createDelhiMetroGraph();

    // QPriorityQueue<std::pair<double, int>,
    //                QVector<std::pair<double, int>>,
    //                std::greater<>> pq;
    const double INF = 1e9;
    QMap<int, double> dist;
    QMap<int, int> parent;
    QSet<int> visited;

    for (auto key : stationMap.keys()) dist[key] = INF;
    dist[src] = 0.0;

    std::priority_queue<
        std::pair<double, int>,
        std::vector<std::pair<double, int>>,
        std::greater<>
        > pq;

    pq.push({0.0, src});

    // int i = 0;
    while (!pq.empty()) {
        qDebug() << "inside calculateShortestPath";
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

    QList<int> path;
    for (int at = dest; at != src; at = parent[at]) path.prepend(at);
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

    while (!q.isEmpty()) {
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

    QList<int> path;
    for (int at = dest; at != src; at = parent[at]) path.prepend(at);
    path.prepend(src);
    return path;
}
