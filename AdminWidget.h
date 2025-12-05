#pragma once

#include "DomainTypes.h"
#include "Serializer.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>

/**
 * @brief Виджет администрирования курса.
 * 
 * Предоставляет интерфейс для редактирования содержимого тем курса
 * и сохранения изменений.
 */
class AdminWidget : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Конструктор виджета администрирования.
     * @param course Указатель на курс для редактирования.
     * @param parent Родительский виджет.
     */
    explicit AdminWidget(Course* course, QWidget* parent = nullptr);

signals:
    /**
     * @brief Сигнал запроса возврата к главному экрану.
     */
    void backRequested();

private slots:
    /**
     * @brief Обработчик изменения выбранной темы.
     * @param index Индекс выбранной темы.
     */
    void onTopicChanged(int index);

    /**
     * @brief Обработчик сохранения изменений.
     */
    void onSaveClicked();

    /**
     * @brief Обработчик выхода из панели администратора.
     */
    void onLogoutClicked();

private:
    /**
     * @brief Настраивает пользовательский интерфейс.
     */
    void setupUi();

    /**
     * @brief Загружает список тем в комбобокс.
     */
    void loadTopics();

    Course* m_course;
    QVBoxLayout* m_layout;
    QLabel* m_lblHeader;
    QComboBox* m_cbTopics;
    QTextEdit* m_txtHtmlEditor;
    QPushButton* m_btnSave;
    QPushButton* m_btnLogout;
};
