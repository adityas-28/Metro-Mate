#include "MetroDatabaseHandler.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCoreApplication>
#include <QDebug>
#include <QMap>
#include <QList>
#include <QString>
#include <Qdir>

MetroDatabaseHandler::MetroDatabaseHandler(const QString& dbPath)
    : databasePath(dbPath)
{
    openDatabase();
}

MetroDatabaseHandler::~MetroDatabaseHandler() {
    closeDatabase();
}

void MetroDatabaseHandler::openDatabase() {
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + "/data.db";

    mydb.setDatabaseName(dbPath);
    //qDebug() << "Trying to open database at:" << dbPath;

    if (!mydb.open()) {
        qDebug() << "Database failed to open: " << mydb.lastError().text();
    } else {
        qDebug() << "Database opened successfully in login page.";
    }
}

void MetroDatabaseHandler::closeDatabase() {
//     if (mydb.isOpen())
//         mydb.close();
}

QMap<int, std::pair<QString, double>> MetroDatabaseHandler::getStationCodeNameMap() {
    QMap<int, std::pair<QString, double>> stationMap;

    QSqlQuery query("SELECT Station_Code, Station_Name, distance FROM metro_database");
    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return stationMap;
    }

    while (query.next()) {
        int code = query.value(0).toInt();
        QString name = query.value(1).toString();
        double dist = query.value(2).toDouble();
        stationMap[code] = {name, dist};
    }

    return stationMap;
}


