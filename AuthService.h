#pragma once

#include <QString>

/**
 * @brief Сервис аутентификации администратора.
 *
 * Отвечает за проверку пароля путем сравнения SHA-256 хэшей.
 */
class AuthService {
public:
    /**
     * @brief Проверяет валидность пароля администратора.
     * @param password Введенный пароль в открытом виде.
     * @return true, если хэш пароля совпадает с эталонным.
     */
    static bool checkAdminPassword(const QString& password);
};
