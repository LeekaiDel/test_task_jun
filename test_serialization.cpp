#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

int main(int argc, char* argv[]) {
    // 1. Получение пути к файлу
    std::string filename;
    if (argc > 1) {
        // Если путь передан как аргумент командной строки
        filename = argv[1];
    } else {
        // Иначе запрашиваем вручную
        std::cout << "Введите путь к бинарному файлу: ";
        std::getline(std::cin, filename);
    }

    // 2. Открытие файла в бинарном режиме для чтения
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл '" << filename << "'" << std::endl;
        return 1;
    }

    // 3. Чтение общего количества узлов (int32_t = 4 байта)
    int32_t count = 0;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));
    
    // Проверка на успешное чтение счетчика
    if (!file && count != 0) { 
        std::cerr << "Ошибка: Файл поврежден или пуст." << std::endl;
        return 1;
    }

    // 4. Построчное чтение и вывод данных
    for (int32_t i = 0; i < count; ++i) {
        // 4.1 Читаем длину строки data (int32_t)
        int32_t len = 0;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));

        if (!file) {
            std::cerr << "Ошибка: Неожиданный конец файла при чтении длины строки узла #" << i << std::endl;
            break;
        }

        // 4.2 Читаем само содержимое строки data
        std::string data;
        if (len > 0) {
            data.resize(len);
            // &data[0] дает доступ к внутреннему буферу строки (C++11)
            file.read(&data[0], len);
        }

        if (!file) {
            std::cerr << "Ошибка: Не удалось прочитать данные узла #" << i << std::endl;
            break;
        }

        // 4.3 Читаем индекс rand (int32_t)
        int32_t rand_index = -1;
        file.read(reinterpret_cast<char*>(&rand_index), sizeof(rand_index));

        if (!file) {
            std::cerr << "Ошибка: Не удалось прочитать индекс rand узла #" << i << std::endl;
            break;
        }

        // 5. Вывод в консоль в формате: <data>;<rand_index>
        std::cout << data << ";" << rand_index << std::endl;
    }

    file.close();
    return 0;
}