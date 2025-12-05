#include "AuthService.h"
#include <QCryptographicHash>
#include <QByteArray>

// Хэш SHA-256 для пароля "admin123"
// Примечание: В реальном приложении хэш должен храниться в защищенном конфигурационном файле
static const QString ADMIN_PASSWORD_HASH = "240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9";

bool AuthService::checkAdminPassword(const QString& password) {
    if (password.isEmpty()) {
        return false;
    }

    // Вычисляем SHA-256 хэш введенного пароля
    QByteArray hashBytes = QCryptographicHash::hash(
        password.toUtf8(),
        QCryptographicHash::Sha256
        );

    // Преобразуем в Hex-строку (lowercase по умолчанию в Qt)
    QString inputHash = QString::fromLatin1(hashBytes.toHex());

    // Сравниваем с эталоном (регистронезависимо для надежности hex представления)
    return (QString::compare(inputHash, ADMIN_PASSWORD_HASH, Qt::CaseInsensitive) == 0);
}
