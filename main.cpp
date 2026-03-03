#include "my_list.hpp"


int main(int argc, char* argv[])
{
  std::string filename;
  if (argc > 1) {
      filename = argv[1];
  } else {
      // Иначе запрашиваем вручную
      std::cout << "Введите путь к бинарному файлу: ";
      std::getline(std::cin, filename);
  }

  DataSerialisation ds;
  ds.read_inpt_data(filename);
  // std::cout << ds.list_node_wrapper.size() << std::endl;
  ds.write_serialize_data("outlet.out");

  return 0;
}