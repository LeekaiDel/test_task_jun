#include "my_list.hpp"


int main()
{
  DataSerialisation ds;
  ds.read_inpt_data("../inlet.in");
  // std::cout << ds.list_node_wrapper.size() << std::endl;
  ds.write_serialize_data("outlet.out");

  return 0;
}