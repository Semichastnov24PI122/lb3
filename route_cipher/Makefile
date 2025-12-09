CXX = g++
CXXFLAGS = -std=c++17 -fexec-charset=UTF-8 -finput-charset=UTF-8 -Wall -Wextra
LDFLAGS = -lUnitTest++

# Основные цели
all: RouteCipherDemo RouteCipherTest

# Демонстрационная программа
RouteCipherDemo: route_cipher.o main.o
	$(CXX) $(CXXFLAGS) -o RouteCipherDemo route_cipher.o main.o

# Тестовая программа
RouteCipherTest: route_cipher.o test_route_cipher.o
	$(CXX) $(CXXFLAGS) -o RouteCipherTest route_cipher.o test_route_cipher.o $(LDFLAGS)

# Компиляция объектных файлов
route_cipher.o: route_cipher.cpp route_cipher.h
	$(CXX) $(CXXFLAGS) -c route_cipher.cpp

main.o: main.cpp route_cipher.h
	$(CXX) $(CXXFLAGS) -c main.cpp

test_route_cipher.o: test_route_cipher.cpp route_cipher.h
	$(CXX) $(CXXFLAGS) -c test_route_cipher.cpp

# Очистка
clean:
	rm -f *.o RouteCipherDemo RouteCipherTest

# Запуск демо
demo: RouteCipherDemo
	./RouteCipherDemo

# Запуск тестов
test: RouteCipherTest
	./RouteCipherTest

.PHONY: all clean demo test
