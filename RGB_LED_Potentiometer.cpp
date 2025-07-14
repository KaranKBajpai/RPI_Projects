#include <wiringPi.h>
#include <iostream>
#include <stdio.h>
#include <softPwm.h>
#include <ADCDevice.hpp>

#define ledRedPin 3     // Define 3 pins for RGBLED
#define ledGreenPin 2
#define ledBluePin 0

ADCDevice *adc; // Define an ADC Device class object

int main (void) 
{
    adc = new ADCDevice ();
    std::cout << "Program is starting... " << std::endl;

    if (adc->detectI2C(0x48))
    {
        delete adc;
        adc = new PCF8591 ();
    }
    else if(adc->detectI2C(0x4b))
    {
        delete adc;
        adc = new ADS7830 ();
    }
    else
    {
        std::cout << "No correct 12C address found," << std::endl;
        std::cout << "Please use command 'i2cdetect -y 1' to check the I2C address!" << std::endl;
        std::cout << "Program exit." << std::endl;
        return -1; 
    }

    wiringPiSetup();
    softPwmCreate (ledRedPin, 0, 100);
    softPwmCreate (ledGreenPin, 0, 100);
    softPwmCreate (ledBluePin, 0, 100);

    std::cout << std::endl;

    while (1)
    {
        int val_Red = adc->analogRead(0);
        int val_Green = adc->analogRead(1);
        int val_Blue = adc->analogRead(2);
        softPwmWrite(ledRedPin, val_Red*100/255);
        softPwmWrite(ledGreenPin, val_Green*100/255);
        softPwmWrite(ledBluePin, val_Blue*100/255);
        std::cout << "ADC Value val_Red: " << val_Red << std::endl;
        std::cout << "ADC Value val_Green: " << val_Green << std::endl;
        std::cout << "ADC Value val_Blue: " << val_Blue << std::endl;
        delay (100);
    }
    return 0;
}