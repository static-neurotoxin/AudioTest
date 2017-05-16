#pragma once
#ifndef _AUDIO_GENERATOR_H_
#define _AUDIO_GENERATOR_H_

#include<vector>
#include<ctgmath>
#include<functional>

// Base class of audio generator
//
class AudioGenerator
{
public:
    AudioGenerator(double duration = 0, double dBFS = -10) : m_duration(duration), m_dBFS(dBFS) {}
    virtual double get(double offset) const = 0;
    inline float operator[](double offset) const {return get(offset);}

    inline double getDuration() const   {return m_duration;}
    inline double getdBFS() const       {return m_dBFS;}

    inline void setDuration(double duration)    {m_duration = duration;}
    inline void setdBFS(double dBFS)            {m_dBFS = dBFS;}
private:
    double m_duration;
    double m_dBFS;
};

class SilenceGenerator : public AudioGenerator
{
    public:
    SilenceGenerator(double duration) : AudioGenerator(duration) {}
    virtual double get(double) {return 0;}
};

class PeriodicGenerator : public AudioGenerator
{
    public:
        PeriodicGenerator(const std::function<double(double)> &func, double frequency = 0, double phase = 0, double duration = 0, double dBFS = -10)
        : AudioGenerator(duration, dBFS), m_func(func), m_frequency(frequency), m_phase(phase) {}

    inline double getPhase(double offset) const
    {
        double theta((offset * m_frequency * M_PI * 2) + m_phase);
        double phase(std::fmod(theta, M_PI * 2));
        return phase;
    }

    inline double getFrequency() const {return m_frequency;}

    inline void setPhase(double phase)          {m_phase = phase;}
    inline void setFrequency(double frequency)  {m_frequency = frequency;}

    virtual double get(double offset) const
    {
        return m_func(getPhase(offset)) * std::pow(10,getdBFS()/10);
    }


    private:
    std::function<double(double)> m_func;
    double m_frequency;
    double m_phase;
};

inline double squareWave(double theta)
{
    return std::fmod(theta, M_PI * 2) > M_PI ? -1 : 1;
}

inline double sawWave(double theta)
{
    return std::fmod(theta, M_PI * 2) / M_PI - 1;
}

inline double triangleWave(double theta)
{
    double saw = sawWave(theta * 2);
    return (1 - std::max(saw, -saw)) * squareWave(theta);
}


#endif//_AUDIO_GENERATOR_H_