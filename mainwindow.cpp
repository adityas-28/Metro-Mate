#include "mainwindow.h"
#include "farepage.h"
#include "welcomepage.h"
#include "loginpage.h"
#include "signuppage.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "app strting !";
    stack = new QStackedWidget(this);

    welcomePage = new WelcomePage;
    loginPage = new LoginPage;
    signupPage = new SignupPage;
    farePage = new FarePage;

    // stack->addWidget(farePage);
    stack->addWidget(welcomePage);
    stack->addWidget(loginPage);
    stack->addWidget(signupPage);
    stack->addWidget(farePage);

    setCentralWidget(stack);
    setFixedSize(1000, 700);

    connect(welcomePage->getLoginButton(), &QPushButton::clicked, this, &MainWindow::showLoginPage);
    connect(welcomePage->getSignupButton(), &QPushButton::clicked, this, &MainWindow::showSignupPage);

    // connect(loginPage->getLoginButton(), &QPushButton::clicked, this, &MainWindow::showFarePage);
    connect(signupPage->getSignupButton(), &QPushButton::clicked, this, &MainWindow::showLoginPage2);

    connect(loginPage, &LoginPage::goBackToWelcomePage, this, &MainWindow::showWelcomePage);
    connect(loginPage, &LoginPage::loginSuccess, this, &MainWindow::showFarePage);
    connect(signupPage, &SignupPage::goBackToWelcomePage, this, &MainWindow::showWelcomePage);

    connect(farePage, &FarePage::goBackToWelcomePage, this, &MainWindow::showWelcomePage);
    connect(signupPage, &SignupPage::goBackToLoginPage, this, &MainWindow::showWelcomePage);

}

MainWindow::~MainWindow() {}

void MainWindow::showLoginPage() {
    stack->setCurrentWidget(loginPage);
}

void MainWindow::showLoginPage2() {
    //QMessageBox::information(this, "Signup Successful", "User added successfully! Please log in.");
    stack->setCurrentWidget(loginPage);
}


void MainWindow::showSignupPage() {
    stack->setCurrentWidget(signupPage);
}

void MainWindow::showFarePage() {
    stack->setCurrentWidget(farePage);
}

// New Slot to Show WelcomePage
void MainWindow::showWelcomePage() {
    stack->setCurrentWidget(welcomePage);
}
void MainWindow::setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE"); // Using SQLite
    db.setDatabaseName("data.db"); // Database file (will be created if doesn't exist)

    if (!db.open()) {
        qDebug() << "Error: Could not open database!";
    } else {
        qDebug() << "Database connected successfully.";

        // Optional: create table if not exists
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE, password TEXT)");
    }
}
