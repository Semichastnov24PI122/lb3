CXX = g++
CXXFLAGS = -std=c++17 -fexec-charset=UTF-8 -finput-charset=UTF-8 -Wall -Wextra
LDFLAGS = -lUnitTest++

# Основная цель
all: RusGronsfeldTest

# Сборка тестов
RusGronsfeldTest: modRusCipher.o test_modRusCipher.o
	$(CXX) $(CXXFLAGS) -o RusGronsfeldTest modRusCipher.o test_modRusCipher.o $(LDFLAGS)

# Компиляция объектных файлов
modRusCipher.o: modRusCipher.cpp modRusCipher.h
	$(CXX) $(CXXFLAGS) -c modRusCipher.cpp

test_modRusCipher.o: test_modRusCipher.cpp modRusCipher.h
	$(CXX) $(CXXFLAGS) -c test_modRusCipher.cpp

# Очистка
clean:
	rm -f *.o RusGronsfeldTest

# Запуск тестов
test: RusGronsfeldTest
	./RusGronsfeldTest

.PHONY: all clean test
