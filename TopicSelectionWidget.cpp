#include "TopicSelectionWidget.h"
#include <QMessageBox>

TopicSelectionWidget::TopicSelectionWidget(QWidget *parent) : QWidget(parent) {
    // UI Setup
    m_titleLabel = new QLabel("Выберите тему для изучения:", this);
    QFont font = m_titleLabel->font();
    font.setBold(true);
    font.setPointSize(12);
    m_titleLabel->setFont(font);

    m_topicsList = new QListWidget(this);

    m_selectButton = new QPushButton("Выбрать тему", this);
    m_logoutButton = new QPushButton("Выход в меню", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_topicsList);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(m_logoutButton);
    btnLayout->addStretch();
    btnLayout->addWidget(m_selectButton);

    layout->addLayout(btnLayout);

    // Connections
    connect(m_logoutButton, &QPushButton::clicked, this, &TopicSelectionWidget::logoutRequested);
    connect(m_selectButton, &QPushButton::clicked, this, &TopicSelectionWidget::onSelectClicked);
    connect(m_topicsList, &QListWidget::itemDoubleClicked, this, &TopicSelectionWidget::onListDoubleClicked);
}

void TopicSelectionWidget::setTopics(const QList<Topic>& topics) {
    m_topicsList->clear();
    for (const auto& topic : topics) {
        m_topicsList->addItem(topic.title);
    }
}

void TopicSelectionWidget::onSelectClicked() {
    int currentRow = m_topicsList->currentRow();
    if (currentRow >= 0) {
        emit topicSelected(currentRow);
    } else {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, выберите тему из списка.");
    }
}

void TopicSelectionWidget::onListDoubleClicked(QListWidgetItem *item) {
    Q_UNUSED(item);
    onSelectClicked(); // Reuse logic
}
