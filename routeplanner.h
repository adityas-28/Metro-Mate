#ifndef ROUTEPLANNER_H
#define ROUTEPLANNER_H

#include <QString>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QStringList>

class RoutePlanner {
public:
    RoutePlanner();
    void loadGraphFromDB();
    QStringList findShortestPath(const QString &source, const QString &destination, int &totalDistance);
    QList<int> findShortestTimePath(int src, int dest);        // Dijkstra with time weights
    QList<int> findShortestDistancePath(int src, int dest);    // Dijkstra with distance weights
    QList<int> findMinInterchangePath(int src, int dest);      // BFS
    int calculateInterchanges(const QList<int>& path);
    std::pair<QString, QString> getLine(int stationId);
    int calculatePathLength(const QList<int>& path);

private:
    QMap<QString, QVector<QPair<QString, int>>> adjList; // station -> [(neighbor, distance)]

};

#endif // ROUTEPLANNER_H
