#ifndef FAREPAGE_H
#define FAREPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include "routeplanner.h"
#include <QSqlDatabase>

class FarePage : public QWidget {
    Q_OBJECT

public:
    explicit FarePage(QWidget *parent = nullptr);

signals:
    void goBackToWelcomePage();

private:
    QComboBox *sourceStation;
    QComboBox *destinationStation;
    QComboBox *filterDropdown;

    QPushButton *getFareButton;
    QPushButton *backBtn;

    QLabel *fareLabel;
    QLabel *timeLabel;
    QLabel *interchangeLabel;

    QTextEdit *routeDisplay;
    QSqlDatabase mydb;
    void populateStations();
    RoutePlanner *planner;

private slots:
    void calculateRoute();
};

#endif // FAREPAGE_H