QMap<int, QList<int>> MetroDatabaseHandler::createDelhiMetroGraph() {
    QMap<int, QList<int>> graph;

    // BLUE LINE CONNECTIONS
    graph[1].append(2);
    graph[2].append(1);
    graph[2].append(3);
    graph[3].append(2);
    graph[3].append(4);
    graph[4].append(3);
    graph[4].append(5);
    graph[5].append(4);
    graph[5].append(6);
    graph[6].append(5);
    graph[6].append(7);
    graph[7].append(6);
    graph[7].append(8);
    graph[8].append(7);
    graph[8].append(9);
    graph[9].append(8);
    graph[9].append(10);
    graph[10].append(9);
    graph[10].append(11);
    graph[11].append(10);
    graph[11].append(12);
    graph[12].append(11);
    graph[12].append(13);
    graph[13].append(12);
    graph[13].append(14);
    graph[13].append(92); // Kashmere Gate
    graph[14].append(13);
    graph[14].append(15);
    graph[15].append(14);
    graph[15].append(16);
    graph[16].append(15);
    graph[16].append(17);
    graph[17].append(16);
    graph[17].append(18);
    graph[18].append(17);
    graph[18].append(19);
    graph[19].append(18);
    graph[19].append(20);
    graph[20].append(19);
    graph[20].append(21);
    graph[21].append(20);
    graph[21].append(22);
    graph[22].append(21);
    graph[22].append(23);
    graph[23].append(22);
    graph[23].append(24);
    graph[24].append(23);
    graph[24].append(25);
    graph[25].append(24);
    graph[25].append(26);
    graph[26].append(25);
    graph[26].append(27);
    graph[27].append(26);
    graph[27].append(28);
    graph[28].append(27);
    graph[28].append(29);
    graph[29].append(28);
    graph[29].append(30);
    graph[30].append(29);
    graph[30].append(31);
    graph[31].append(30);
    graph[31].append(32);
    graph[32].append(31);
    graph[32].append(33);
    graph[33].append(32);
    graph[33].append(34);
    graph[34].append(33);
    graph[34].append(35);
    graph[35].append(34);
    graph[35].append(36);
    graph[36].append(35);
    graph[36].append(37);
    graph[37].append(36);
    graph[37].append(38);
    graph[38].append(37);
    graph[38].append(39);
    graph[39].append(38);
    graph[39].append(40);
    graph[40].append(39);
    graph[40].append(41);
    graph[41].append(40);
    graph[41].append(33);
    graph[9].append(56); // Mayur Vihar 1
    graph[56].append(9);
    graph[56].append(57);
    graph[57].append(56);
    graph[57].append(11);
    graph[11].append(57);

    graph[42].append(43);
    graph[43].append(42);
    graph[43].append(44);
    graph[44].append(43);
    graph[44].append(45);
    graph[45].append(44);
    graph[45].append(46);
    graph[46].append(45);
    graph[46].append(47);
    graph[47].append(46);
    graph[47].append(48);
    graph[48].append(47);
    graph[48].append(49);
    graph[49].append(48);
    graph[49].append(50);
    graph[50].append(49);
    graph[50].append(51);
    graph[51].append(50);
    graph[51].append(52);
    graph[52].append(51);
    graph[52].append(53);
    graph[53].append(52);
    graph[53].append(54);
    graph[54].append(53);
    graph[54].append(55);
    graph[55].append(54);
    graph[55].append(56);
    graph[56].append(55);

    //MAGENTA LINE CONNCTONS
    graph[58].append(59);
    graph[59].append(58);
    graph[59].append(60);
    graph[60].append(59);
    graph[60].append(61);
    graph[61].append(60);
    graph[61].append(62);
    graph[62].append(61);
    graph[62].append(63);
    graph[63].append(62);
    graph[63].append(64);
    graph[64].append(63);
    graph[64].append(65);
    graph[65].append(64);
    graph[65].append(66);
    graph[66].append(65);
    graph[66].append(67);
    graph[67].append(66);
    graph[67].append(68);
    graph[68].append(67);
    graph[68].append(69);
    graph[69].append(68);
    graph[69].append(70);
    graph[70].append(69);
    graph[70].append(71);
    graph[71].append(70);
    graph[71].append(72);
    graph[72].append(71);
    graph[72].append(73);
    graph[73].append(72);
    graph[73].append(74);
    graph[74].append(73);
    graph[74].append(75);
    graph[75].append(74);
    graph[75].append(76);
    graph[76].append(75);
    graph[76].append(77);
    graph[77].append(76);
    graph[77].append(78);
    graph[78].append(77);
    graph[78].append(79);
    graph[79].append(78);
    graph[79].append(80);
    graph[80].append(79);
    graph[65].append(182); //Lajpat Nagar
    graph[18].append(65);

    //YELLOW LINE CONNECTIONS
    graph[81].append(82);
    graph[82].append(81);
    graph[82].append(83);
    graph[83].append(82);
    graph[83].append(84);
    graph[84].append(83);
    graph[84].append(85);
    graph[85].append(84);
    graph[85].append(86);
    graph[86].append(85);
    graph[86].append(87);
    graph[87].append(86);
    graph[87].append(88);
    graph[88].append(87);
    graph[88].append(89);
    graph[89].append(88);
    graph[89].append(90);
    graph[90].append(89);
    graph[90].append(91);
    graph[91].append(90);
    graph[91].append(92);
    graph[92].append(91);
    graph[92].append(93);
    graph[92].append(13); //Rajiv Chowk
    graph[13].append(92);
    graph[93].append(92);
    graph[93].append(94);
    graph[94].append(93);
    graph[94].append(95);
    graph[95].append(94);
    graph[95].append(96);
    graph[96].append(95);
    graph[96].append(97);
    graph[97].append(96);
    graph[97].append(98);
    graph[98].append(97);
    graph[98].append(99);
    graph[99].append(98);
    graph[99].append(100);
    graph[100].append(99);
    graph[100].append(101);
    graph[101].append(100);
    graph[101].append(102);
    graph[102].append(101);
    graph[102].append(103);
    graph[103].append(102);
    graph[103].append(104);
    graph[104].append(103);
    graph[104].append(105);
    graph[105].append(104);
    graph[105].append(106);
    graph[106].append(105);
    graph[106].append(107);
    graph[107].append(106);
    graph[107].append(108);
    graph[108].append(107);
    graph[108].append(109);
    graph[109].append(108);
    graph[109].append(110);
    graph[110].append(109);
    graph[110].append(111);
    graph[111].append(110);
    graph[111].append(112);
    graph[112].append(111);
    graph[112].append(113);
    graph[113].append(112);
    graph[113].append(114);
    graph[114].append(113);
    graph[114].append(115);
    graph[115].append(114);

    //GREEN LINE CONNECTIONS
    graph[116].append(117);
    graph[117].append(116);
    graph[117].append(118);
    graph[118].append(117);
    graph[118].append(119);
    graph[119].append(118);
    graph[119].append(120);
    graph[120].append(119);
    graph[120].append(121);
    graph[121].append(120);
    graph[121].append(122);
    graph[122].append(121);
    graph[122].append(123);
    graph[123].append(122);
    graph[123].append(124);
    graph[124].append(123);
    graph[124].append(125);
    graph[125].append(124);
    graph[125].append(126);
    graph[126].append(125);
    graph[126].append(127);
    graph[127].append(126);
    graph[127].append(128);
    graph[128].append(127);
    graph[128].append(129);
    graph[129].append(128);
    graph[129].append(130);
    graph[130].append(129);
    graph[130].append(131);
    graph[131].append(130);
    graph[131].append(132);
    graph[132].append(131);
    graph[132].append(133);
    graph[133].append(132);
    graph[133].append(134);
    graph[134].append(133);
    graph[134].append(135);
    graph[135].append(134);
    graph[135].append(136);
    graph[136].append(135);
    graph[136].append(137);
    graph[137].append(136);
    graph[20].append(134);  //Kirti Nagar
    graph[134].append(20);

    //ORANGE LINE CONNECTIONS
    graph[138].append(139);
    graph[139].append(138);
    graph[139].append(140);
    graph[140].append(139);
    graph[140].append(141);
    graph[141].append(140);
    graph[97].append(141);  //Central Secretariat
    graph[141].append(97);

    //RED LINE CONNECTIONS
    graph[142].append(143);
    graph[143].append(142);
    graph[143].append(144);
    graph[144].append(143);
    graph[144].append(145);
    graph[145].append(144);
    graph[145].append(146);
    graph[146].append(145);
    graph[146].append(147);
    graph[147].append(146);
    graph[147].append(148);
    graph[148].append(147);
    graph[148].append(149);
    graph[149].append(148);
    graph[149].append(150);
    graph[150].append(149);
    graph[150].append(151);
    graph[151].append(150);
    graph[151].append(152);
    graph[152].append(151);
    graph[152].append(153);
    graph[153].append(152);
    graph[153].append(92);  //Kashmere Gate
    graph[92].append(153);
    graph[92].append(154);
    graph[154].append(92);
    graph[154].append(155);
    graph[155].append(154);
    graph[155].append(156);
    graph[156].append(155);
    graph[156].append(157);
    graph[157].append(156);
    graph[157].append(158);
    graph[158].append(157);
    graph[158].append(159);
    graph[159].append(158);
    graph[159].append(160);
    graph[160].append(159);
    graph[160].append(161);
    graph[161].append(160);
    graph[161].append(162);
    graph[162].append(161);
    graph[162].append(163);
    graph[163].append(162);
    graph[163].append(164);
    graph[164].append(163);
    graph[164].append(165);
    graph[165].append(164);
    graph[165].append(166);
    graph[166].append(165);
    graph[166].append(167);
    graph[167].append(166);
    graph[167].append(168);
    graph[168].append(167);

    //PINK LINE CONNECTIONS
    graph[169].append(170);
    graph[170].append(169);
    graph[170].append(171);
    graph[171].append(170);
    graph[171].append(172);
    graph[172].append(171);
    graph[172].append(173);
    graph[173].append(172);
    graph[173].append(174);
    graph[174].append(173);
    graph[174].append(175);
    graph[175].append(174);
    graph[175].append(176);
    graph[176].append(175);
    graph[176].append(177);
    graph[177].append(176);
    graph[177].append(178);
    graph[178].append(177);
    graph[178].append(179);
    graph[179].append(178);
    graph[179].append(180);
    graph[180].append(179);
    graph[180].append(181);
    graph[181].append(180);
    graph[181].append(182);
    graph[182].append(181);
    graph[182].append(183);
    graph[183].append(182);
    graph[183].append(184);
    graph[184].append(183);
    graph[184].append(185);
    graph[185].append(184);
    graph[185].append(186);
    graph[186].append(185);
    graph[186].append(187);
    graph[187].append(186);
    graph[187].append(188);
    graph[188].append(187);
    graph[188].append(189);
    graph[189].append(188);
    graph[189].append(190);
    graph[190].append(189);
    graph[190].append(191);
    graph[191].append(190);
    graph[191].append(192);
    graph[192].append(191);
    graph[192].append(193);
    graph[193].append(192);
    graph[193].append(194);
    graph[194].append(193);
    graph[194].append(195);
    graph[195].append(194);
    graph[195].append(196);
    graph[196].append(195);
    graph[196].append(197);
    graph[197].append(196);
    graph[197].append(198);
    graph[198].append(197);
    graph[182].append(50);  //Botanical Garden
    graph[50].append(182);

    //VIOLET LINE CONNECTIONS
    graph[199].append(200);
    graph[200].append(199);
    graph[200].append(201);
    graph[201].append(200);
    graph[201].append(202);
    graph[202].append(201);
    graph[202].append(203);
    graph[203].append(202);
    graph[203].append(204);
    graph[204].append(203);
    graph[204].append(205);
    graph[205].append(204);
    graph[205].append(206);
    graph[206].append(205);
    graph[65].append(206); //Kalkaji Mandir
    graph[206].append(65);

    return graph;
}

