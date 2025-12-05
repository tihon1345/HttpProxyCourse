#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

/**
 * @brief Виджет авторизации пользователя.
 * 
 * Предоставляет интерфейс для входа в систему как студента или администратора.
 */
class LoginWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор виджета авторизации.
     * @param parent Родительский виджет.
     */
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    /**
     * @brief Сигнал попытки входа администратора.
     * @param password Введенный пароль.
     */
    void adminLoginAttempt(QString password);

    /**
     * @brief Сигнал запуска студенческой сессии.
     */
    void startStudentSession();

private slots:
    /**
     * @brief Обработчик нажатия кнопки входа администратора.
     */
    void onAdminLoginClicked();

private:
    QLabel *m_welcomeLabel;
    QLineEdit *m_passwordEdit;
    QPushButton *m_adminLoginButton;
    QPushButton *m_studentStartButton;
};
