#include "my_list.hpp"


void ListNodeWrapper::push_back(std::string data) // Не используется
{
  if(this->head == nullptr)
  {
    this->head = new ListNode();
    this->head->data = data;
  }
  else
  {
    ListNode* temp_p = this->head;
    while(temp_p->next != nullptr)
    {
      temp_p = temp_p->next;
    }
    temp_p->next = new ListNode();
    temp_p->next->prev = temp_p;
    temp_p->next->data = data;
    this->tail = temp_p->next;
  }
  ++sz;
}


void ListNodeWrapper::write_pair_vector(const std::vector< std::pair<std::string, int> > &pair_vector)
{
  std::vector<ListNode*> p_node_list_v;
  // Сначала заполним двусвязный список данныхми из строкового значения пары
  for(const auto& pair : pair_vector)
  {
    if(this->head == nullptr)
    {
      this->head = new ListNode();
      this->head->data = pair.first;
      this->tail = this->head;  // Установим адрес головы в хвост, потому что на этом моменте и начало и конец в одном узле
      p_node_list_v.push_back(this->head);
    }   
    else
    {
      ListNode* temp_p = this->tail;
      if(!temp_p->next)
      {
        temp_p->next = new ListNode();
        temp_p->next->prev = temp_p;
        temp_p->next->data = pair.first;
        this->tail = temp_p->next;
        p_node_list_v.push_back(temp_p->next);
      }
    }
    ++sz;
  }

  // Теперь будем перебирать все узлы и сверять их индекс в списке с значением rand
  for(int i = 0; i < pair_vector.size(); ++i) // 
  {
    int rand_index = pair_vector[i].second;
    if(rand_index >= 0 && rand_index < p_node_list_v.size())
    {
      p_node_list_v[i]->rand = p_node_list_v[rand_index];
    }
  }
}


void ListNodeWrapper::read_pair_vector(std::vector< std::pair<std::string, int> > &pair_vector) const
{
  std::vector<std::pair<std::string, int>> temp_v;
  // Шаг 1: Создаем таблицу соответствия: Адрес узла -> Его индекс
  // Это O(N)
  std::unordered_map<ListNode*, int> node_to_index;
  ListNode* curr = this->head;
  int idx = 0;
  while(curr) 
  {
    node_to_index[curr] = idx++;
    curr = curr->next;
  }

  // Шаг 2: Проходим по списку еще раз и заполняем вектор
  // Это O(N), так как поиск в map занимает в среднем O(1)
  curr = this->head;
  while(curr) 
  {
    if(curr->rand) 
    {
      // Мгновенно получаем индекс через map
      int rand_idx = node_to_index[curr->rand];
      temp_v.push_back({curr->data, rand_idx});
    } 
    else 
    {
      temp_v.push_back({curr->data, -1});
    }
    curr = curr->next;
  }
  pair_vector = std::move(temp_v);
}


std::string ListNodeWrapper::at(int index) const
{
  if(index < 0)
    return std::string();

  int div = int(sz / 2);
  if( (index + 1 - div) <= 0)
  {
    // Реализовать прямой перебор
    ListNode* temp_p = this->head;
    int cntr = 0;
    while(temp_p != nullptr)
    {
      if(cntr == index)
      {
        return temp_p->data; 
      }
      temp_p = temp_p->next;
      cntr++;
    }
  }
  else
  {
    // Реализовать обратный перебор
    ListNode* temp_p = this->tail;
    int cntr = sz - 1;
    while(temp_p != nullptr)
    {
      if(cntr == index)
      {
        return temp_p->data; 
      }
      temp_p = temp_p->prev;
      cntr--;
    }
  }
  return "";
}


ListNodeWrapper::~ListNodeWrapper()
{
  // std::cout << "start ~ListNodeWrapper()" << std::endl;
  ListNode* temp_p = this->head;
  while(temp_p != nullptr)
  {
    ListNode* nxt = temp_p->next;
    delete temp_p;
    temp_p = nxt;
  }
  // std::cout << "end ~ListNodeWrapper()" << std::endl;
}


int DataSerialisation::read_inpt_data(std::string file_name)
{
  std::ifstream file(file_name); // Путь к файлу с входными данными
  if(!file.is_open())
  {
    std::cerr << "Error: Неудалось открыть файл." << std::endl;
    return 1;
  }

  int ret = read_lines(&file);
  std::cout << ret << " -> nodes received." << std::endl;

  file.close();
  return 0;
}


int DataSerialisation::read_lines(std::ifstream *file)
{
  // Временный вектор для хранения пар
  std::vector<std::pair<std::string, int>> temp_pair_v;   
  std::string line;
  // std::string str;
  // int rand;
  while(std::getline(*file, line))
  {
    // Находим последнюю точку с запятой
    size_t delimiter_pos = line.rfind(';');    
    if (delimiter_pos != std::string::npos) 
    {
      std::string str = line.substr(0, delimiter_pos);        // Всё до последней ;
      std::string rand_str = line.substr(delimiter_pos + 1);  // Всё после последней ;
      
      // Проверяем длину данных
      if(str.size() > 1000) { /* ошибка */ continue; }
       
      // Преобразуем индекс (нужен #include <stdlib.h> или просто stoi, но stoi может кидать исключения)
      // Лучше использовать stringstream для надежности или std::stoi с try-catch
      try
      {
        int rand_index = std::stoi(rand_str);
        temp_pair_v.push_back({str, rand_index});
      }
      catch (...) 
      {
        // Обработка ошибки преобразования числа
        std::cerr << "Error parsing rand index in line: " << line << std::endl;
      }
    }

  }

  if(temp_pair_v.size())
  {
    list_node_wrapper.write_pair_vector(temp_pair_v);
  }

  return temp_pair_v.size();
}


int DataSerialisation::write_serialize_data(const char *file_name)
{
  std::ofstream file(file_name, std::ios::binary);
  if(!file.is_open()) 
  {
    return -1;
  }

  std::vector<std::pair<std::string, int>> pair_vector;
  list_node_wrapper.read_pair_vector(pair_vector);

  // for(const auto& obj : pair_vector)  // Debug
  //   std::cout << "Out: " << obj.first << ";" << obj.second << std::endl;
     
  int32_t size = list_node_wrapper.size();
  file.write(reinterpret_cast<const char*>(&size), sizeof(size));

  for(const auto& obj : pair_vector)
  {
    size = static_cast<int32_t>(obj.first.size());
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(obj.first.c_str()), obj.first.size());
    file.write(reinterpret_cast<const char*>(&obj.second), sizeof(obj.second));
  }

  file.close();
  return 0;
}


DataSerialisation::~DataSerialisation()
{

}
