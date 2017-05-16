#include <iostream>
#include <cmath>

#include "AudioGenerator.h"
#include "AudioWriter.h"

int main(int argc, const char *argv[])
{
    std::cout << "Hello World" << std::endl;

    std::function<double(double)> func = [](double arg) -> double { return std::sin(arg); };

    PeriodicGenerator sine(squareWave);
    sine.setDuration(1);
    sine.setdBFS(-10);
    sine.setFrequency(1000);
    sine.setPhase(0);

    SoxDatWriter writer("test.dat");
    writer.write(sine, 8000);

    return 0;
}