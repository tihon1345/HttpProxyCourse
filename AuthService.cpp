#include "AuthService.h"
#include <QCryptographicHash>
#include <QByteArray>

// Хэш SHA-256 для пароля администратора
// ВНИМАНИЕ: В реальном проекте хэш должен храниться в защищенном конфигурационном файле!
// Текущий хэш соответствует паролю "admin123"
static const char* ADMIN_HASH = "240be518fabd2724ddb6f04eeb1da5967448d7e831c08c8fa822809f74c720a9";

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

    // Сравниваем с эталоном (точное сравнение, так как SHA-256 всегда lowercase)
    return (inputHash == QString::fromLatin1(ADMIN_HASH));
}
