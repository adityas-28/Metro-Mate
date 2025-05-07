#include "farepage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QSqlQuery>
#include <QDir>
#include "MetroDatabaseHandler.h"
#include "routeplanner.h"
#include <QSqlError>
#include <iomanip>

FarePage::FarePage(QWidget *parent)
    : QWidget(parent)
{
    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();

    qDebug() << "Total stations:" << stationMap.size();
    for (auto it = stationMap.begin(); it != stationMap.end(); ++it) {
        qDebug() << "Code:" << it.key() << ", Name:" << it.value().first << ", dist : " << QString::number(it.value().second, 'f', 1);;
    }

    // Assuming you're inside main() or any function:
    //  MetroDatabaseHandler dbHandler;

    //  QMap<int, std::pair<QString, double>> stationMap = dbHandler.getStationCodeNameMap();
    QMap<int, QList<std::pair<int, double>>> graph = handler.createDelhiMetroGraph();  // You must have a method or way to access the graph

    handler.printAdjacencyList(stationMap, graph);

    setWindowTitle("Delhi Metro Fare & Route");
    setStyleSheet("background-color: #1e1e1e; color: #FFFFFF;"); // Dark theme

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *title = new QLabel("🛤 Plan Your Journey", this);
    title->setStyleSheet("font-size: 42px; font-weight: bold; font-family: 'Segoe UI'; color: #FFFFFF;");
    mainLayout->addWidget(title, 0, Qt::AlignHCenter);

    // Station input section
    QHBoxLayout *stationLayout = new QHBoxLayout();
    stationLayout->setSpacing(20);

    QLabel *fromLabel = new QLabel("📍From :", this);
    QLabel *toLabel = new QLabel("🏁To :", this);
    QString labelStyle = "font-size: 18px; color: white;";

    fromLabel->setStyleSheet(labelStyle);
    toLabel->setStyleSheet(labelStyle);

    sourceStation = new QComboBox(this);
    destinationStation = new QComboBox(this);

    sourceStation->addItem("Select Source Station");
    destinationStation->addItem("Select Destination Station");

    QList<QComboBox*> comboBoxes = {sourceStation, destinationStation};
    for (auto box : comboBoxes) {
        box->setStyleSheet(R"(
            QComboBox {
                font-size: 14px;
                padding: 6px;
                background-color: #333;
                color: white;
                border: 1px solid #555;
                border-radius: 4px;
            }
            QComboBox QAbstractItemView {
                background-color: #333;
                color: white;
                selection-background-color: #555;
            }
        )");
        box->setMinimumWidth(200);
        box->setMinimumHeight(40);
    }

    stationLayout->addWidget(fromLabel);
    stationLayout->addWidget(sourceStation);
    stationLayout->addWidget(toLabel);
    stationLayout->addWidget(destinationStation);
    mainLayout->addLayout(stationLayout);

    // Get Fare button
    getFareButton = new QPushButton("Get Fare and Route", this);
    getFareButton->setStyleSheet(R"(
        QPushButton {
            font-size: 15px;
            font-weight: bold;
            background-color: #4CAF50;
            color: white;
            padding: 5px 10px;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
    )");
    getFareButton->setMinimumWidth(150);
    getFareButton->setMinimumHeight(36);
    mainLayout->addWidget(getFareButton, 0, Qt::AlignCenter);

    // Back button
    backBtn = new QPushButton("Back", this);
    backBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 15px;
            font-weight: bold;
            background-color: #FF6347;
            color: white;
            padding: 5px 10px;
            border-radius: 6px;
        }
        QPushButton:hover {
            background-color: #FF4500;
        }
    )");
    backBtn->setMinimumWidth(150);
    backBtn->setMinimumHeight(36);
    mainLayout->addWidget(backBtn, 0, Qt::AlignCenter);

    connect(backBtn, &QPushButton::clicked, this, &FarePage::goBackToWelcomePage);

    // Filter dropdown

    // Step 1: Create filterDropdown first
    filterDropdown = new QComboBox(this);
    filterDropdown->addItem("Minimum Interchange");
    filterDropdown->addItem("Minimum Time");
    filterDropdown->addItem("Minimum Distance");
    filterDropdown->setStyleSheet(R"(
    QComboBox {
        font-size: 14px;
        padding: 3px;
        margin: 2px, 1px, 1px, 1px;
        background-color: #333;
        color: white;
        border-radius: 2px;
    }
)");

    filterDropdown->setMinimumWidth(200);
    filterDropdown->setMinimumHeight(40);

    // Step 2: Create label
    QLabel *filterLabel = new QLabel("🔎 Advanced Filters :", this);
    filterLabel->setStyleSheet(labelStyle);
    // filterLabel->setStyleSheet("color: white; font-size: 14px;");

    // Step 3: Create layout and add both label and dropdown
    QVBoxLayout *filterLayout = new QVBoxLayout();
    filterLayout->addWidget(filterLabel, 0, Qt::AlignCenter);
    filterLayout->addWidget(filterDropdown, 0, Qt::AlignCenter);

    // Step 4: Add the layout to main layout
    mainLayout->addLayout(filterLayout);


    // Info labels
    fareLabel = new QLabel("💰 Fare: ₹ --", this);
    timeLabel = new QLabel("⏱ Estimated Time: -- mins", this);
    interchangeLabel = new QLabel("🔁 Interchanges: --", this);
    QString infoLabelStyle = "font-size: 15px; color: white; padding: 4px;";
    fareLabel->setStyleSheet(infoLabelStyle);
    timeLabel->setStyleSheet(infoLabelStyle);
    interchangeLabel->setStyleSheet(infoLabelStyle);

    mainLayout->addWidget(fareLabel);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(interchangeLabel);

    // Route section
    QLabel *routeLabel = new QLabel("📌 Route Details:", this);
    routeLabel->setStyleSheet("font-size: 17px; font-weight: 600; color: white;");
    mainLayout->addWidget(routeLabel);

    routeDisplay = new QTextEdit(this);
    routeDisplay->setReadOnly(true);
    routeDisplay->setPlaceholderText("Stations & interchanges will appear here...");
    routeDisplay->setStyleSheet("font-size: 14px; background-color: #2a2a2a; color: white; padding: 8px;");
    routeDisplay->setMinimumHeight(100);
    mainLayout->addWidget(routeDisplay);


    mydb = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + "/data.db";

    mydb.setDatabaseName(dbPath);
    //qDebug() << "Trying to open database at:" << dbPath;

    if (!mydb.open()) {
        qDebug() << "Database failed to open: " << mydb.lastError().text();
    } else {
        qDebug() << "Database opened successfully in login page.";
    }

    planner = new RoutePlanner();
    connect(getFareButton, &QPushButton::clicked, this, &FarePage::calculateRoute);

    populateStations();

}

