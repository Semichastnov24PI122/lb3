#include <iostream>
#include <locale>
#include <codecvt>
#include "route_cipher.h"

void demonstrate_encryption() {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    
    try {
        // Пример 1: Короткий текст
        std::cout << "Пример 1:" << std::endl;
        RouteCipher cipher1(3);
        std::wstring text1 = L"ПРИВЕТ";
        std::wstring encrypted1 = cipher1.encrypt(text1);
        std::wstring decrypted1 = cipher1.decrypt(encrypted1);
        
        std::cout << "Исходный: " << converter.to_bytes(text1) << std::endl;
        std::cout << "Зашифрованный: " << converter.to_bytes(encrypted1) << std::endl;
        std::cout << "Расшифрованный: " << converter.to_bytes(decrypted1) << std::endl;
        std::cout << "Совпадает: " << (text1 == decrypted1 ? "ДА" : "НЕТ") << std::endl;
        std::cout << std::endl;

        // Пример 2: Длинный текст
        std::cout << "Пример 2:" << std::endl;
        RouteCipher cipher2(4);
        std::wstring text2 = L"ПРИВЕТМИР";
        std::wstring encrypted2 = cipher2.encrypt(text2);
        std::wstring decrypted2 = cipher2.decrypt(encrypted2);
        
        std::cout << "Исходный: " << converter.to_bytes(text2) << std::endl;
        std::cout << "Зашифрованный: " << converter.to_bytes(encrypted2) << std::endl;
        std::cout << "Расшифрованный: " << converter.to_bytes(decrypted2) << std::endl;
        std::cout << "Совпадает: " << (text2 == decrypted2 ? "ДА" : "НЕТ") << std::endl;
        std::cout << std::endl;

        // Пример 3: Текст с пробелами и знаками препинания
        std::cout << "Пример 3:" << std::endl;
        RouteCipher cipher3(2);
        std::wstring text3 = L"Привет, мир! 2024";
        std::wstring encrypted3 = cipher3.encrypt(text3);
        std::wstring decrypted3 = cipher3.decrypt(encrypted3);
        
        std::cout << "Исходный: " << converter.to_bytes(text3) << std::endl;
        std::cout << "Зашифрованный: " << converter.to_bytes(encrypted3) << std::endl;
        std::cout << "Расшифрованный: " << converter.to_bytes(decrypted3) << std::endl;
        std::cout << "Ожидаемый: ПРИВЕТМИР" << std::endl;
        std::cout << "Совпадает: " << (L"ПРИВЕТМИР" == decrypted3 ? "ДА" : "НЕТ") << std::endl;
        
    } catch (const cipher_error& e) {
        std::cout << "Ошибка: " << e.what() << std::endl;
    }
}

int main() {
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::cout << "Демонстрация работы шифра маршрутной перестановки" << std::endl;
    std::cout << "==============================================" << std::endl;
    demonstrate_encryption();
    return 0;
}
