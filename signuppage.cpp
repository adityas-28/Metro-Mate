#include "signuppage.h"
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

SignupPage::SignupPage(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #f4f4f4;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(20);
    layout->setContentsMargins(40, 40, 40, 40);

    QLabel *title = new QLabel("📝 <span style='color:#2E8B57;'> Signup for MetroMate </span>", this);
    title->setStyleSheet(R"(
        font-size: 24px;
        font-weight: 600;
        font-family: 'Segoe UI';
        padding: 10px;
        color: #333;
    )");
    layout->addWidget(title, 0, Qt::AlignHCenter);


    // QLineEdit::placeholder {
    // color: gray; /* You can use any color like #888888 or rgba */
    // }
    // --- Corrected here ---
    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Choose Username");
    usernameLineEdit->setStyleSheet("font-size: 16px; padding: 8px; color: black;");
    layout->addWidget(usernameLineEdit);

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Create Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("font-size: 16px; padding: 8px; color: black;");
    layout->addWidget(passwordLineEdit);
    // -----------------------

    signupBtn = new QPushButton("Signup", this);
    signupBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 17px;
            font-family: 'Segoe UI';
            padding: 10px 20px;
            background-color: #32CD32;
            color: white;
            border-radius: 6px;
            width: 250px;
        }
        QPushButton:hover {
            background-color: #28a745;
        }
    )");
    layout->addWidget(signupBtn, 0, Qt::AlignHCenter);

    QPushButton *backBtn = new QPushButton("Back", this);
    backBtn->setStyleSheet(R"(
        QPushButton {
            font-size: 17px;
            font-family: 'Segoe UI';
            padding: 10px 20px;
            background-color: #FF6347;
            color: white;
            border-radius: 8px;
            width: 250px;
        }
        QPushButton:hover {
            background-color: #FF4500;
        }
    )");
    layout->addWidget(backBtn, 0, Qt::AlignHCenter);

    connect(signupBtn, &QPushButton::clicked, this, &SignupPage::handleSignUp);
    connect(backBtn, &QPushButton::clicked, this, &SignupPage::goBackToWelcomePage);

    // Initialize database
    mydb = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + "/data.db";
    mydb.setDatabaseName(dbPath);

    if (!mydb.open()) {
        qDebug() << "Database failed to open:" << mydb.lastError().text();
    } else {
        qDebug() << "Database opened successfully in signup page.";

        // Create users table if not exists
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "name TEXT UNIQUE, "
                   "password TEXT)");
    }
}


QPushButton* SignupPage::getSignupButton() const {
    return signupBtn;
}

void SignupPage::handleSignUp() {
    QString user = usernameLineEdit->text();
    QString pass = passwordLineEdit->text();

    if (user.isEmpty() || pass.isEmpty()) {
        showErrorMessage("Please fill all fields.");
        return;
    }

    // Check if username already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT * FROM users WHERE name = ?");
    checkQuery.addBindValue(user);

    if (!checkQuery.exec()) {
        qDebug() << "Database query error:" << checkQuery.lastError().text();
        return;
    }

    if (checkQuery.next()) {
        showErrorMessage("Username already taken. Please choose another.");
        return;
    }

    // Insert new user into database
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO users (name, password) VALUES (?, ?)");
    insertQuery.addBindValue(user);
    insertQuery.addBindValue(pass);

    if (!insertQuery.exec()) {
        qDebug() << "Insert failed:" << insertQuery.lastError().text();
        return;
    }

    showSuccessMessage("Signup Successful! You can now login.");
    emit goBackToWelcomePage();
}

// Utility to show success message
void SignupPage::showSuccessMessage(const QString &message) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setWindowTitle("Success");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStyleSheet(R"(
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
    msgBox.exec();
}

// Utility to show error message
void SignupPage::showErrorMessage(const QString &message) {
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setWindowTitle("Error");
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStyleSheet(R"(
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
    msgBox.exec();
}
