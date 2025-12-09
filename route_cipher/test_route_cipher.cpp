#include <UnitTest++/UnitTest++.h>
#include "route_cipher.h"
#include <locale>
#include <codecvt>
#include <iostream>

// Настройка локали
struct LocaleSetup {
    LocaleSetup() {
        try {
            std::locale::global(std::locale("ru_RU.UTF-8"));
        } catch (const std::exception& e) {
            std::cout << "Warning: Russian locale not available, using default" << std::endl;
        }
    }
};
LocaleSetup localeSetup;

// Поддержка сравнения std::wstring в UnitTest++
inline void CHECK_WSTR_EQUAL(const std::wstring& expected, const std::wstring& actual)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::string exp_str = conv.to_bytes(expected);
    std::string act_str = conv.to_bytes(actual);
    CHECK_EQUAL(exp_str, act_str);
}

// Тестовые сценарии для конструктора
SUITE(ConstructorTest)
{
    TEST(ValidKey) {
        RouteCipher cipher(3);
        CHECK(true);
    }
    
    TEST(KeyOne) {
        RouteCipher cipher(1);
        CHECK(true);
    }
    
    TEST(ZeroKey) {
        CHECK_THROW(RouteCipher cipher(0), cipher_error);
    }
    
    TEST(NegativeKey) {
        CHECK_THROW(RouteCipher cipher(-3), cipher_error);
    }
}

// Фикстуры для часто используемых ключей
struct Key3_fixture {
    RouteCipher* p;
    Key3_fixture() { p = new RouteCipher(3); }
    ~Key3_fixture() { delete p; }
};

struct Key4_fixture {
    RouteCipher* p;
    Key4_fixture() { p = new RouteCipher(4); }
    ~Key4_fixture() { delete p; }
};

// Базовые тесты шифрования/дешифрования
SUITE(BasicTests)
{
    TEST_FIXTURE(Key3_fixture, ShortTextRoundTrip) {
        std::wstring original = L"ПРИВЕТ";
        std::wstring encrypted = p->encrypt(original);
        std::wstring decrypted = p->decrypt(encrypted);
        CHECK_WSTR_EQUAL(original, decrypted);
    }
    
    TEST_FIXTURE(Key4_fixture, LongTextRoundTrip) {
        std::wstring original = L"ПРИВЕТМИР";
        std::wstring encrypted = p->encrypt(original);
        std::wstring decrypted = p->decrypt(encrypted);
        CHECK_WSTR_EQUAL(original, decrypted);
    }
    
    TEST_FIXTURE(Key3_fixture, TextWithWhitespace) {
        std::wstring original = L"ПРИВЕТ МИР";
        std::wstring encrypted = p->encrypt(original);
        std::wstring decrypted = p->decrypt(encrypted);
        // После удаления пробелов должно получиться "ПРИВЕТМИР"
        CHECK_WSTR_EQUAL(L"ПРИВЕТМИР", decrypted);
    }
    
    TEST_FIXTURE(Key3_fixture, LowCaseString) {
        std::wstring original = L"привет";
        std::wstring encrypted = p->encrypt(original);
        std::wstring decrypted = p->decrypt(encrypted);
        CHECK_WSTR_EQUAL(L"ПРИВЕТ", decrypted);
    }
    
    TEST_FIXTURE(Key3_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(Key3_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error);
    }
    
    TEST_FIXTURE(Key3_fixture, TextShorterThanKey) {
        std::wstring original = L"ПРИ";
        std::wstring encrypted = p->encrypt(original);
        std::wstring decrypted = p->decrypt(encrypted);
        CHECK_WSTR_EQUAL(original, decrypted);
    }
}

// Тесты особых случаев
SUITE(SpecialCasesTest)
{
    TEST(KeyOne) {
        RouteCipher cipher(1);
        std::wstring original = L"ТЕСТ";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK_WSTR_EQUAL(original, decrypted);
    }
    
    TEST(SingleCharacter) {
        RouteCipher cipher(3);
        std::wstring original = L"А";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK_WSTR_EQUAL(original, decrypted);
    }
    
    TEST(RepeatedCharacters) {
        RouteCipher cipher(2);
        std::wstring original = L"АААА";
        std::wstring encrypted = cipher.encrypt(original);
        std::wstring decrypted = cipher.decrypt(encrypted);
        CHECK_WSTR_EQUAL(original, decrypted);
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
