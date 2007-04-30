#include "Headers/LogTime.h"

#include <string>

void test(int fileId, const std::string &dsetName, const std::string &data)
{
  std::cout << logTime() << "Inside string version" << std::endl;
}


template<typename T>
void test(int fileId, const std::string &dsetName, T &data)
{
  std::cout << logTime() << "Inside Template" << std::endl;
}


int main(int argc, char **argv) {
  int fileId = 123123;
  std::string dsetName("DataSet Name");
  std::string data ("String Data");
  
  int iData = 999;
  
  test (fileId, dsetName, data);
  test (fileId, dsetName, iData);
  return 0;
}
