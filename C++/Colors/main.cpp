#include <iostream>
#include <vector>
#include <fstream>
#include <regex>
#include <iterator>

#include "Color.h"
#include "map"
#include "cmath"

//Validate if passed string with color that is valid and return its type and matched sequence
std::pair<ColorFormat,std::string> ValidateColor(const std::string &Color)
{
    //Regex for hex format
    std::regex hexFormat("^([0-9a-f]{3}|[0-9a-f]{6}|[0-9a-f]{8})$");
    auto hexBegin =std::sregex_iterator(Color.begin(), Color.end(), hexFormat);
    auto hexEnd = std::sregex_iterator();
    if(std::distance(hexBegin,hexEnd))
    {
        return std::make_pair(HEX,hexBegin->str());
    }
    //Regex for decimal format
    std::regex decimalFormat("^([0-9]{1,3},){3}[0-9]{1,3}$");
    auto decimalBegin =std::sregex_iterator(Color.begin(), Color.end(), decimalFormat);
    auto decimalEnd = std::sregex_iterator();
    if(std::distance(decimalBegin,decimalEnd))
    {
        return std::make_pair(DECIMAL,decimalBegin->str());
    }
    //If neither regex match return incorrect color format
    return std::make_pair(INCORRECT,Color);
}
//Load data from passed file into the passed vector of Color objects
void LoadData(const std::string &fileName,std::vector<Color> &colorsVector)
{
    std::ifstream colorsFile(fileName);
    std::string line;
    if(!colorsFile.is_open())
    {
        std::cerr<< "Cannot access the file"<<std::endl;
    } else {
        while (std::getline(colorsFile,line))
        {
            if(!line.empty())
            {
                std::pair validationResult =ValidateColor(line);
                switch (validationResult.first) {
                    case INCORRECT:
                        std::cerr<<"Wrong color format "<<validationResult.second<<std::endl;
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
//returns color with RGBA values equal to average values in vector
Color Mix(const std::vector<Color> &colorsVector)
{   Color outputColor("00000000",HEX);
    //Using additional floats as accumulators to avoid potential rounding to zero or int overflows with large amount of data
    float accumulatorR=0.0;
    float accumulatorG=0.0;
    float accumulatorB=0.0;
    float accumulatorAlpha=0.0;
    for(auto color: colorsVector)
    {
        accumulatorR += color.r/(float)colorsVector.size();
        accumulatorG += color.g/(float)colorsVector.size();
        accumulatorB += color.b/(float)colorsVector.size();
        accumulatorAlpha+=color.alpha/(float)colorsVector.size();
    }
    outputColor.r =std::round(accumulatorR);
    outputColor.g =std::round(accumulatorG);
    outputColor.b =std::round(accumulatorB);
    outputColor.alpha =std::round(accumulatorAlpha);

    outputColor.ConvertRGBtoHSL();
    return outputColor;
}
//returns color with RGBA values equal to the lowest values in vector
Color Lowest(const std::vector<Color> &colorsVector)
{   Color outputColor("00000000",HEX);
    for(auto color:colorsVector)
    {
        outputColor.r=outputColor.r>color.r ? color.r: outputColor.r;
        outputColor.g=outputColor.g>color.g ? color.g: outputColor.g;
        outputColor.b=outputColor.b>color.b ? color.b: outputColor.b;
        outputColor.alpha=outputColor.alpha>color.alpha ? color.alpha: outputColor.alpha;
    }
    outputColor.ConvertRGBtoHSL();
    return outputColor;
}
//returns color with RGBA values equal to the highest values in vector
Color Highest(const std::vector<Color> &colorsVector)
{   Color outputColor("00000000",HEX);
    for(auto color:colorsVector)
    {
        outputColor.r=outputColor.r<color.r ? color.r: outputColor.r;
        outputColor.g=outputColor.g<color.g ? color.g: outputColor.g;
        outputColor.b=outputColor.b<color.b ? color.b: outputColor.b;
        outputColor.alpha=outputColor.alpha<color.alpha ? color.alpha: outputColor.alpha;
    }
    outputColor.ConvertRGBtoHSL();
    return outputColor;
}
//returns color equal to last color in vector with saturation equal to average saturation in the vector
Color MixSaturate(const std::vector<Color> &colorsVector)
{   Color outputColor=colorsVector.back();
    float meanSaturation=0.0;
    for(auto color:colorsVector)
    {
        meanSaturation += color.saturation/colorsVector.size();
    }
    outputColor.saturation=meanSaturation;
    outputColor.ConvertHSLtoRGB();
    return outputColor;
}

//Enum type with all supported modes
enum Mode{
    mix,lowest,highest,mixSaturate
};
//Initialize map used to convert string to enum type for use in switches
void initMap(std::map<std::string,Mode> &Map)
{
    Map["mix"]=mix;
    Map["lowest"]=lowest;
    Map["highest"]=highest;
    Map["mix-saturate"]=mixSaturate;
}


int  main(int argc, char *argv[]) {
    std::vector<Color> colorsVector;
    LoadData("colors.txt",colorsVector);

    std::map<std::string,Mode> argToEnum; //Map used to convert string to enum
    initMap(argToEnum);
    Mode mode=mix;
    for(int i=1;i<argc;i++)//Iterating and parsing arguments
    {
        std::string arg=argv[i];
        if(arg=="-m"|arg=="--mode")
        {
            if((i+1)<argc)
            {
                std::string nextArg=argv[i+1];
                i++;
                std::transform(nextArg.begin(), nextArg.end(), nextArg.begin(),
                               [](unsigned char c){ return std::tolower(c); });
                if(argToEnum.find(nextArg)==argToEnum.end())
                {
                    std::cerr <<"specified incorrect mode ignoring it:"<<nextArg<<std::endl;
                } else mode=argToEnum[nextArg];
            }
        } else
        {
            std::pair validationResult =ValidateColor(arg);
            switch (validationResult.first) {
                case INCORRECT:
                    std::cerr<<"Wrong color format "<<validationResult.second<<std::endl;
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

    switch (mode) {//call appropriate function depending on specified mode
        case mix:
        {
           std::cout<< Mix(colorsVector)<<std::endl;
            break;
        }
        case lowest:
            std::cout<< Lowest(colorsVector)<<std::endl;
            break;
        case highest:
            std::cout<< Highest(colorsVector)<<std::endl;
            break;
        case mixSaturate:
            std::cout<< MixSaturate(colorsVector)<<std::endl;
            break;
    }
    return 0;
}