void FarePage::populateStations() {
    QSqlQuery query("SELECT \"Station_Name\" FROM metro_database ORDER BY \"Station_Name\"");

    if (!query.exec()) {
        qDebug() << "Failed to fetch station names:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QString station = query.value(0).toString();
        sourceStation->addItem(station);
        destinationStation->addItem(station);
    }
}

void FarePage::calculateRoute() {
    MetroDatabaseHandler handler("data.db");
    QMap<int, std::pair<QString, double>> stationMap = handler.getStationCodeNameMap();
    QMap<int, QList<std::pair<int, double>>> graph = handler.createDelhiMetroGraph();

    QString source = sourceStation->currentText();
    QString destination = destinationStation->currentText();

    if (source == "Select Source Station" || destination == "Select Destination Station" || source == destination) {
        routeDisplay->setPlainText("⚠️ Please select valid and distinct stations.");
        return;
    }

    int srcId = -1, destId = -1;
    for (auto it = stationMap.begin(); it != stationMap.end(); ++it) {
        if (it.value().first == source) srcId = it.key();
        if (it.value().first == destination) destId = it.key();
    }

    if (srcId == -1 || destId == -1) {
        routeDisplay->setPlainText("❌ Station not found in the database.");
        return;
    }

    // Get selected filter
    QString selectedFilter = filterDropdown->currentText();
    QList<int> path;

    if (selectedFilter == "Minimum Distance") {
        // path = planner->findShortestDistancePath(srcId, destId);
        path = planner->findMinInterchangePath(srcId, destId);
    } else if (selectedFilter == "Minimum Interchange") {
        // path = planner->findMinInterchangePath(srcId, destId);
        path = planner->findShortestDistancePath(srcId, destId);
    } else if (selectedFilter == "Minimum Time") {
        //path = planner->findShortestTimePath(srcId, destId);
        path = planner->findMinInterchangePath(srcId, destId);
    } else {
        // path = planner->findShortestDistancePath(srcId, destId);
        path = planner->findMinInterchangePath(srcId, destId);
    }

    if (path.isEmpty()) {
        routeDisplay->setPlainText("🚫 No route found between selected stations.");
        return;
    }

    QMap<QString, QString> lineColors = {
        {"Blue", "#90D5FF"},
        {"Yellow", "#FFD700"},
        {"Voilet", "#8A2BE2"},
        {"Red", "#FF0000"},
        {"Green", "#008000"},
        {"Magenta", "#FF00FF"},
        {"Orange", "#FFA500"},
        {"Pink", "#FF69B4"},
        {"Grey", "#A9A9A9"},
        {"Aqua", "#00FFFF"}
        // Add more if needed
    };

    QStringList stationDisplay;
    double totalDistance = 0.0;
    QString previousLine = planner->getLine(path[0]).first;

    for (int i = 0; i < path.size(); ++i) {
        auto linePair = planner->getLine(path[i]);
        QString stationName = stationMap[path[i]].first;

        QString rawLineName = linePair.first;
        QString baseLineColor = rawLineName.split(" ").first().trimmed();
        QString color = lineColors.value(baseLineColor, "#ffffff");

        QString formattedName = "<span style='color:" + color + ";'>" + stationName + "</span>";
        stationDisplay.append(formattedName);

        if (i > 0){
            if(planner->getLine(path[i - 1]).first.split(" ").first().trimmed() != planner->getLine(path[i]).first.split(" ").first().trimmed()) continue;
            else totalDistance += handler.getDistanceBetweenStations(path[i - 1], path[i]);
        }
    }

    routeDisplay->setHtml("🛤 <b>Route:</b> " + stationDisplay.join(" ➡ "));

    // Calculate extras
    int interchanges = planner->calculateInterchanges(path);
    double fare = handler.calculateFare(path);
    double time = totalDistance * 1.32; // assuming 45 km/h = 1.3 min/km

    fareLabel->setText("💰 Fare: ₹ " + QString::number(fare, 'f', 0));
    timeLabel->setText("⏱ Estimated Time: " + QString::number(time, 'f', 0) + " mins");
    interchangeLabel->setText("🔁 Interchanges: " + QString::number(interchanges));
}

