#include "modRusCipher.h"
#include <cwctype>

modRusCipher::modRusCipher(const std::wstring& skey)
{
    for (unsigned i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidKey(skey));
}

std::vector<int> modRusCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (auto c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

std::wstring modRusCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (auto i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

std::wstring modRusCipher::getValidKey(const std::wstring& s)
{
    if (s.empty())
        throw cipher_error("Пустой ключ");
    
    std::wstring tmp;
    for (auto c : s) {
        if (std::iswalpha(c)) {
            wchar_t up = std::towupper(c);
            if (alphaNum.find(up) == alphaNum.end())
                throw cipher_error("Недопустимый символ в ключе");
            tmp.push_back(up);
        } else {
            throw cipher_error("Недопустимый символ в ключе");
        }
    }
    return tmp;
}

std::wstring modRusCipher::getValidOpenText(const std::wstring& s)
{
    std::wstring tmp;
    for (auto c : s) {
        if (std::iswalpha(c)) {
            wchar_t up = std::towupper(c);
            if (alphaNum.find(up) != alphaNum.end())
                tmp.push_back(up);
        }
    }
    if (tmp.empty())
        throw cipher_error("Пустой открытый текст");
    return tmp;
}

std::wstring modRusCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty())
        throw cipher_error("Пустой шифротекст");
    
    for (auto c : s) {
        if (alphaNum.find(c) == alphaNum.end())
            throw cipher_error("Недопустимый символ в шифротексте");
    }
    return s;
}

std::wstring modRusCipher::encrypt(const std::wstring& open_text)
{
    std::vector<int> work = convert(getValidOpenText(open_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % numAlpha.size();
    }
    return convert(work);
}

std::wstring modRusCipher::decrypt(const std::wstring& cipher_text)
{
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    for (unsigned i = 0; i < work.size(); i++) {
        work[i] = (work[i] + numAlpha.size() - key[i % key.size()]) % numAlpha.size();
    }
    return convert(work);
}
