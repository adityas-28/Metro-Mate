#ifndef METRODATABASEHANDLER_H
#define METRODATABASEHANDLER_H

#include <QString>
#include <QMap>
#include <QSqlDatabase>

class MetroDatabaseHandler {
public:
    MetroDatabaseHandler(const QString& dbPath);
    ~MetroDatabaseHandler();

    static QMap<int, QList<int>> createDelhiMetroGraph();

    void printAdjacencyList(const QMap<int, std::pair<QString, double>>& stationMap,
                            const QMap<int, QList<int>>& graph);
    QMap<int, std::pair<QString, double>> getStationCodeNameMap();

    double calculateFare(double distance);
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
    MetroDatabaseHandler();  // private constructor

    void openConnection();
   // static QMap<int, QList<int>> createDelhiMetroGraph();
    // void printAdjacencyList(const QMap<int, std::pair<QString, double>>& stationMap,
    //                         const QMap<int, QList<int>>& graph);

};

#endif // METRODATABASEHANDLER_H
