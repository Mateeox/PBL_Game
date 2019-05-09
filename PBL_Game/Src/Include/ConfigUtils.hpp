#pragma once
#include <vector>
#include <string>
#include <variant>

using TypeVariant = std::variant<int, unsigned long, float, double, std::string>;
namespace ConfigUtils
{
template<typename T>
T GetTypeFromString(std::string &aType, std::string valueInString)
{
    T tegotypu;

    if (aType == "i")
    {
        tegotypu = std::stoi(valueInString);
        return tegotypu;
    }
    else if (aType == "u")
    {
        tegotypu = std::stoul(valueInString);
        return tegotypu;
    }
    else if (aType == "f")
    {
        tegotypu = std::atof(valueInString.c_str());
        return tegotypu;
    }
    else if (aType == "d")
    {
          tegotypu = std::stod(valueInString.c_str());
        return tegotypu;
    }
    else
    {
        tegotypu = valueInString;
        return tegotypu;
    }
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

} // namespace ConfigUtils