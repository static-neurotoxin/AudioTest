#include <iostream>
#include <cmath>

#include <boost/program_options.hpp>
#include <boost/version.hpp>

#include "AudioGenerator.h"
#include "AudioWriter.h"

int main(int argc, const char *argv[])
{
    boost::program_options::options_description desc("audiogenerator [options] input_file");
    desc.add_options()
            ("help,h",    "display help options")
            ("version,v", "display version information");

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);

    if(vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    if(vm.count("version"))
    {
        std::cout << "AudioGenerator: 0.0.1a" << std::endl;
        std::cout << "Boost version: "
                  << (BOOST_VERSION / 100000)
                  << '.'
                  << (BOOST_VERSION / 100 % 1000)
                  << '.'
                  << (BOOST_VERSION % 100)
                  << std::endl;
    }

    std::function<double(double)> func = [](double arg) -> double { return std::sin(arg); };

    PeriodicGenerator sine(func);
    sine.setDuration(1);
    sine.setdBFS(-10);
    sine.setFrequency(1000);
    sine.setPhase(0);

    SoxDatWriter writer("test.dat");
    writer.write(sine, 8000);

    WaveWriter wWriter("test.wav", false);
    wWriter.write(sine, 8000);

    return 0;
}