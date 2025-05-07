#include "loginpage.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDir>


LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #000000;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 60, 40, 60);

    QLabel *title = new QLabel("🔐 <span style='color:#ffffff;'> Login to MetroMate </span>", this);
    title->setStyleSheet(R"(
        font-size: 45px;
        font-weight: 600;
        font-family: 'Segoe UI';
        padding: 10px;
        color: #ffffff;
    )");
    layout->addWidget(title, 0, Qt::AlignHCenter);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(12);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 120));
    title->setGraphicsEffect(shadow);

    // Initialize username/password fields
    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Enter Username");
    usernameLineEdit->setStyleSheet(R"(
        font-size: 16px;
        padding: 8px;
        border-radius: 5px;
        background-color: #3a3a3a;
        color: #ffffff;
        border: 1px solid #666;
    )");
    layout->addWidget(usernameLineEdit);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Enter Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet(R"(
        font-size: 16px;
        padding: 8px;
        border-radius: 5px;
        background-color: #3a3a3a;
        color: #ffffff;
        border: 1px solid #666;
    )");
    layout->addWidget(passwordLineEdit);

    // Initialize login button
    loginBtn = new QPushButton("Login", this);
    connect(loginBtn, &QPushButton::clicked, this, &LoginPage::handleLogin);

    loginBtn->setStyleSheet(R"(
        font-size: 17px;
        font-family: 'Segoe UI';
        padding: 10px 20px;
        background-color: #2E8B57;
        color: white;
        border-radius: 8px;
        width: 250px;
    )");
    loginBtn->setMaximumWidth(250);
    layout->addWidget(loginBtn, 0, Qt::AlignHCenter);

    // Back button
    QPushButton *backBtn = new QPushButton("Back", this);
    backBtn->setStyleSheet(R"(
        font-size: 17px;
        font-family: 'Segoe UI';
        padding: 10px 20px;
        background-color: #FF6347;
        color: white;
        border-radius: 8px;
        width: 250px;
    )");
    backBtn->setMaximumWidth(250);
    layout->addWidget(backBtn, 0, Qt::AlignHCenter);

    connect(backBtn, &QPushButton::clicked, this, &LoginPage::goBackToWelcomePage);

    // Initialize database
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

void LoginPage::handleLogin() {
    //qDebug() << "inside handleToLogin";
    QString user = usernameLineEdit->text();
    QString pass = passwordLineEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        // Custom styled "Empty fields" error
        QMessageBox msgBoxError;
        msgBoxError.setText("Please enter both username and password.");
        msgBoxError.setWindowTitle("Input Error");
        msgBoxError.setIcon(QMessageBox::Critical);
        msgBoxError.setStyleSheet(R"(
            QMessageBox {
                background-color: #8B0000;
                color: white;
                font-size: 16px;
                font-family: 'Segoe UI';
            }
            QPushButton {
                background-color: #FF6347;
                color: white;
                border-radius: 6px;
                padding: 6px 12px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #FF4500;
            }
        )");
        msgBoxError.exec();
        return;
    }

    /*QSqlQuery query2;
    query2.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, password TEXT)");

    query2.prepare("INSERT INTO users (name, password) VALUES (?, ?)");
    query2.addBindValue("aditya");
    query2.addBindValue("adi123");
    if (!query2.exec()) {
        qDebug() << "Insert failed:" << query2.lastError().text();
    } else {
        qDebug() << "Test user inserted successfully!";
    }*/

    QSqlQuery query;
    QString queryString = QString("SELECT * FROM users WHERE name = '%1' AND password = '%2'")
                              .arg(user)
                              .arg(pass);

    //qDebug() << "Last Query:" << query.lastQuery();
    //qDebug() << "Bound Values:" << query.boundValues();

    if (!query.exec(queryString)) {
        qDebug() << "Database query error:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        // Login Success - Show custom green success box
        QMessageBox msgBoxSuccess;
        msgBoxSuccess.setText("Welcome back, " + user + "!");
        msgBoxSuccess.setWindowTitle("Login Successful");
        msgBoxSuccess.setIcon(QMessageBox::Information);
        msgBoxSuccess.setStyleSheet(R"(
            QMessageBox {
                background-color: #2E8B57;
                color: white;
                font-size: 16px;
                font-family: 'Segoe UI';
            }
            QPushButton {
                background-color: #4CAF50;
                color: white;
                border-radius: 6px;
                padding: 6px 12px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #45a049;
            }
        )");
        msgBoxSuccess.exec();

        emit loginSuccess();
    } else {
        // Login Failed - Show custom red error box
        QMessageBox msgBoxError;
        msgBoxError.setText("Invalid username or password.");
        msgBoxError.setWindowTitle("Login Failed");
        msgBoxError.setIcon(QMessageBox::Critical);
        msgBoxError.setStyleSheet(R"(
            QMessageBox {
                background-color: #8B0000;
                color: white;
                font-size: 16px;
                font-family: 'Segoe UI';
            }
            QPushButton {
                background-color: #FF6347;
                color: white;
                border-radius: 6px;
                padding: 6px 12px;
                font-weight: bold;
            }
            QPushButton:hover {
                background-color: #FF4500;
            }
        )");
        msgBoxError.exec();
    }
}
