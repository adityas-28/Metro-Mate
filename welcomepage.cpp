#include "welcomepage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QGraphicsDropShadowEffect>

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
{
    // Set a soft, light color background for the whole window (no white)
    setStyleSheet("background-color: #f4f4f4;"); // Light grey background for the page

    // Main layout with some spacing and margins
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(30);
    layout->setContentsMargins(50, 80, 50, 80);

    // Title with custom styles (No white background)
    QLabel *title = new QLabel("🚇 <span style='color:#2E8B57;'> Welcome to MetroMate</span>", this);
    title->setStyleSheet(R"(
        font-size: 32px;
        font-weight: 600;
        font-family: 'Segoe UI', sans-serif;
        padding: 10px;
        color: #333;
        background: transparent; /* Ensure no background behind the title */
    )");
    title->setTextFormat(Qt::RichText);
    layout->addWidget(title, 0, Qt::AlignHCenter);

    // Add shadow effect to the title for a nice visual
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(12);
    shadow->setOffset(3, 3);
    shadow->setColor(QColor(0, 0, 0, 120));
    title->setGraphicsEffect(shadow);

    // Buttons for login and signup
    loginButton = new QPushButton("🔐 Login", this);
    signupButton = new QPushButton("📝 Signup", this);

    // Button style (smooth hover effects and padding)
    QString btnStyle = R"(
        QPushButton {
            font-size: 18px;
            font-family: 'Segoe UI', sans-serif;
            padding: 12px 30px;
            border-radius: 12px;
            background-color: #2E8B57;  /* Green background */
            color: white;
            border: none;
            transition: all 0.3s ease;
            width: 250px;  /* Set both buttons to the same width */
        }
        QPushButton:hover {
            background-color: #246b45;  /* Darker green on hover */
        }
        QPushButton:pressed {
            background-color: #1f5c3b;  /* Even darker green when pressed */
        }
    )";

    // Apply the same style to both buttons
    loginButton->setStyleSheet(btnStyle);
    signupButton->setStyleSheet(btnStyle);

    // Add buttons to the layout with horizontal centering
    layout->addWidget(loginButton, 0, Qt::AlignHCenter);
    layout->addWidget(signupButton, 0, Qt::AlignHCenter);
}

QPushButton* WelcomePage::getLoginButton() const {
    return loginButton;
}

QPushButton* WelcomePage::getSignupButton() const {
    return signupButton;
}
