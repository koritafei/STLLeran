/**
 * @Copyright (c) 2021  koritafei
 * @file ArrayDemo.cc
 * @brief STL Array Test
 * @author koritafei (koritafei@gmail.com)
 * @version 0.1
 * @date 2021-04-18 10:04:37
 *
 * */

#include <array>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace TestSTL {
const int DEFAULTSIZE = 1000;

void TestArray() {
  std::cout << "Test Array .... " << std::endl;
  std::array<long, DEFAULTSIZE> c;

  clock_t timeStart = std::clock();

  for (int i = 0; i < DEFAULTSIZE; i++) {
    c[i] = rand();
  }

  std::cout << "Milli-seconds : " << (clock() - timeStart) << std::endl;
  std::cout << "Array size : " << c.size() << std::endl;
  std::cout << "Array front : " << c.front() << std::endl;
  std::cout << "Array back : " << c.back() << std::endl;
  std::cout << "Array data : " << c.data() << std::endl;
}
}  // namespace TestSTL

int main(int argc, char **argv) {
  TestSTL::TestArray();
}
