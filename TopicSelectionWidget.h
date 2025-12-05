#pragma once

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "DomainTypes.h"

class TopicSelectionWidget : public QWidget {
    Q_OBJECT

public:
    explicit TopicSelectionWidget(QWidget *parent = nullptr);
    void setTopics(const QList<Topic>& topics);

signals:
    void topicSelected(int index);
    void logoutRequested();

private slots:
    void onSelectClicked();
    void onListDoubleClicked(QListWidgetItem *item);

private:
    QLabel *m_titleLabel;
    QListWidget *m_topicsList;
    QPushButton *m_selectButton;
    QPushButton *m_logoutButton;
};
