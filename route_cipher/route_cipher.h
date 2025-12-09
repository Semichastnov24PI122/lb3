#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>
#include <locale>
#include <codecvt>
#include <algorithm>

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) :
        std::invalid_argument(what_arg) {}
};

class RouteCipher {
private:
    int key;
    
    int getValidKey(const int k);
    std::wstring getValidOpenText(const std::wstring& s);
    std::wstring getValidCipherText(const std::wstring& s);
    
public:
    RouteCipher() = delete;
    explicit RouteCipher(const int k);
    
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};
