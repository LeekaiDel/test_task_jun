#ifndef MY_LIST_H
#define MY_LIST_H
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdint>

/*  Реализация кастомного двусвязного списка с дополнительным указателем rand,
    который сслается на произвольный узел списка
*/
class ListNodeWrapper
{
public:
    struct ListNode { // ListNode модифицировать нельзя
        std::string data; // произвольные пользовательские данные 
        ListNode*   prev = nullptr; // указатель на предыдущий элемент или nullptr
        ListNode*   next = nullptr;
        ListNode*   rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr` 
    };

    // Метод позволяет добавить данные в конец списка, указатель rand остается nullptr (в итоге не понадобился)
    void push_back(std::string data);

    // Заполняем структуру из вектора пар. Данный метод сразу создает структуру с указателем rand на нужное поле
    void write_pair_vector(std::vector< std::pair<std::string, int> > pair_vector);
    // Читаем структуру в вектор пар. Данный метод сразу возвращает структуру с указателем rand на нужное поле
    void read_pair_vector(std::vector< std::pair<std::string, int> > &pair_vector);

    // Метод позволяет получить данные по индексу как для обычного двусвязного списка
    std::string at(int index);
    // Метод позволяет получить данные по интексу по связи через указатель rand
    std::string at_via_rand(int index);
    // Метод возвращает количество узлов в списке
    int32_t size() { return sz; };
    ~ListNodeWrapper();

private:
    ListNode* head = nullptr;
    ListNode* tail = nullptr;
    int32_t sz = 0;             // Количество узлов в списке
};

// Класс отвечает за функции ввода и вывода данных
class DataSerialisation
{
public:
    ListNodeWrapper list_node_wrapper;
    // Читаем входные данные и сразу формируем из них двусвязный список
    int read_inpt_data(std::string file_name);
    // Сериализуем наш список в файл
    int write_serialize_data(const char *file_name);
    ~DataSerialisation();
private:
    // Читаем и парсим строки из файла
    int read_lines(std::ifstream *file);

};


#endif // MY_LIST_H
