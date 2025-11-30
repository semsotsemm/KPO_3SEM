#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>

const uint8_t TYPE_INT = 0x01;
const uint8_t TYPE_STRING = 0x02;

int main() {
    setlocale(LC_ALL, "Russian");

    std::ifstream inFile("data.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Ошибка: Файл 'data.bin' не найден. Сначала запустите Проект 1." << std::endl;
        return 1;
    }

    // Файл для записи ассемблерного кода
    std::ofstream asmFile("generated_data.inc");

    // Пишем начало секции данных
    asmFile << "; Сгенерированный файл данных\n";
    asmFile << ".data\n";

    std::cout << "Десериализация...\n";

    uint8_t typeMarker;
    int varCounter = 1; // Для генерации уникальных имен переменных

    // Читаем файл байт за байтом, ищем маркеры типов
    while (inFile.read(reinterpret_cast<char*>(&typeMarker), sizeof(typeMarker))) {

        if (typeMarker == TYPE_INT) {
            int32_t value;
            if (inFile.read(reinterpret_cast<char*>(&value), sizeof(value))) {
                std::cout << "Найдено INT: " << value << std::endl;

                // Генерация кода ассемблера: var_int1 dd 12345
                asmFile << "var_int" << varCounter << " dd " << value << "\n";
            }
        }
        else if (typeMarker == TYPE_STRING) {
            uint8_t len;
            if (inFile.read(reinterpret_cast<char*>(&len), sizeof(len))) {
                std::vector<char> buffer(len);
                inFile.read(buffer.data(), len);

                std::string strVal(buffer.begin(), buffer.end());
                std::cout << "Найдено STRING: " << strVal << std::endl;

                // Генерация кода ассемблера: var_str1 db 'text', 0
                asmFile << "var_str" << varCounter << " db '" << strVal << "', 0\n";
            }
        }
        else {
            std::cerr << "Неизвестный тип данных или нарушение структуры!" << std::endl;
            break;
        }
        varCounter++;
    }

    inFile.close();
    asmFile.close();

    std::cout << "Код ассемблера сгенерирован в 'generated_data.inc'" << std::endl;

    return 0;
}