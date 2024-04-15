#include "PCA9633PW.h"
//#include <cmath>

//Constructor taking devives I2C adrres and handle to TwoWire object
PCA9633PW::PCA9633PW(const uint8_t deviceAddress, TwoWire *wire)
{
    _deviceAddres=deviceAddress;
    _wire=wire;
    _wire->begin();
}
//Writes data to a specified registery
void PCA9633PW::WriteReg(const uint8_t registerAddress, const uint8_t data)
{
    _wire->beginTransmission(_deviceAddres);
    _wire->write(registerAddress);
    _wire->write(data);
    uint8_t result= _wire->endTransmission();
    if(result!=0)
    {
        Serial.print("Error during transmision");
        Serial.print(result);
    }
}
//Reads data from a spcefied registery
uint8_t PCA9633PW::ReadReg(const uint8_t registerAddress)
{
    _wire->beginTransmission(_deviceAddres);
    _wire->write(registerAddress);
    _wire->endTransmission();

    _wire->requestFrom(_deviceAddres, (uint8_t) 1);

    if (_wire->available() == 1) {
        return _wire->read();
    }
    Serial.print("Error during reading of the registery");
    return -1;
}
//Sets LdrState for all led channels
void PCA9633PW::SetLdrStateAll(const uint8_t state)
{
    uint8_t newReg = ( state << 6
                       | state << 4
                       | state << 2
                       | state << 0 );

    WriteReg(LEDOUT_REG, newReg);
}
//Sets pwm values for all of the channels
void PCA9633PW::SetRGBW(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w)
{
    WriteReg(REG_PWM_R, r);
    WriteReg(REG_PWM_G, g);
    WriteReg(REG_PWM_B, b);
    WriteReg(REG_PWM_W, w);
}
//Increments value of a given channel each step with delay between steps
void PCA9633PW::AscendOneChannel(const uint8_t startingValue,const uint8_t finalValue,const uint8_t PWMChannel,const int delayTime,uint8_t step)
{
    for(uint8_t i=startingValue;i<=finalValue;i++)
    {
        WriteReg(PWMChannel,i);
        delay(delayTime);
    }
}
//Sets pwm values for all of the channels from 8 digits long hex string
void PCA9633PW::SetRGBWFromHex(const String &hexCode)
{
    if(hexCode.length()==8)
    {
        uint32_t color=strtoul(hexCode.c_str(),NULL,16);
        uint8_t w=color&0xFF;
        color >>=8;
        uint8_t b=color&0xFF;
        color >>=8;
        uint8_t g=color&0xFF;
        color >>=8;
        uint8_t r=color&0xFF;
        SetRGBW(r,g,b,w);
    }else Serial.print("Wrong hex code length");
}
//Converts given HSL values to RGB and returns them as 4 bytes where first byte is 0 if conversion was succesfull and remaining three store r,g,b values respectively
uint32_t PCA9633PW::ConvertHSLtoRGB(const float hue,const float saturation,const float lightness)
{
    uint32_t r,g,b;
    uint32_t retVal=0;
    float C=(1-fabs((2*lightness)-1))*saturation;
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
            Serial.print("Incorect HSL color");
            retVal=1;
            return retVal<<24;
            break;
    }
    retVal=(r<<16)|(g<<8)|b;
    return retVal;
}
//Sets pwm values for r,g,b channels after converting given HSL color and sets white pwm to 0
void PCA9633PW::SetRGBWFromHSL(const float hue,const float saturation,const float lightness)
{
    uint32_t rgb=ConvertHSLtoRGB(hue,saturation,lightness);
    if((rgb>>24)!=1)
    {
        uint8_t b=rgb&0xFF;
        rgb >>=8;
        uint8_t g=rgb&0xFF;
        rgb >>=8;
        uint8_t r=rgb&0xFF;
        SetRGBW(r,g,b,0);
    }
}
//Increments lightness of a given HSL color in every step
void PCA9633PW::AscendLightness(const float startingLightnessValue,const float finalLightnessValue,const float hue,const float saturation,const int delayTime,const float step)
{
    for(float lightness=startingLightnessValue;lightness<=finalLightnessValue;lightness+=step)
    {
        SetRGBWFromHSL(hue,saturation,lightness);
        delay(delayTime);
    }
} 
//Increments hue of a given HSL color in every step
void PCA9633PW::Rainbow(const float startingHueValue,const float finalHueValue,const float delayTime,const float step)
{
     for(float hue=startingHueValue;hue<=finalHueValue;hue+=step)
    {
        SetRGBWFromHSL(hue,1,0.5);
        delay(delayTime);
    }
}