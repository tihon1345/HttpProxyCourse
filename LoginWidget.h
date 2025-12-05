#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class LoginWidget : public QWidget {
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void adminLoginAttempt(QString password);
    void startStudentSession();

private slots:
    void onAdminLoginClicked();

private:
    QLabel *m_welcomeLabel;
    QLineEdit *m_passwordEdit;
    QPushButton *m_adminLoginButton;
    QPushButton *m_studentStartButton;
};
