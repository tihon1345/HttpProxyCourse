#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug> // Для логирования

// Компоненты логики
#include "SessionManager.h"
#include "AuthService.h"

// Виджеты UI
#include "LoginWidget.h"
#include "TopicSelectionWidget.h"
#include "TopicViewWidget.h"
#include "TestWidget.h"
#include "AdminWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Слоты авторизации
    void handleStudentStart();
    void handleAdminLogin(const QString& password);
    void handleLogout();

    // Логика перехода администратора
    void onAdminBackRequested();

    // Логика переходов студента
    void onTopicSelected(int index);
    void onStartTestRequested();
    void onAnswerSubmitted(int answerIndex);

    // Меню
    void showAboutDialog();

private:
    void loadCourseData();
    void setupMenu(); // Инициализация MenuBar

    // Основные менеджеры
    SessionManager m_sessionManager;

    // UI
    QStackedWidget *m_stackedWidget;

    // Экраны приложения
    LoginWidget *m_loginWidget;
    TopicSelectionWidget *m_topicWidget;
    TopicViewWidget *m_topicViewWidget;
    TestWidget *m_testWidget;
    AdminWidget *m_adminWidget; // Lazy initialization
};
