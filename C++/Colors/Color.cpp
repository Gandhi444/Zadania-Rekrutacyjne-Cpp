//
// Created by anton on 10.04.2024.
//

#include <sstream>
#include "Color.h"
#include "cinttypes"
#include "vector"
#include "algorithm"
void Color::ConvertRGBtoHSV()
{
    float normalizedR=r/255.0;
    float normalizedG=g/255.0;
    float normalizedB=b/255.0;
    float cMax=std::max({normalizedR,normalizedG,normalizedB});
    float cMin=std::min({normalizedR,normalizedG,normalizedB});
    float delta=cMax+cMin;
    if(delta==0)
    {
        hue=0;
    }else if(cMax==normalizedR)
    {
        hue=60.0*(int((normalizedG-normalizedB)/delta)%6);
    } else if (cMax==normalizedG)
    {
        hue=60.0*(((normalizedB-normalizedR)/delta)+2);
    } else if (cMax==normalizedB)
    {
        hue=60.0*(((normalizedR-normalizedG)/delta)+4);
    }
    if(cMax==0){
        saturation=0;
    } else
    {
        saturation=delta/cMax;
    }
    lightness=cMax;
}


Color::Color(std::string colorString,ColorFormat format)
{
    switch (format) {
        case HEX:
        {
            if(colorString.length()==3)
            {
                r=std::stoul(colorString.substr(0,1), nullptr, 16);
                r=r+r*16;
                g=std::stoul(colorString.substr(1,1), nullptr, 16);
                g=g+g*16;
                b=std::stoul(colorString.substr(2,1), nullptr, 16);
                b=b+b*16;
                alpha=255;
            }else if(colorString.length()==6)
            {
                r=std::stoul(colorString.substr(0,2), nullptr, 16);
                g=std::stoul(colorString.substr(2,2), nullptr, 16);
                b=std::stoul(colorString.substr(4,2), nullptr, 16);
                alpha=255;
            }else if(colorString.length()==8)
            {
                r=std::stoul(colorString.substr(0,2), nullptr, 16);
                g=std::stoul(colorString.substr(2,2), nullptr, 16);
                b=std::stoul(colorString.substr(4,2), nullptr, 16);
                alpha=std::stoul(colorString.substr(6,2), nullptr, 16);
            }else  std::cout<<"Invalid hex format length"<<std::endl;
            break;
        }
        case DECIMAL:
        {
            size_t pos = 0;
            std::string value;
            std::string delimiter = ",";
            std::vector<unsigned int> values;
            while ((pos = colorString.find(delimiter)) != std::string::npos) {
                value = colorString.substr(0, pos);
                values.emplace_back(std::stoul(value));
                colorString.erase(0, pos + delimiter.length());
            }
            values.emplace_back(std::stoul(colorString));
                r = values[0];
                g = values[1];
                b = values[2];
                alpha = values[3];

            break;
        }
        default:
        {
            r = 0;
            g = 0;
            b = 0;
            break;
        }
    }
    ConvertRGBtoHSV();
    std::cout<< *this<<std::endl;
}
std::ostream& operator<<(std::ostream& stream, const Color& color)
{
    stream << "Red: " << color.r << std::endl;
    stream << "Green: " << color.g << std::endl;
    stream << "Blue: " << color.b << std::endl;
    stream << "Alpha: " << color.alpha << std::endl;
    stream << "Hex: " << "#" << std::hex << color.r  << color.g
              << color.b  << color.alpha << std::dec << std::endl;
    stream << "Hue: " << color.hue << std::endl;
    stream << "Saturation: " << color.saturation << std::endl;
    stream << "Lightness: " << color.lightness << std::endl;
    return stream;
}