void MetroDatabaseHandler::printAdjacencyList(const QMap<int, std::pair<QString, double>>& stationMap,
                                              const QMap<int, QList<int>>& graph) {
    for (auto it = graph.begin(); it != graph.end(); ++it) {
        int srcCode = it.key();
        const QList<int>& neighbors = it.value();

        QString srcName = stationMap[srcCode].first;

        qDebug() << "# " << srcCode << " : " << srcName << " - ";

        for (int destCode : neighbors) {
            QString destName = stationMap[destCode].first;

            double srcDist = stationMap[srcCode].second;
            double destDist = stationMap[destCode].second;
            double dist = qAbs(destDist - srcDist);  // Assuming distance from origin is used

            qDebug() << " -> " << destCode << " : " << destName << " - :"
                     << QString::number(dist, 'f', 1) << "km";
        }
    }
}

QString MetroDatabaseHandler::getLine(int stationId) const {
    QSqlQuery query;
    query.prepare("SELECT line FROM stations WHERE id = ?");
    query.addBindValue(stationId);
    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }
    return "";
}

MetroDatabaseHandler& MetroDatabaseHandler::instance() {
    static MetroDatabaseHandler instance;
    return instance;
}

MetroDatabaseHandler::MetroDatabaseHandler() {
    openConnection();
}

void MetroDatabaseHandler::openConnection() {
    if (QSqlDatabase::contains("metromate_connection")) {
        db = QSqlDatabase::database("metromate_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "metromate_connection");
        db.setDatabaseName("data.db");
        if (!db.open()) {
            qDebug() << "Failed to open DB:" << db.lastError();
        } else {
            qDebug() << "Database opened successfully.";
        }
    }
}

