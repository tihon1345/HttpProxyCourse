#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>

#include "DomainTypes.h"
#include "Serializer.h"

class AdminWidget : public QWidget {
    Q_OBJECT

public:
    // Принимаем указатель на курс для прямого редактирования данных в памяти
    explicit AdminWidget(Course* course, QWidget* parent = nullptr);

signals:
    void backRequested();

private slots:
    void onTopicChanged(int index);
    void onSaveClicked();
    void onLogoutClicked();

private:
    void setupUi();
    void loadTopics();

    Course* m_course; // Указатель на структуру данных курса

    // UI Elements
    QVBoxLayout* m_layout;
    QLabel* m_lblHeader;
    QComboBox* m_cbTopics;
    QTextEdit* m_txtHtmlEditor;
    QPushButton* m_btnSave;
    QPushButton* m_btnLogout;
};
