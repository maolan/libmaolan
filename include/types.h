#pragma once
#include <deque>
#include <memory>
#include <string>
#include <utility>


typedef std::deque<float> BufferData;
typedef std::shared_ptr<BufferData> Buffer;
typedef std::shared_ptr<std::string> Origin;
typedef std::pair<Origin, Buffer> PackageData;
typedef std::shared_ptr<PackageData> Package;


Package const newPackage()
{
  return Package(new PackageData(new std::string, new BufferData));
}
