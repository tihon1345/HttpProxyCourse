#include "MainWindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_adminWidget(nullptr)
{
    setWindowTitle("HTTP Proxy Training System");
    resize(900, 650);

    setupMenu();

    m_stackedWidget = new QStackedWidget(this);

    m_loginWidget = new LoginWidget(this);
    m_topicWidget = new TopicSelectionWidget(this);
    m_topicViewWidget = new TopicViewWidget(this);
    m_testWidget = new TestWidget(this);

    m_stackedWidget->addWidget(m_loginWidget);
    m_stackedWidget->addWidget(m_topicWidget);
    m_stackedWidget->addWidget(m_topicViewWidget);
    m_stackedWidget->addWidget(m_testWidget);

    setCentralWidget(m_stackedWidget);

    connect(m_loginWidget, &LoginWidget::startStudentSession,
            this, &MainWindow::handleStudentStart);
    connect(m_loginWidget, &LoginWidget::adminLoginAttempt,
            this, &MainWindow::handleAdminLogin);

    connect(m_topicWidget, &TopicSelectionWidget::logoutRequested,
            this, &MainWindow::handleLogout);
    connect(m_topicWidget, &TopicSelectionWidget::topicSelected,
            this, &MainWindow::onTopicSelected);

    connect(m_topicViewWidget, &TopicViewWidget::backRequested,
            this, [this](){ m_stackedWidget->setCurrentWidget(m_topicWidget); });
    connect(m_topicViewWidget, &TopicViewWidget::startTestRequested,
            this, &MainWindow::onStartTestRequested);

    connect(m_testWidget, &TestWidget::answerSubmitted,
            this, &MainWindow::onAnswerSubmitted);

    loadCourseData();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupMenu() {
    QMenuBar* bar = menuBar();
    QMenu* helpMenu = bar->addMenu("Справка");
    QAction* aboutAction = helpMenu->addAction("О программе");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);
}

void MainWindow::showAboutDialog() {
    QString aboutText =
        "Курсовая работа: Обучающая система HTTP Proxy.\n"
        "Выполнил: Несов Кирилл Алексеевич, студент группы АБс-424, АВТФ, НГТУ\n"
        "Год: 2025\n\n"
        "Программа предназначена для изучения и тестирования знаний по теме прокси-серверов.";

    QMessageBox::about(this, "О программе", aboutText);
}

void MainWindow::loadCourseData() {
    try {
        m_sessionManager.loadCourse("course.dat");
    } catch (const std::exception& e) {
        qWarning() << "Could not load course data:" << e.what();
    }
}

void MainWindow::handleStudentStart() {
    if (!m_sessionManager.isCourseLoaded()) {
        QMessageBox::critical(this, "Ошибка", "Курс не загружен. Обратитесь к администратору.");
        return;
    }
    m_topicWidget->setTopics(m_sessionManager.getCourse().topics);
    m_stackedWidget->setCurrentWidget(m_topicWidget);
}

void MainWindow::handleAdminLogin(const QString& password) {
    if (AuthService::checkAdminPassword(password)) {
        if (!m_adminWidget) {
            m_adminWidget = new AdminWidget(const_cast<Course*>(&m_sessionManager.getCourse()), this);
            connect(m_adminWidget, &AdminWidget::backRequested,
                    this, &MainWindow::onAdminBackRequested);
            m_stackedWidget->addWidget(m_adminWidget);
        }
        m_stackedWidget->setCurrentWidget(m_adminWidget);
    } else {
        QMessageBox::warning(this, "Ошибка доступа", "Неверный пароль администратора.");
    }
}

void MainWindow::handleLogout() {
    m_stackedWidget->setCurrentWidget(m_loginWidget);
}

void MainWindow::onAdminBackRequested() {
    loadCourseData();
    m_stackedWidget->setCurrentWidget(m_loginWidget);
}

void MainWindow::onTopicSelected(int index) {
    try {
        m_sessionManager.startTopic(index);

        Topic* topic = m_sessionManager.getCurrentTopic();
        if (topic) {
            m_topicViewWidget->showTopic(*topic);
            m_stackedWidget->setCurrentWidget(m_topicViewWidget);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Не удалось открыть тему: %1").arg(e.what()));
    }
}

void MainWindow::onStartTestRequested() {
    try {
        Question* q = m_sessionManager.getCurrentQuestion();
        if (q) {
            m_testWidget->showQuestion(*q);
            m_stackedWidget->setCurrentWidget(m_testWidget);
        } else {
            QMessageBox::warning(this, "Внимание", "В данной теме отсутствуют вопросы для тестирования.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка теста", e.what());
    }
}

void MainWindow::onAnswerSubmitted(int answerIndex) {
    try {
        auto result = m_sessionManager.submitAnswer(answerIndex);

        switch (result) {
        case SessionManager::SubmitResult::Correct:
            QMessageBox::information(this, "Верно", "Правильный ответ!");
            {
                Question* nextQ = m_sessionManager.getCurrentQuestion();
                if (nextQ) {
                    m_testWidget->showQuestion(*nextQ);
                }
            }
            break;

        case SessionManager::SubmitResult::Wrong:
            QMessageBox::warning(this, "Ошибка", "Неверный ответ. Попробуйте снова.");
            break;

        case SessionManager::SubmitResult::FailRelearn:
            QMessageBox::critical(this, "Тест провален",
                                  "Допущено критическое количество ошибок (3).\n"
                                  "Вам необходимо повторить теоретический материал.");
            m_stackedWidget->setCurrentWidget(m_topicViewWidget);
            break;

        case SessionManager::SubmitResult::TopicFinished:
            QMessageBox::information(this, "Успех", "Тема успешно пройдена!");
            m_topicWidget->setTopics(m_sessionManager.getCourse().topics);
            m_stackedWidget->setCurrentWidget(m_topicWidget);
            break;

        case SessionManager::SubmitResult::CourseFinished:
            QMessageBox::information(this, "Поздравляем", "Вы успешно завершили весь курс!");
            m_stackedWidget->setCurrentWidget(m_topicWidget);
            break;
        }

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Системная ошибка", e.what());
        m_stackedWidget->setCurrentWidget(m_topicWidget);
    }
}
