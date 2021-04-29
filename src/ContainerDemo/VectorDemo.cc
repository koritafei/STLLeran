/**
 * @Copyright (c) 2021  koritafei
 * @file VectorDemo.cc
 * @brief Vector TEST
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-18 11:04:30
 * 
 * */

#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>

namespace TestSTL{

void TestVector() {
  std::cout << "Test Vector" << std::endl;
  std::vector<std::string> v;
  char buff[10];
  clock_t timeStart = std::clock();

  for (int i = 0; i < 10000;i++) {
    try{
      snprintf(buff, 10, "%d", rand());
      v.push_back(std::string(buff));
    } catch (std::exception &p) {
      std::cout << "i = " << i << " " << p.what() << std::endl;
      abort();
    }
  }

  std::cout << "Milli-seconds : " << (clock() - timeStart) << std::endl;
  std::cout << "vector size : " << v.size() << std::endl;
  std::cout << "vector front : " << v.front() << std::endl;
  std::cout << "vector back : " << v.back() << std::endl;
  std::cout << "vector data : " << v.data() << std::endl;
}

}

int main(int argc, char **argv) {
  TestSTL::TestVector();
}
