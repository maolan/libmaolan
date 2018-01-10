#include "types.h"


using namespace std;


Package const newPackage()
{
  return Package(new PackageData(new string, new BufferData));
}
