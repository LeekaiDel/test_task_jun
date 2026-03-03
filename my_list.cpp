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


void ListNodeWrapper::write_pair_vector(std::vector< std::pair<std::string, int> > pair_vector)
{
  // Сначала заполним двусвязный список данныхми из строкового значения пары
  for(const auto& pair : pair_vector)
  {
    if(this->head == nullptr)
    {
      this->head = new ListNode();
      this->head->data = pair.first;
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
      temp_p->next->data = pair.first;
      this->tail = temp_p->next;
    }
    ++sz;
  }

  // Теперь будем перебирать все узлы и сверять их индекс в списке с значением rand
  ListNode* prnt_p = this->head;      // Возьмем первый узел и будем искать узел, на который он ссылается через rand
  for(const auto& pair : pair_vector) // Делаем перебор всех rand в векторе полученных данных из файла
  {
    // Важно, чтобы индексы rand в pair_vector располагались в той же последовательности, что и дата из этого вектора в листе
    ListNode* chld_p = this->head;

    if(pair.second > -1) // Если (rand != -1), то находим узел по индексу и присваиваем его адрес перенту цикла 
    {
      int cntr = 0;
      while(chld_p != nullptr)
      {
        if(cntr == pair.second) // Как только счетчик досчитал до нужного индекса rand - присваиваем указателю rand адрес чаилда
        {
          prnt_p->rand = chld_p;
          prnt_p = prnt_p->next;
          break;
        }
        chld_p = chld_p->next;
        cntr++;
      }
    }
    else // Если индекс rand у текущего перента = -1, тогда указываем, что у него нет узла, на который он бы ссылался через rand
    {
      prnt_p->rand = nullptr;
      prnt_p = prnt_p->next;
    }
  }
}


void ListNodeWrapper::read_pair_vector(std::vector< std::pair<std::string, int> > &pair_vector)
{
  std::vector<std::pair<std::string, int>> temp_v;
  ListNode* temp_ln_p = this->head;
  while(temp_ln_p)
  {
    if(temp_ln_p->rand)
    {
      int cntr = 0;
      ListNode* rand_p = this->head;
      while(rand_p != temp_ln_p->rand)
      {
        rand_p = rand_p->next;
        ++cntr; 
      }
      temp_v.push_back(std::pair<std::string, int>(temp_ln_p->data, cntr));
      // std::cout << "Out: " << temp_ln_p->data << ";" << cntr << std::endl;
    }
    else
    {
      temp_v.push_back(std::pair<std::string, int>(temp_ln_p->data, -1));
      // std::cout << "Out: " << temp_ln_p->data << ";" << -1 << std::endl;
    }
    temp_ln_p = temp_ln_p->next;
  }
  pair_vector = std::move(temp_v);
  // for(const auto& obj : temp_v)  // Debug
  // {
  //   std::cout << "Out: " << obj.first << ";" << obj.second << std::endl;
  // }
}


std::string ListNodeWrapper::at(int index)
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
  return 0;
}


std::string ListNodeWrapper::at_via_rand(int index)
{
  if(index < 0)
    return std::string();

  ListNode* temp_p = this->head;
  int cntr = 0;
  while(temp_p != nullptr)
  {
    if(cntr == index)
    {
      return temp_p->data; 
    }
    temp_p = temp_p->rand;
    cntr++;
  }
  return 0;
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


int DataSerialisation::read_inpt_data(const char *file_name)
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
  std::string str;
  int rand;
  while(std::getline(*file, line))
  {
    std::stringstream ss(line);
    std::getline(ss, str, ';');
    // Задаем ограничение входных строк по длине 1000
    if(str.size() > 1000)
    {
      std::cerr << "Поле data превишает лимит в 1000 символов! Этот узел будет пропущен." << std::endl;
      continue;
    }
    ss >> rand;

    if(temp_pair_v.size() <= 1'000'000)
      temp_pair_v.push_back(std::pair<std::string, int>(str, rand));
    else 
      std::cerr << "Максимальное количество узлов в списке(1 000 000) превышено, все последующие узлы будут пропущены." << std::endl;
    
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
