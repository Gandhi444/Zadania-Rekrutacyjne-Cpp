//
// Created by anton on 10.04.2024.
//

#ifndef COLORS_COLOR_H
#define COLORS_COLOR_H
#include <iostream>
enum ColorFormat{
    HEX=1,
    DECIMAL=2,
    INCORRECT=0
};


class Color {
private:

public:
    Color()=delete;
    Color(std::string colorString,ColorFormat format);

    unsigned int r=0;
    unsigned int g=0;
    unsigned int b=0;
    unsigned int alpha=0;
    float hue=0;
    float saturation=0;
    float lightness=0;

    void ConvertRGBtoHSV();

    friend std::ostream& operator<<(std::ostream& stream, const Color& color);
};


#endif //COLORS_COLOR_H
