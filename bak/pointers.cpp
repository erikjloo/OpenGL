#include <iostream>
#include <string.h>

int main()
{
  char* buffer = new char[8]; // allocate 8 bytes of memory to buffer
  std::cout << (int*)buffer << std::endl; // cast to int so i can actually view output

  memset(buffer, 0, 8); // set all bytes in buffer to 0
  std::cout << (int)*buffer << std::endl; // cast to int to view output

  delete[] buffer;
//   delete[] message;
  return 1;
}