double MetroDatabaseHandler::getDistanceBetweenStations(int stationId1, int stationId2) {
    QSqlQuery query;
    query.prepare("SELECT distance FROM metro_database WHERE Station_Code = :stationId1;");
    query.bindValue(":stationId1", stationId1);

    if (!query.exec()) {
        qDebug() << "Error executing query for stationId1:" << query.lastError().text();
        return 0;
    }

    double d1, d2;

    if (query.next()) {
        d1 = query.value(0).toDouble();
    } else {
        return 0;
    }

    QSqlQuery query2;
    query2.prepare("SELECT distance FROM metro_database WHERE Station_Code = :stationId2;");
    query2.bindValue(":stationId2", stationId2);

    if (!query2.exec()) {
        qDebug() << "Error executing query for stationId2:" << query2.lastError().text();
        return 0;
    }

    if (query2.next()) {
        d2 = query2.value(0).toDouble();
    } else {
        return 0;
    }

    return std::abs(d1 - d2); // not working correctly
    // return 2.15;
}

int RoutePlanner::calculateInterchanges(const QList<int>& path) {
    if (path.size() <= 2) return 0;

    int interchanges = 0;

    auto prevNode = getLine(path[0]);
    QString previousLine = prevNode.first;
    QString previousName = prevNode.second;

    for (int i = 1; i < path.size(); ++i) {
        auto currNode = getLine(path[i]);
        QString currentLine = currNode.first;
        QString currentName = currNode.second;

        if ((i == 1 || i == path.size() - 1) && (currentName == previousName)) {
            previousLine = currentLine;
            continue;
        }

        if (currentLine != previousLine) {
            interchanges++;
            previousLine = currentLine;
        }
    }

    return interchanges;
}

int RoutePlanner::calculatePathLength(const QList<int>& path) {
    return path.size();
}

double MetroDatabaseHandler::calculateFare(const QList<int>& path) {
    RoutePlanner *planner;
    int pathLength = planner->calculatePathLength(path);
    if (pathLength <= 3) {
        return 10.0;
    } else if (pathLength <= 6) {
        return 20.0;
    } else if (pathLength <= 9) {
        return 30.0;
    } else if (pathLength <= 12) {
        return 40.0;
    } else if (pathLength <= 15) {
        return 50.0;
    } else {
        return 60.0;
    }
}

std::pair<QString, QString> RoutePlanner::getLine(int stationId) {
    QSqlQuery query;
    query.prepare("SELECT Line, station_name FROM metro_database WHERE Station_Code = :stationId");
    query.bindValue(":stationId", stationId);

    if (!query.exec()) {
      //  qDebug() << "Error executing queryy: " << query.lastError().text();
        return std::make_pair("", ""); // Return an empty string on error.
    }
    if (query.next()){
        QString line = query.value(0).toString();
        QString name = query.value(1).toString();
        QString strippedName = name.section(" [Conn:", 0, 0).trimmed();
        return std::make_pair(line, strippedName);
    }
    else
        return std::make_pair("", "");
}

QString getLine(int stationId) {
    QSqlQuery query;
    query.prepare("SELECT line FROM stations WHERE id = ?");
    query.addBindValue(stationId);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}
