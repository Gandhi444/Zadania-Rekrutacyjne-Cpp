#include <Wire.h>
#include <Arduino.h>
//Led Output state register addres
#define LEDOUT_REG 0x08

//Adresses for PWM registers
#define REG_PWM_R 0x02

#define REG_PWM_G 0x03

#define REG_PWM_B 0x04

#define REG_PWM_W 0x05

//LED driver x individual brightness can be controlled through its PWMx register.
#define LDR_STATE_IND 0x02


class PCA9633PW
{
private:
    TwoWire *_wire;
    uint8_t _deviceAddres;
    //Writes data to a specified registery
    void WriteReg(const uint8_t registerAddress, const uint8_t data);
    //Reads data from a spcefied registery
    uint8_t ReadReg(const uint8_t registerAddress);
    //Converts given HSL values to RGB and returns them as 4 bytes where first byte is 0 if conversion was succesfull and remaining three store r,g,b values respectively
    uint32_t ConvertHSLtoRGB(const float hue,const float saturation,const float lightness);
public:
    //Constructor taking devives I2C adrres and handle to TwoWire object
    PCA9633PW(const uint8_t deviceAddress, TwoWire *wire);
    //Sets LdrState for all led channels
    void SetLdrStateAll(const uint8_t state);
    //Sets pwm values for all of the channels
    void SetRGBW(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t w);
    //Increments value of a given channel each step with delay between steps
    void AscendOneChannel(const uint8_t startingValue,const uint8_t finalValue,const uint8_t PWMChannel,const int delayTime,uint8_t step);
    //Sets pwm values for all of the channels from 8 digits long hex string
    void SetRGBWFromHex(const String &hexCode);
    //Sets pwm values for r,g,b channels after converting given HSL color and sets white pwm to 0
    void SetRGBWFromHSL(const float hue,const float saturation,const float lightness);
    //Increments lightness of a given HSL color in every step
    void AscendLightness(const float startingLightnessValue,const float finalLightnessValue,const float hue,const float saturation,const int delayTime,const float step);
    //Increments hue of a given HSL color in every step
    void Rainbow(const float startingHueValue,const float finalHueValue,const float delayTime,const float step);
};  


