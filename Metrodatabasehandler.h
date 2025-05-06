#ifndef METRODATABASEHANDLER_H
#define METRODATABASEHANDLER_H

#include <QString>
#include <QMap>
#include <QSqlDatabase>

class MetroDatabaseHandler {
public:
    MetroDatabaseHandler(const QString& dbPath);
    ~MetroDatabaseHandler();

    QMap<int, QList<std::pair<int, double>>> createDelhiMetroGraph();

    void printAdjacencyList(const QMap<int, std::pair<QString, double>>& stationMap,
                            const QMap<int, QList<std::pair<int, double>>>& graph);
    QMap<int, std::pair<QString, double>> getStationCodeNameMap();

    double calculateFare(const QList<int>& path);
    QString getLine(int stationId) const;
    static MetroDatabaseHandler& instance();
    double getDistanceBetweenStations(int stationId1, int stationId2);


private:
    QSqlDatabase db;
    void openDatabase();
    void closeDatabase();
    QSqlDatabase mydb;
    QString databasePath;
    void printGraph();
    MetroDatabaseHandler();
    void openConnection();
   // static QMap<int, QList<int>> createDelhiMetroGraph();
    // void printAdjacencyList(const QMap<int, std::pair<QString, double>>& stationMap,
    //                         const QMap<int, QList<int>>& graph);

};

#endif // METRODATABASEHANDLER_H
