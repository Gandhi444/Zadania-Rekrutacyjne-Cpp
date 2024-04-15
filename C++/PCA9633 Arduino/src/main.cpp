#include <Arduino.h>
#include "PCA9633PW.h"

#define PCA9633PW_ADDRESS (0xc0 >> 1)

PCA9633PW pca9633PW(PCA9633PW_ADDRESS,&Wire);

void setup() 
{
  Serial.begin(9600);
  pca9633PW.SetLdrStateAll(LDR_STATE_IND);

  //Red 0-255
  pca9633PW.SetRGBW(0,0,0,0);
  pca9633PW.AscendOneChannel(0,255,REG_PWM_R,50,1);
  delay(1000);
  //Green 0-255
  pca9633PW.SetRGBW(0,0,0,0);
  pca9633PW.AscendOneChannel(0,255,REG_PWM_G,50,1);
  delay(1000);
  //Blue 0-255
  pca9633PW.SetRGBW(0,0,0,0);
  pca9633PW.AscendOneChannel(0,255,REG_PWM_B,50,1);
  delay(1000);
  //White 0-255
  pca9633PW.SetRGBW(0,0,0,0);
  pca9633PW.AscendOneChannel(0,255,REG_PWM_W,50,1);
  delay(1000);
  //Red and blue 255 from hex
  pca9633PW.SetRGBW(0,0,0,0);
  pca9633PW.SetRGBWFromHex("ff00ff00");
  delay(1000);
  //hue=0 saturation=50 lightness=0-1
  pca9633PW.SetRGBW(0,0,0,0);
  pca9633PW.AscendLightness(0,1,0,1,50,0.01);
  delay(1000);
  
}

void loop()
{
pca9633PW.Rainbow(0,240,50,1);
}

