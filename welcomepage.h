#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>
#include <QPushButton>

class WelcomePage : public QWidget {
    Q_OBJECT

public:
    WelcomePage(QWidget *parent = nullptr);
    QPushButton* getLoginButton() const;
    QPushButton* getSignupButton() const;

private:
    QPushButton *loginButton;
    QPushButton *signupButton;
};

#endif // WELCOMEPAGE_H
