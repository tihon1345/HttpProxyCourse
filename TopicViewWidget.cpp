#include "TopicViewWidget.h"

TopicViewWidget::TopicViewWidget(QWidget *parent) : QWidget(parent) {
    auto layout = new QVBoxLayout(this);

    textBrowser = new QTextBrowser(this);
    textBrowser->setReadOnly(true);
    // Отключаем открытие ссылок во внешнем браузере для безопасности/изоляции
    textBrowser->setOpenExternalLinks(false);

    btnStartTest = new QPushButton("Перейти к тесту", this);
    btnBack = new QPushButton("Назад к списку", this);

    layout->addWidget(textBrowser);
    layout->addWidget(btnStartTest);
    layout->addWidget(btnBack);

    connect(btnStartTest, &QPushButton::clicked, this, &TopicViewWidget::startTestRequested);
    connect(btnBack, &QPushButton::clicked, this, &TopicViewWidget::backRequested);
}

void TopicViewWidget::showTopic(const Topic& topic) {
    // В будущем можно добавить CSS для красоты
    textBrowser->setHtml(topic.htmlContent);
}
