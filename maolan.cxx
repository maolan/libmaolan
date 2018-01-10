#include <iostream>
#include "types.h"


using namespace std;


int main(int argc, char **argv)
{
  Package package(new PackageData(new string, new BufferData));
  auto buffer = package->second;
  auto origin = package->first;
  *origin = "Origin";
  buffer->push_back(0.5);
  cout << *origin << ' ' << buffer->front() << ' ' << buffer->size() << endl;
  cout << "Before " << package.use_count() << endl;
  {
    auto other = package;
    cout << "During " << package.use_count() << endl;
  }
  cout << "After " << package.use_count() << endl;
  return 0;
}
