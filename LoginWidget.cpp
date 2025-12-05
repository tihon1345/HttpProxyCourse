#include "LoginWidget.h"
#include <QFont>

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent) {
    // 1. Создание элементов
    m_welcomeLabel = new QLabel("Добро пожаловать в курс HTTP Proxy", this);
    m_welcomeLabel->setAlignment(Qt::AlignCenter);

    // Увеличение шрифта
    QFont titleFont = m_welcomeLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_welcomeLabel->setFont(titleFont);

    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setPlaceholderText("Пароль администратора");
    m_passwordEdit->setEchoMode(QLineEdit::Password);

    m_adminLoginButton = new QPushButton("Войти как Администратор", this);
    m_studentStartButton = new QPushButton("Начать обучение (Студент)", this);

    // 2. Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(m_welcomeLabel);
    layout->addSpacing(20);
    layout->addWidget(m_studentStartButton);
    layout->addSpacing(20);
    layout->addWidget(m_passwordEdit);
    layout->addWidget(m_adminLoginButton);
    layout->addStretch();

    // Центрирование по горизонтали с отступами
    layout->setContentsMargins(50, 20, 50, 20);

    // 3. Коннекты
    connect(m_adminLoginButton, &QPushButton::clicked, this, &LoginWidget::onAdminLoginClicked);
    connect(m_studentStartButton, &QPushButton::clicked, this, &LoginWidget::startStudentSession);

    // Также отправляем сигнал при нажатии Enter в поле пароля
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::onAdminLoginClicked);
}

void LoginWidget::onAdminLoginClicked() {
    emit adminLoginAttempt(m_passwordEdit->text());
    m_passwordEdit->clear(); // Очистить поле после попытки
}
