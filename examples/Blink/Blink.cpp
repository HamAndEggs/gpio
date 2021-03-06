#include <iostream>
#include <signal.h>
#include <unistd.h>

#include <chrono>
#include <thread>
#include <linux/i2c-dev.h>

#include "../../TinyGPIO.h"

bool KeepGoing = true;

void static CtrlHandler(int SigNum)
{
    static int numTimesAskedToExit = 0;
    std::cout << std::endl << "Asked to quit, please wait" << std::endl;
    if( numTimesAskedToExit > 2 )
    {
        std::cout << "Asked to quit to many times, forcing exit in bad way" << std::endl;
        exit(1);
    }
    KeepGoing = false;
}

#define LED_PIN 17

int main(int argc, char *argv[])
{
// Say hello to the world!
    std::cout << "Hello world, a skeleton app generated by appbuild.\n";

// Display the constants defined by app build. \n";
    std::cout << "Build date and time " << APP_BUILD_DATE_TIME << '\n';
    std::cout << "Build date " << APP_BUILD_DATE << '\n';
    std::cout << "Build time " << APP_BUILD_TIME << '\n';

    signal (SIGINT,CtrlHandler);

    tinygpio::GPIOMem Pins;

    if( Pins.Open() )
    {
        Pins.SetPinMode(LED_PIN,tinygpio::GPIOMem::PINMODE_OUT);

        while(KeepGoing)
        {
            Pins.SetPin(LED_PIN,true);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            Pins.SetPin(LED_PIN,false);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    Pins.Close();

    return EXIT_SUCCESS;
}
