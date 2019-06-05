#include "ConfigUtils.hpp"

namespace ConfigUtils
{
std::unordered_map<std::string, VariantType> GlobalConfigMap;
  void LoadConfig()
{
  tinyxml2::XMLDocument doc;
  doc.LoadFile("Configuration/CaleTe.xml");

  tinyxml2::XMLElement *pRoot = doc.FirstChildElement();

  if (pRoot != nullptr)
  {
    for (tinyxml2::XMLElement *e = pRoot->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
    {
      GetVariantValueAndInsertToMap(e, GlobalConfigMap);
    }
  }
}

void GetVariantValueAndInsertToMap(tinyxml2::XMLElement *xmlelemnt,std::unordered_map<std::string,VariantType> & aConfigMap)
{
  VariantType value;
  std::string elementType = xmlelemnt->FirstChildElement("Type")->GetText();
  std::string elementName = xmlelemnt->FirstChildElement("Name")->GetText();

  if (elementType == "I")
  {
    value = xmlelemnt->FirstChildElement("Value")->IntText();
    //std::cout << std::get<int>(value) << "\n";
  }
  else if (elementType == "U")
  {
    value = xmlelemnt->FirstChildElement("Value")->UnsignedText();
    //std::cout << std::get<unsigned>(value) << "\n";
  }
  else if (elementType == "F")
  {
    value = xmlelemnt->FirstChildElement("Value")->FloatText();
    //std::cout << std::get<float>(value) << "\n";
  }
  else if (elementType == "D")
  {
    value = xmlelemnt->FirstChildElement("Value")->DoubleText();
   // std::cout << std::get<double>(value) << "\n";
  }
  else if (elementType == "S")
  {
    value = xmlelemnt->FirstChildElement("Value")->GetText();
    //std::cout << std::get<std::string>(value) << "\n";
  }

  if (aConfigMap.find(elementName) == aConfigMap.end())
  {
    aConfigMap[elementName] = value;
  }
  else
  {
    printf("Element already in map change Name !! \n");
  }
  
}
}