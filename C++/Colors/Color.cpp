//
// Created by anton on 10.04.2024.
//

#include <cmath>
#include <sstream>
#include "Color.h"
#include "cinttypes"
#include "vector"
#include "algorithm"
#include "iomanip"

//Convert RGB to HSL based on https://www.rapidtables.com/convert/color/rgb-to-hsl.html
void Color::ConvertRGBtoHSL()
{
    float normalizedR=r/255.0;
    float normalizedG=g/255.0;
    float normalizedB=b/255.0;
    float cMax=std::max({normalizedR,normalizedG,normalizedB});
    float cMin=std::min({normalizedR,normalizedG,normalizedB});
    float delta=cMax-cMin;
    float rC=(cMax-normalizedR)/delta;
    float bC=(cMax-normalizedB)/delta;
    float gC=(cMax-normalizedG)/delta;
    float hPrim=0;
    if(delta==0)
    {
        hPrim=0;
    }else if(cMax==normalizedR)
    {
        hPrim=bC-gC;

    } else if (cMax==normalizedG)
    {
        hPrim=rC-bC;
    } else if (cMax==normalizedB)
    {
        hPrim=gC-rC;
    }
    hue=hPrim*60;
    if(hue<0.0)
    {
        hue=360+hue;
    }
    lightness=(cMax+cMin)/2;
    if(delta==0){
        saturation=0;
    } else
    {
        saturation=delta/(1- std::fabs(2*lightness-1));
    }

}
//Convert HSL to RGB based on https://www.rapidtables.com/convert/color/hsl-to-rgb.html
void Color::ConvertHSLtoRGB() {
    float C=(1-std::fabs((2*lightness)-1))*saturation;
    int i=hue/60;
    float X=C*(1-abs((i%2)-1));
    float m=lightness-(C/2);
    switch (i) {
        case 0: {
            r = (C+m)*255;
            g = (X+m)*255;
            b = m*255;
            break;
        }
        case 1:
        {
            r = (X+m)*255;
            g = (C+m)*255;
            b = m*255;
            break;
            }
        case 2:
        {
            r = m*255;
            g = (C+m)*255;
            b = (X+m)*255;
            break;
        }

        case 3:
        {
            r = m*255;
            g = (X+m)*255;
            b = (C+m)*255;
            break;
        }
        case 4:
        {
            r = (X+m)*255;
            g = m*255;
            b = (C+m)*255;
            break;
        }
        case 5:
        {
            r = (C+m)*255;
            g = m*255;
            b = (X+m)*255;
            break;
        }
        default:
            std::cerr<<"incorect HSV color"<<std::endl;
            break;
    }
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
            size_t pos;
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

    ConvertRGBtoHSL();
    std::cout<<*this<<std::endl;
}
//Overloaded ostream operator for printing
std::ostream& operator<<(std::ostream& stream, const Color& color)
{
    stream << "Red: " << color.r << std::endl;
    stream << "Green: " << color.g << std::endl;
    stream << "Blue: " << color.b << std::endl;
    stream << "Alpha: " << color.alpha << std::endl;
    //Adding padding in cases when hex representation needs leading 0
    std::stringstream stringStreamR;
    stringStreamR << std::setfill ('0') << std::setw(2)
                  << std::hex << color.r;
    std::stringstream stringStreamG;
    stringStreamG << std::setfill ('0') << std::setw(2)
                  << std::hex << color.g;
    std::stringstream stringStreamB;
    stringStreamB << std::setfill ('0') << std::setw(2)
                  << std::hex << color.b;
    std::stringstream stringStreamAlpha;
    stringStreamB << std::setfill ('0') << std::setw(2)
                  << std::hex << color.alpha;

    stream << "Hex: " << "#"  << stringStreamR.str()  << stringStreamG.str()
           << stringStreamB.str()  <<stringStreamAlpha.str() << std::dec << std::endl;
    stream << "Hue: " << color.hue << std::endl;
    stream << "Saturation: " << color.saturation << std::endl;
    stream << "Lightness: " << color.lightness << std::endl;
    return stream;
}