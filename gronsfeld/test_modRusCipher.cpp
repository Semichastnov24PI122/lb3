#include <UnitTest++/UnitTest++.h>
#include "modRusCipher.h"
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

// Тесты
SUITE(KeyTest)
{
    TEST(ValidKey) {
        CHECK_WSTR_EQUAL(L"БББББ", modRusCipher(L"Б").encrypt(L"ААААА"));
    }
    
    TEST(LongKey) {
        CHECK_WSTR_EQUAL(L"БВГДЕ", modRusCipher(L"БВГДЕ").encrypt(L"ААААА"));
    }
    
    TEST(LowCaseKey) {
        CHECK_WSTR_EQUAL(L"БББББ", modRusCipher(L"б").encrypt(L"ААААА"));
    }
    
    TEST(DigitsInKey) {
        CHECK_THROW(modRusCipher cp(L"А1"), cipher_error);
    }
    
    TEST(PunctuationInKey) {
        CHECK_THROW(modRusCipher cp(L"А,Б"), cipher_error);
    }
    
    TEST(WhitespaceInKey) {
        CHECK_THROW(modRusCipher cp(L"А Б"), cipher_error);
    }
    
    TEST(EmptyKey) {
        CHECK_THROW(modRusCipher cp(L""), cipher_error);
    }
}

struct KeyБ_fixture {
    modRusCipher* p;
    KeyБ_fixture() { p = new modRusCipher(L"Б"); }
    ~KeyБ_fixture() { delete p; }
};

SUITE(EncryptTest)
{
    TEST_FIXTURE(KeyБ_fixture, UpCaseString) {
        CHECK_WSTR_EQUAL(L"БВГДЕЁЖЗИ", p->encrypt(L"АБВГДЕЁЖЗ"));
    }
    
    TEST_FIXTURE(KeyБ_fixture, LowCaseString) {
        CHECK_WSTR_EQUAL(L"БВГДЕЁЖЗИ", p->encrypt(L"абвгдеёжз"));
    }
    
    TEST_FIXTURE(KeyБ_fixture, StringWithWhitespaceAndPunct) {
        CHECK_WSTR_EQUAL(L"БВГДЕЁЖЗИ", p->encrypt(L"АБВ, ГДЕЁЖЗ!"));
    }
    
    TEST_FIXTURE(KeyБ_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }
    
    TEST_FIXTURE(KeyБ_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error);
    }
}

SUITE(DecryptTest)
{
    TEST_FIXTURE(KeyБ_fixture, UpCaseString) {
        std::wstring cipher = p->encrypt(L"АБВГД");
        CHECK_WSTR_EQUAL(L"АБВГД", p->decrypt(cipher));
    }
    
    TEST_FIXTURE(KeyБ_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"бвгде"), cipher_error);
    }
    
    TEST_FIXTURE(KeyБ_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
