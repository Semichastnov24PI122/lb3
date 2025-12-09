#include "route_cipher.h"
#include <cwctype>
#include <iostream>
#include <algorithm>

RouteCipher::RouteCipher(const int k)
{
    key = getValidKey(k);
}

int RouteCipher::getValidKey(const int k)
{
    if (k <= 0) {
        throw cipher_error("Ключ должен быть положительным числом");
    }
    return k;
}

std::wstring RouteCipher::getValidOpenText(const std::wstring& s)
{
    std::wstring tmp;
    for (auto c : s) {
        if (std::iswalpha(c)) {
            wchar_t up = std::towupper(c);
            // Проверяем, что символ русской буквы
            if ((up >= L'А' && up <= L'Я') ||  up == L'Ё') {
                tmp.push_back(up);
            }
        }
    }
    if (tmp.empty()) {
        throw cipher_error("Пустой открытый текст");
    }
    return tmp;
}

std::wstring RouteCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Пустой шифротекст");
    }
    
    std::wstring tmp;
    for (auto c : s) {
        if (std::iswalpha(c)) {
            wchar_t up = std::towupper(c);
            // Проверяем, что символ русской буквы
            if ((up >= L'А' && up <= L'Я') ||  up == L'Ё') {
                tmp.push_back(up);
            } else {
                throw cipher_error("Недопустимый символ в шифротексте");
            }
        } else {
            throw cipher_error("Недопустимый символ в шифротексте");
        }
    }
    return tmp;
}

std::wstring RouteCipher::encrypt(const std::wstring& open_text)
{
    std::wstring valid_text = getValidOpenText(open_text);
    size_t len = valid_text.length();
    
    if (key == 1 ||  len <= 1) {
        return valid_text;
    }
    
    size_t cols = key;
    size_t rows = (len + cols - 1) / cols; // Округление вверх
    
    // Создаем таблицу
    std::vector<std::wstring> table(rows);
    for (size_t i = 0; i < rows; i++) {
        size_t start = i * cols;
        size_t end = std::min(start + cols, len);
        table[i] = valid_text.substr(start, end - start);
        // Дополняем последнюю строку пробелами если нужно
        if (table[i].length() < cols) {
            table[i] += std::wstring(cols - table[i].length(), L' ');
        }
    }
    
    // Читаем сверху вниз, справа налево
    std::wstring result;
    for (int col = cols - 1; col >= 0; col--) {
        for (size_t row = 0; row < rows; row++) {
            if (table[row][col] != L' ') {
                result += table[row][col];
            }
        }
    }
    
    // ⭐️ ВАЖНО: Преобразуем результат к верхнему регистру
    for (auto& c : result) {
        c = std::towupper(c);
    }
    
    return result;
}

std::wstring RouteCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring valid_text = getValidCipherText(cipher_text);
    size_t len = valid_text.length();
    
    if (key == 1 ||  len <= 1) {
        return valid_text;
    }
    
    size_t cols = key;
    size_t rows = (len + cols - 1) / cols;
    
    // Вычисляем количество символов в каждом столбце
    std::vector<size_t> col_sizes(cols, rows);
    size_t total_cells = rows * cols;
    size_t empty_cells = total_cells - len;
    
    // Уменьшаем размеры последних столбцов для учета неполной таблицы
    for (size_t i = 0; i < empty_cells; i++) {
        col_sizes[cols - 1 - i]--;
    }
    
    // Создаем пустую таблицу
    std::vector<std::wstring> table(rows, std::wstring(cols, L' '));
    
    // Заполняем таблицу сверху вниз, справа налево
    size_t index = 0;
    for (int col = cols - 1; col >= 0; col--) {
        for (size_t row = 0; row < col_sizes[col]; row++) {
            table[row][col] = valid_text[index++];
        }
    }
    
    // Читаем по горизонтали слева направо, сверху вниз
    std::wstring result;
    for (size_t row = 0; row < rows; row++) {
        for (size_t col = 0; col < cols; col++) {
            if (table[row][col] != L' ') {
                result += table[row][col];
            }
        }
    }
    return result;
}
