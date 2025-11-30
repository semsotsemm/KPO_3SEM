#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

// Определение типов для мета-информации
const uint8_t TYPE_INT = 0x01;
const uint8_t TYPE_STRING = 0x02;

int main() {
    setlocale(LC_ALL, "Russian");

    // 1. Определение данных (Вариант 1/9)
    int32_t myInteger = 123456789;        // Целочисленный литерал (4 байта)
    std::string myString = "Hello, ASM!"; // Строка (макс 127 байт)

    // Проверка ограничения на длину строки
    if (myString.length() > 127) {
        std::cerr << "Ошибка: Строка слишком длинная!" << std::endl;
        return 1;
    }

    std::cout << "Исходные данные:\n";
    std::cout << "Integer: " << myInteger << "\n";
    std::cout << "String: " << myString << "\n";

    // 2. Сериализация в файл (построение плана памяти)
    std::ofstream outFile("data.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
        return 1;
    }

    // --- Запись целого числа ---
    // Мета: Тип
    outFile.write(reinterpret_cast<const char*>(&TYPE_INT), sizeof(TYPE_INT));
    // Значение
    outFile.write(reinterpret_cast<const char*>(&myInteger), sizeof(myInteger));

    // --- Запись строки ---
    // Мета: Тип
    outFile.write(reinterpret_cast<const char*>(&TYPE_STRING), sizeof(TYPE_STRING));

    // Мета: Длина (1 байт, т.к. макс 127)
    uint8_t strLength = static_cast<uint8_t>(myString.length());
    outFile.write(reinterpret_cast<const char*>(&strLength), sizeof(strLength));

    // Значение
    outFile.write(myString.c_str(), strLength);

    outFile.close();
    std::cout << "Данные успешно сериализованы в 'data.bin'." << std::endl;

    return 0;
}