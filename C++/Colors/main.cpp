#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <iterator>

#include "Color.h"

std::pair<ColorFormat,std::string> ValidateColor(const std::string &Color)
{
    std::regex hexFormat("^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$");
    auto hexBegin =std::sregex_iterator(Color.begin(), Color.end(), hexFormat);
    auto hexEnd = std::sregex_iterator();
    if(std::distance(hexBegin,hexEnd))
    {
        return std::make_pair(HEX,hexBegin->str());
    }
    std::regex decimalFormat("^([0-9]{1,3},){3}[0-9]{1,3}$");
    auto decimalBegin =std::sregex_iterator(Color.begin(), Color.end(), decimalFormat);
    auto decimalEnd = std::sregex_iterator();
    if(std::distance(decimalBegin,decimalEnd))
    {
        return std::make_pair(DECIMAL,decimalBegin->str());
    }

    return std::make_pair(INCORRECT,Color);
}
void LoadData(const std::string &fileName,std::vector<Color> &colorsVector)
{
    std::ifstream colorsFile(fileName);
    std::string line;
    if(!colorsFile.is_open())
    {
        std::cout<< "Cannot access the file"<<std::endl;
    } else {
        while (std::getline(colorsFile,line))
        {
            if(!line.empty())
            {
                std::pair validationResult =ValidateColor(line);
                switch (validationResult.first) {
                    case INCORRECT:
                        std::cout<<"Wrong color format "<<validationResult.second<<std::endl;
                        break;
                    case DECIMAL:
                        colorsVector.emplace_back(validationResult.second,validationResult.first);
                        break;
                    case HEX:
                        colorsVector.emplace_back(validationResult.second,validationResult.first);
                    default:
                        break;
            }
            }
        }
    }

    colorsFile.close();
}



int main() {
    std::cout << "Hello, World!" << std::endl;
    std::vector<Color> colorsVector;
    LoadData("colors.txt",colorsVector);
    return 0;
}
