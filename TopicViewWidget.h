#pragma once

#include <QWidget>
#include <QTextBrowser>
#include <QPushButton>
#include <QVBoxLayout>
#include "DomainTypes.h"

class TopicViewWidget : public QWidget {
    Q_OBJECT

public:
    explicit TopicViewWidget(QWidget *parent = nullptr);

    // Загружает контент темы
    void showTopic(const Topic& topic);

signals:
    void startTestRequested();
    void backRequested();

private:
    QTextBrowser* textBrowser;
    QPushButton* btnStartTest;
    QPushButton* btnBack;
};
