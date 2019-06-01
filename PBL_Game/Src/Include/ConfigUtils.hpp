
#include <variant>
#include <tinyxml2.h>
#include <string>
#include <unordered_map>

using VariantType = std::variant<int,unsigned,float,double,std::string>;
namespace ConfigUtils
{

template<typename T>
T GetValueFromMap(const std::string & name,const std::unordered_map<std::string,VariantType> & aConfigMap)
{
  VariantType value = aConfigMap.at(name);
  return std::get<T>(value);
}

void GetVariantValueAndInsertToMap(tinyxml2::XMLElement *xmlelemnt,std::unordered_map<std::string,VariantType> & aConfigMap);

}