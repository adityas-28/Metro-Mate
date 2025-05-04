#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlDatabase>
#include <QDir>

class SignupPage : public QWidget {
    Q_OBJECT  // This is essential for Qt signal/slot system

public:
    explicit SignupPage(QWidget *parent = nullptr);
    QPushButton* getSignupButton() const;

signals:
    void goBackToWelcomePage();  // Declare the signal to go back to the welcome page
    void goBackToLoginPage();

private slots:
    void handleSignUp();
    void showSuccessMessage(const QString &message);
    void showErrorMessage(const QString &message);

private:
    QLineEdit *usernameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *signupBtn;
    QSqlDatabase mydb;
};


#endif // SIGNUPPAGE_H
