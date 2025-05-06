// loginpage.h

#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QDir>

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    QPushButton* getLoginButton() const;

signals:
    void goBackToWelcomePage();
    void loginSuccess();

private slots:
    void handleLogin();

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginBtn;
    QSqlDatabase mydb;
};

#endif // LOGINPAGE_H
