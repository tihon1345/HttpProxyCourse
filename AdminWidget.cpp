#include "AdminWidget.h"

// Константа для имени файла курса
static const QString COURSE_DATA_FILE = "course.dat";

AdminWidget::AdminWidget(Course* course, QWidget* parent)
    : QWidget(parent)
    , m_course(course)
{
    setupUi();
    loadTopics();
}

void AdminWidget::setupUi() {
    m_layout = new QVBoxLayout(this);

    m_lblHeader = new QLabel("Панель Администратора: Редактор Лекций", this);
    QFont font = m_lblHeader->font();
    font.setBold(true);
    font.setPointSize(12);
    m_lblHeader->setFont(font);
    m_lblHeader->setAlignment(Qt::AlignCenter);
    m_layout->addWidget(m_lblHeader);

    m_layout->addWidget(new QLabel("Выберите тему для редактирования:", this));
    m_cbTopics = new QComboBox(this);
    m_layout->addWidget(m_cbTopics);

    m_layout->addWidget(new QLabel("HTML Контент (QTextBrowser совместимый):", this));
    m_txtHtmlEditor = new QTextEdit(this);
    m_txtHtmlEditor->setAcceptRichText(false);
    m_layout->addWidget(m_txtHtmlEditor);

    m_btnSave = new QPushButton("Сохранить изменения в файл", this);
    m_layout->addWidget(m_btnSave);

    m_btnLogout = new QPushButton("Выйти", this);
    m_layout->addWidget(m_btnLogout);

    connect(m_cbTopics, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AdminWidget::onTopicChanged);
    connect(m_btnSave, &QPushButton::clicked, this, &AdminWidget::onSaveClicked);
    connect(m_btnLogout, &QPushButton::clicked, this, &AdminWidget::onLogoutClicked);
}

void AdminWidget::loadTopics() {
    if (!m_course) return;

    m_cbTopics->blockSignals(true);
    m_cbTopics->clear();

    for (const Topic& topic : m_course->topics) {
        m_cbTopics->addItem(topic.title);
    }

    m_cbTopics->blockSignals(false);

    if (m_cbTopics->count() > 0) {
        m_cbTopics->setCurrentIndex(0);
        onTopicChanged(0);
    }
}

void AdminWidget::onTopicChanged(int index) {
    if (!m_course || index < 0 || index >= m_course->topics.size()) {
        m_txtHtmlEditor->clear();
        return;
    }

    m_txtHtmlEditor->setPlainText(m_course->topics[index].htmlContent);
}

void AdminWidget::onSaveClicked() {
    if (!m_course) return;

    int index = m_cbTopics->currentIndex();
    if (index < 0 || index >= m_course->topics.size()) {
        QMessageBox::warning(this, "Ошибка", "Нет выбранной темы для сохранения.");
        return;
    }

    try {
        QString newContent = m_txtHtmlEditor->toPlainText();
        if (newContent.trimmed().isEmpty()) {
            QMessageBox::warning(this, "Предупреждение", "Текст лекции пуст!");
            return;
        }
        m_course->topics[index].htmlContent = newContent;

        Serializer::save(*m_course, COURSE_DATA_FILE);
        QMessageBox::information(this, "Успех", "Курс успешно сохранен и зашифрован!");

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка сохранения",
                              QString("Не удалось записать файл:\n%1").arg(e.what()));
    }
}

void AdminWidget::onLogoutClicked() {
    m_txtHtmlEditor->clear();
    emit backRequested();
}
